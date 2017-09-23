<?php

/**
 * @file
 * forum_access.node.inc
 *
 * Include file for forum_access.module, containing (sub-)page handling
 * (form_alter) code for the node and comment forms as well as code
 * for temporarily assigning the 'forum moderator' role and managing
 * the resulting rights.
 */

/**
 * Really implements hook_node_view_alter().
 *
 * Removes the "Add new comment" link and form from a node page if the user
 * is not allowed to add comments to that node.
 */
function _forum_access_node_view_alter(&$build, $tid) {
  global $user;

  //dpm($build, '_forum_access_node_view_alter() BEFORE:');
  $node = $build['#node'];
  if (forum_access_node_access($node, 'create', $user) == NODE_ACCESS_DENY) {
    unset($build['links']['comment']['#links']['comment-add']);
    unset($build['comments']['comment_form']);
  }
  //dpm($build, '_forum_access_node_view_alter() AFTER:');
}

/**
 * Really implements hook_comment_view_alter().
 *
 * Adds and removes action links from/to one comment.
 */
function _forum_access_comment_view_alter(&$build, $tid) {
  global $user;

  $node = $build['#node'];
  $comment = $build['#comment'];
  //dpm($build, "_forum_access_comment_view_alter() tid=[$tid] cid=[$comment->cid] BEFORE:");
  $links =& $build['links']['comment']['#links'];

  if ($user->uid != 1 && !user_access('bypass node access') && !forum_access_access('create', $tid)) {
    unset($links['comment-reply']);
  }

  if (!user_access('administer comments') || !empty($user->_forum_access_moderator)) {
    $default_link_keys = array(
      'create' => array('comment-reply'),
      'update' => array('comment-edit', 'comment-approve'),
      'delete' => array('comment-delete'),
    );
    forum_access_enable_moderator();
    $admin_links = comment_links($comment, $node);
    forum_access_enable_moderator(FALSE);
    //dpm($admin_links, "_forum_access_comment_view_alter() ADMINISTRATOR_LINKS for comment/$comment->cid:");
    foreach (array_keys($default_link_keys) as $op) {
      $access = (bool) forum_access_access($op, $tid);
      if ($op != 'create') {
        $perm = ($op == 'update' ? 'edit' : $op);
        $access |= ($user->uid == $comment->uid && user_access("$perm own forum content"));
      }
      $available_keys = variable_get("forum_access_allowed_comment_links_for_$op", $default_link_keys[$op]);
      $old_links = $links;
      $links = array();
      foreach ($default_link_keys[$op] as $key) {
        if ($access && array_search($key, $available_keys) !== FALSE && isset($admin_links[$key])) {
          //dpm($key, '_forum_access_comment_view_alter() ADDED:');
          $links[$key] = $admin_links[$key];
        }
        if (!$access && !empty($old_links[$key])) {
          //dpm($key, '_forum_access_comment_view_alter() REMOVED:');
          unset($old_links[$key]);
        }
      }
      if (is_array($old_links)) {
        $links = array_merge($links, $old_links);
      }
    }
    //dpm($build, "_forum_access_comment_view_alter() tid=[$tid] cid=[$comment->cid] AFTER:");
  }
}

/**
 * Rewrites the taxonomy item on the node form.
 */
function _forum_access_node_form(&$form, &$form_state) {
  global $user;
  $vid = _forum_access_get_vid();

  if (!isset($form['taxonomy_forums'])) {
    return;
  }

  // True node administrators are all powerful and do NOT get their forms rewritten here.
  if (user_access('bypass node access') && empty($user->_forum_access_moderator)) {
    return;
  }

  $roles = array_keys($user->roles);
  $tids = array();
  $result = db_query("SELECT tid FROM {forum_access} WHERE rid IN (:roles) AND grant_create = 1", array(
    ':roles' => $roles,
  ));
  foreach ($result as $obj) {
    $tids[$obj->tid] = $obj->tid;
  }

  // Also get all forums they happen to be able to moderate.
  $result = db_query("SELECT a.number AS tid FROM {acl} a INNER JOIN {acl_user} u ON a.acl_id = u.acl_id WHERE a.module = 'forum_access' AND u.uid = :uid", array(
    ':uid' => $user->uid,
  ));
  foreach ($result as $obj) {
    $tids[$obj->tid] = $obj->tid;
  }

  $nid = $form['nid']['#value'];
  if (!empty($nid)) {
    // Edit an existing node.
    if (!forum_access_access('update', $form['forum_tid']['#value']) && !user_access('edit any forum content') && !(user_access('edit own forum content') && $form['uid']['#value'] == $user->uid)) {
      drupal_access_denied();
      drupal_exit();
    }
    $forum_tid = $form['forum_tid']['#value'];
    $tids[$forum_tid] = $forum_tid;
  }
  else {
    // Create a new node -- ensure the forum they're trying to post to directly
    // is allowed, otherwise there will be much confusion.
    $forum_tid = arg(3);
    if (!empty($forum_tid) && is_numeric($forum_tid) && !isset($tids[$forum_tid])) {
      drupal_access_denied();
      drupal_exit();
    }
  }

  $form_options = &$form['taxonomy_forums'][$form['taxonomy_forums']['#language']]['#options'];
  $options = array();
  foreach ($form_options as $tid => $name) {
    if (!is_numeric($tid)) {
      $options[$tid] = $name;
    }
    elseif (is_object($name)) {
      foreach ($name->option as $sub_tid => $sub_name) {
        if (!empty($tids[$sub_tid])) {
          $options[$tid]->option[$sub_tid] = $sub_name;
        }
      }
    }
    elseif (isset($tids[$tid])) {
      $options[$tid] = $name;
    }
  }
  $form_options = $options;

  // Apply modifications for Moderators (by role or uid).
  if (!user_access('administer nodes') && forum_access_is_moderator($user, $forum_tid)) {
    $allowed_options = variable_get('forum_access_allowed_node_edit_options', array('status', 'sticky', 'subscriptions_notify'));
    foreach (element_children($form) as $key) {
      switch ($key) {
        case 'buttons':
          $tid = $form['taxonomy'][$vid]['#default_value'][0];
          if (!forum_access_access('update', $tid)) {
            $form['buttons']['submit']['#access'] = FALSE;
            $form['buttons']['preview']['#access'] = FALSE;
          }
          if (!forum_access_access('delete', $tid)) {
            $form['buttons']['delete']['#access'] = FALSE;
          }
          break;
        case 'author':
          $form['author']['#disabled'] = TRUE;
          break;
        case 'options':
          foreach (element_children($form['options']) as $key2) {
            if (array_search($key2, $allowed_options) === FALSE) {
              $form['options'][$key2]['#access'] = FALSE;
            }
          }
          $form['options']['#access'] = 1;
          break;
        case 'revision_information':
          $form['revision_information']['#access'] = 1;
          break;
        case 'comment_settings':
          $form['comment_settings']['#access'] = 1;
          break;
        case 'shadow':
          $form['shadow']['#description'] .= '<br />' . t('Note: Access to this topic and its shadows is controlled by the forum that contains the topic.');
          break;
        case 'taxonomy_forums':
          $desc =& $form['taxonomy_forums'][$form['taxonomy_forums']['#language']]['#description'];
          if (!empty($desc)) {
            $desc .= '<br />';
          }
          $desc .= t('Note: Moving a topic to a different forum may change its accessibility.');
      }
    }
  }
}

/**
 * Sanitizes the comment Administration options for users with Edit grants.
 */
function _forum_access_comment_form(&$form, &$form_state) {
  global $user;

  $comment = $form_state['comment'];
  if ($cid = $form['cid']['#value'] && isset($form['author']) && !empty($user->_forum_access_moderator)) {
    $editable_administration_elements = variable_get('forum_access_allowed_comment_edit_administration_elements', array('homepage', 'status', 'subscriptions_notify'));
    foreach (element_children($form['author']) as $key) {
      if (array_search($key, $editable_administration_elements) === FALSE) {
        $form['author'][$key]['#disabled'] = TRUE;
      }
    }
  }
}

