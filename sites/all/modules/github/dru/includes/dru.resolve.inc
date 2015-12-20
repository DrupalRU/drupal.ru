<?php

/**
 * @file
 * Contain functionality for resolve nodes, which can be used as questions.
 */

/**
 * Exclude code from dru_form_node_type_form_alter().
 *
 * @see dru_form_node_type_form_alter()
 */
function dru_add_resolve_setting(&$form, &$form_state, $form_id) {
  if (isset($form['type']) && module_exists('comment')) {
    $type                              = $form['#node_type']->type;
    $form['comment'][DRU_CAN_RESOLVED] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Comments can resolve the question'),
      '#description'   => t('Any node of the content type can be resolved by a comment.'),
      '#default_value' => variable_get(DRU_CAN_RESOLVED . '_' . $type),
    );
  }
}

/**
 * Exclude code from dru_comment_view().
 *
 * @see dru_comment_view()
 */
function dru_add_resolve_link($comment, $view_mode, $langcode) {
  // Get setting.
  $node = node_load($comment->nid);
  if (empty($node->resolved)) {
    $can_resolved = variable_get(DRU_CAN_RESOLVED . '_' . $node->type);
    $token        = dru_resolve_token_to_arg();
    if ($can_resolved && dru_resolve_access($node, $comment, $token)) {
      drupal_add_library('system', 'ajax');
      $links = &$comment->content['links']['comment']['#links'];
      // Add link to comment for resolve the question (node).
      $links['comment-resolve'] = array(
        'title'      => t('resolve'),
        'href'       => _dru_path_to_resolve($comment->nid, $comment->cid),
        'attributes' => array(
          'class' => array('use-ajax'),
        ),
      );
    }
  }
  elseif (!empty($node->resolved_cid) && $node->resolved_cid == $comment->cid) {
    $comment->best = TRUE;
  }
}

/**
 * Performs checking access to resolve a node.
 *
 * @param object $node
 *   Object of checked node.
 *
 * @param mixed  $comment
 *   Object or array of comment which is initiator.
 *
 * @param string $token
 *   Object of checked node.
 *
 * @return bool
 *   TRUE if user has access to resolve the node, FALSE otherwise.
 */
function dru_resolve_access($node, $comment, $token) {
  // We can use the function fron several places and
  // the comment can be as array or object of comment.
  $nid = is_object($comment) && isset($comment->nid) ? $comment->nid : NULL;
  if (!$nid && isset($comment['#node']->nid)) {
    $nid = $comment['#node']->nid;
  }
  // Checks that the comment belongs to the node.
  return $nid == $node->nid
  && (user_access('bypass node access')
    || $GLOBALS['user']->uid == $node->uid)
  && drupal_valid_token($token, 'resolve');
}

/**
 * Ajax callback for mark a node as resolved.
 *
 * @param object $node
 *   Object of a node.
 *
 * @return array
 */
function dru_resolve_node_ajax_callback($node, $comment, $token) {
  $commands = array();
  $keys     = array();
  $record   = array(
    'nid'      => $node->nid,
    'cid'      => $comment->cid,
    'resolved' => TRUE,
  );

  // If the node has mark resolved, then we should just update his.
  if (isset($node->resolved) && $node->resolved == 0) {
    $keys[] = 'nid';
    $keys[] = 'cid';
  }
  drupal_write_record('dru_resolved', $record, $keys);
  $commands[] = dru_ajax_command_resolve();
  return array(
    '#type'     => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Helper for generate page to resolve a node.
 *
 * @param int $nid
 *   Node ID.
 * @param int $cid
 *   Comment ID.
 *
 * @return string
 *   Path to resolve the node, which will be received by nid.
 */
function _dru_path_to_resolve($nid, $cid) {
  return 'node/' . $nid . '/resolve/' . $cid . '/' . dru_resolve_token_to_arg();
}

/**
 * Allows to protect the action from CSRF vulnerabilities.
 *
 * @return string
 *   Generate token for "resolve" action.
 */
function dru_resolve_token_to_arg() {
  $token = &drupal_static(__FUNCTION__);
  if (!isset($token)) {
    $token = drupal_get_token('resolve');
  }
  return $token;
}

/**
 * Adds mark {Resolved] to title of resolved node.
 *
 * @param $build
 */
function dru_add_resolve_mark(&$variables) {
  if (!empty($variables['resolved'])) {
    $variables['title'] = '[' . t('Resolved') . '] ' . $variables['title'];
  }
}
