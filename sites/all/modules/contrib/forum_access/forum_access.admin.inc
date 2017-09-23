<?php

/**
 * @file
 * forum_access.admin.inc
 *
 * Include file for forum_access.module, containing (sub-)page handling
 * (form_alter) and batch code.
 *
 */

/**
 * Rewrite the forum administration page with our new access rules.
 */
function _forum_access_forum_form(&$form, &$form_state, $is_container) {
  $tid = (isset($form['tid']['#value']) ? $form['tid']['#value'] : NULL);
  if (isset($tid) && !forum_access_access('view', $tid, NULL, FALSE)) {
    drupal_access_denied(); // Deny access if the user doesn't have View access.
    drupal_exit();
  }

  if (isset($tid)) { // edit
    $template_tid = variable_get('forum_access_default_template_tid', 0);
    $settings = _forum_access_get_settings($tid);
  }
  else { // create
    $template_tid = variable_get('forum_access_new_template_tid', NULL);
    $settings = _forum_access_get_settings($template_tid);
  }
  $fa_priority = $settings['priority'];

  foreach (module_implements('node_access_records') as $module) {
    $na_modules[$module] = $module;
  }
  unset($na_modules['forum_access']);
  unset($na_modules['acl']);

  $form['forum_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access control'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );

  $form['forum_access']['permissions'] = _forum_access_forum_permissions_form($is_container);
  $form['forum_access']['template'] = _forum_access_forum_template_form($template_tid);
  $form['forum_access']['#after_build'][] = '_forum_access_forum_form_after_build_template';
  $form['forum_access']['grants'] = _forum_access_forum_grants_form($form_state, $is_container, $settings);
  $form['forum_access']['acl'] = _forum_access_forum_acl_form($form_state, $tid);
  $form['forum_access']['interference'] = _forum_access_forum_interference_form($is_container, $na_modules, $fa_priority);
  $form['forum_access']['troubleshooting'] = _forum_access_forum_troubleshooting_form($is_container, count($na_modules));

  if (!$is_container && isset($tid) && !node_access_needs_rebuild()) {
    $count = db_query("SELECT COUNT(DISTINCT ti.nid) FROM {taxonomy_index} ti WHERE ti.tid = :tid", array(
      ':tid' => $tid,
    ))->fetchField();
    $limit = variable_get('forum_access_update_limit', 20); // for _node_access_rebuild_batch_operation()
    $threshold = variable_get('forum_access_batch_threshold', $limit); // change the variable if you want
    $form['forum_access']['update_limit'] = array(
      '#type' => 'value',
      '#value' => $limit,
    );
    $form['forum_access']['update_choice'] = array(
      '#type' => 'radios',
      '#title' => t('Update the permissions'),
      '#description' => t('<em>If</em> you make any node access changes, then each node in this forum needs to be updated. Hover over the radiobuttons for details.'),
      '#options' => NULL,
      0 => array(
        '#type' => 'radio',
        '#title' => t('for all %count nodes immediately', array('%count' => $count)),
        '#attributes' => array('title' => t('This option is the fastest, but with many nodes it can still take considerable time and memory. If it does not complete, it will leave your !node_access table in an inconsistent state.', array('!node_access' => '{node_access}'))),
        '#return_value' => 0,
        '#default_value' => ($count <= $threshold ? 0 : 1),
        '#parents' => array('forum_access', 'update_choice'),
      ),
      1 => array(
        '#type' => 'radio',
        '#title' => check_plain(t('in batches of !limit now', array('!limit' => $limit))),
        '#attributes' => array('title' => t('The batch option will always work reliably, but it takes longer to complete.')),
        '#return_value' => 1,
        '#default_value' => ($count <= $threshold ? 0 : 1),
        '#parents' => array('forum_access', 'update_choice'),
      ),
      2 => array(
        '#type' => 'radio',
        '#title' => t('rebuild <strong>all</strong> permissions later'),
        '#attributes' => array('title' => t("This option will only set a flag to remind you to rebuild all permissions later; this is useful if you want to make multiple changes to your node access settings quickly and delay the updating until you're done.")),
        '#return_value' => 2,
        '#default_value' => ($count <= $threshold ? 0 : 1),
        '#parents' => array('forum_access', 'update_choice'),
      ),
      '#attributes' => array('class' => array('forum-access-flowed')),
    );
  }
  if (isset($tid)) {
    $form['forum_access']['force_update'] = array(
      '#type' => 'checkbox',
      '#title' => t('Update even if unchanged'),
    );
  }

  // Move some stuff down so our block goes in a nice place.
  $form['submit']['#weight'] = 10;
  $form['delete']['#weight'] = 10;

  $form['#validate'][] = '_forum_access_form_validate';
  $form['#submit'][] = '_forum_access_form_submit';
}

function _forum_access_forum_permissions_form($is_container) {
  $tr = 't';
  $variables = array();
  foreach (array(
    'access content' => 'node',
    'access comments' => 'comment',
    'create forum content' => 'node',
    'post comments' => 'comment',
    'skip comment approval' => 'comment',
    'edit own forum content' => 'node',
    'edit any forum content' => 'node',
    'delete own forum content' => 'node',
    'delete any forum content' => 'node',
    'administer comments' => 'comment',
    'administer forums' => 'forum',
    'administer nodes' => 'node',
    'access content overview' => 'node',
    'view own unpublished content' => 'node',
    'edit own comments' => 'comment',
  ) as $perm => $module) {
    $variables += _forum_access_permission_link($module, $perm, '<em title="' . $perm . '">', '</em>');
  }
  if (!$is_container) {
    $form = array(
      '#type' => 'fieldset',
      '#title' => check_plain($tr('Permissions information')),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[] = array(
      '#type'    => 'markup',
      '#markup'  => '<div>' . t('Note that users need') . '<ul style="margin-top: 0"><li>' .
                              t('the !access_content and !access_comments permissions <strong>AND <em>View</em></strong> to be able to see this forum and its content at all,', $variables) . '</li><li>' .
                              t('the !create_forum_content (and similar) permissions <strong>AND <em>Post</em></strong> to be able to create forum content, and', $variables) . '</li><li>' .
                              t('the !post_comments (and optionally !skip_comment_approval) permission AND <em>Post</em> to be able to post comments/replies;', $variables) . '</li><li>' .
                              t('the !edit_own_forum_content or !edit_any_forum_content (and similar) permissions (<strong>OR <em>Edit</em></strong>) can be added if desired, <strong>plus</strong>', $variables) . '</li><li>' .
                              t('the !delete_own_forum_content or !delete_any_forum_content (and similar) permissions (<strong>OR <em>Delete</em></strong>) if desired;', $variables) . '</li><li>' .
                              t('the !administer_comments (global!) permission <strong>OR <em>Edit</em>/<em>Delete</em></strong> to be able to edit/delete comments;', $variables) . '</li><li>' .
                              t('the !administer_forums permission <strong>AND <em>View</em></strong> to be able to administer forums (and change access!).', $variables) . '</li></ul>' .
                              t('Furthermore note that content which is not published is treated in a different way by Drupal: it can be viewed only by its author (with the !view_own_unpublished_content permission) or by users with the !administer_nodes permission. Unpublished comments and replies are accessible to users with <strong><em>Edit</em> OR <em>Delete</em></strong>, <strong>OR</strong> with the !administer_comments permission, but they are never counted on the forum page.', $variables) . '<br />' .
                              t('The global !edit_own_comments permission is ignored, but the edit/delete forum content permissions are extended to comments; the per-forum <strong><em>Edit</em></strong> and <strong><em>Delete</em></strong> apply to both nodes and comments, too.', $variables) . '</div>',
    );
    return $form;
  }
}

function _forum_access_forum_template_form($template_tid) {
  $tr = 't';
  // Load a template:
  $vid = _forum_access_get_vid();
  $form = array(
      '#type' => 'fieldset',
      '#title' => check_plain($tr('Template')),
      '#collapsible' => TRUE,
      '#collapsed' => empty($template_tid),
  );
  $form['taxonomy'][$vid] = _forum_parent_select(NULL, t('Template forum'), 'forum');
  $form['taxonomy'][$vid]['#required'] = FALSE;
  $form['taxonomy'][$vid]['#default_value'] = $template_tid;
  $form['taxonomy'][$vid]['#description'] = t("Select a forum and click !Load to retrieve that forum's settings as a starting point for this forum or container.", array('!Load' => '[' . t('Load') . ']'));
  $form['load_button'] = array(
    '#type'   => 'button',
    '#name'   => 'load_template',
    '#value'  => t('Load'),
    '#submit' => FALSE,
  );
  $form['template_tid'] = array(
    '#type'   => 'value',
    '#value'  => NULL,
  );
  $form['select_by_default'] = array(
    '#type'   => 'checkbox',
    '#title'  => t('Remember this selection.'),
    '#default_value' => FALSE,
  );
  $form['load_for_new'] = array(
    '#type'   => 'checkbox',
    '#title'  => t("Use the selected forum's settings as defaults for new forums and containers."),
    '#default_value' => FALSE,
  );
  return $form;
}

function _forum_access_forum_form_after_build_template($form, &$form_state) {
  if (isset($form_state['clicked_button']['#name']) && $form_state['clicked_button']['#name'] == $form['template']['load_button']['#name']) {
    // Load a setting from a template:
    $template_tid = reset($form_state['input']['forum_access']['template']['taxonomy']);
    $form_state['values']['forum_access']['template']['template_tid'] = $template_tid;
    $form['template']['#collapsed'] = FALSE;
    $form['template']['#attributes']['class'] = array('collapsible');

    $settings = _forum_access_get_settings($template_tid);
    foreach (array('view', 'create', 'update', 'delete') as $grant_type) {
      if (empty($form['grants']['checkboxes'][$grant_type])) {
        continue;
      }
      foreach (element_children($form['grants']['checkboxes'][$grant_type]) as $tid) {
        $checked = array_search($tid, $settings[$grant_type]) !== FALSE;
        $form['grants']['checkboxes'][$grant_type][$tid]['#value'] = ($checked ? $tid : 0);
        $form['grants']['checkboxes'][$grant_type][$tid]['#checked'] = ($checked ? TRUE : FALSE);
      }
    }
    $form['interference']['advanced']['priority']['#value'] = $settings['priority'];
    if ($settings['priority'] != 0) {
      $form['interference']['advanced']['#collapsed'] = FALSE;
    }
  }
  elseif (is_array(reset($form_state['values']['forum_access']['template']['taxonomy']))) {
    $template_tid = current(current($form_state['values']['forum_access']['template']['taxonomy']));
  }
  if (isset($template_tid)) {
    $form['template']['select_by_default']['#value'] = ($template_tid && $template_tid == variable_get('forum_access_default_template_tid', 0));
    $form['template']['load_for_new']['#value'] = ($template_tid && $template_tid == variable_get('forum_access_new_template_tid', 0));
  }
  return $form;
}

function _forum_access_forum_grants_form(&$form_state, $is_container, $settings) {
  $tr = 't';
  $roles = user_roles();
  $administer_forums_roles = user_roles(FALSE, 'administer forums');
  $bypass_node_access_roles = user_roles(FALSE, 'bypass node access');

  $form = array(
    '#type'       => 'item',
    '#theme'      => 'forum_access_table',
    //'#process'    => array('_forum_access_process_grant_form'), // too early
    //'#after_build'=> array('_forum_access_after_build_grant_form'), // too late
    //'#pre_render' => array('drupal_pre_render_markup', '_forum_access_pre_render_grant_form'), // too late
  );
  $forum_vocabulary = taxonomy_vocabulary_load(_forum_access_get_vid());
  if ($is_container) {
    $cols = array(
      'view'   => t('View this container'),
      'create' => t('See this container in the %Forums selection list', array('%Forums' => $forum_vocabulary->name)),
    );
  }
  else {
    $cols = array(
      'view'   => t('View this forum'),
      'create' => t('Post in this forum'),
      'update' => t('Edit posts'),
      'delete' => t('Delete posts'),
    );
  }
  $form['col_ids'] = array(
    '#type'  => 'value',
    '#value' => array_keys($cols),
  );
  foreach ($roles as $rid => $role) {
    $form['rows'][$rid] = array(
      '#type'        => 'item',
      '#markup'      => check_plain($role),
    );
    $special = NULL;
    if (isset($administer_forums_roles[$rid])) {
      $special = t("This role has the '@administer_forums' permission, and granting 'View' enables the role holders to change the settings on this page, including Access Control!", array('@administer_forums' => $tr('administer forums')));
      if (isset($bypass_node_access_roles[$rid])) {
        $special .= ' ' . t("Because the role also has the '@bypass_node_access' permission, it has full access to all nodes either way.", array('@bypass_node_access' => $tr('bypass node access')));
      }
    }
    elseif (isset($bypass_node_access_roles[$rid])) {
      $special = t("This role has the '@bypass_node_access' permission and thus full access to all nodes.", array('@bypass_node_access' => $tr('bypass node access')));
    }
    if (isset($special)) {
      $form['rows'][$rid] += array(
        '#prefix' => '<em><span title="' . $special . '" class="forum-access-special-role">',
        '#suffix' => '</span></em>',
        '#disabled' => TRUE,
      );
    }
    $roles[$rid] = '';
  }
  foreach ($cols as $cid => $ctitle) {
    $form['checkboxes'][$cid] = array(
      '#type'          => 'checkboxes',
      '#options'       => $roles,
      '#default_value' => $settings[$cid],
      '#process'       => array('form_process_checkboxes', '_forum_access_process_grant_form_checkboxes'),
    );
    $form['col_ids'][$cid] = array(
      '#markup' => $ctitle,
      '#tree'   => TRUE,
    );
  }
  $form_state['build_info']['files'][] = _forum_access_module_load_include('admin.inc');
  $form['info1'] = array(
    '#type'            => 'item',
    '#description'     => t('For explanations of <em class="placeholder">special cases</em>, hover your mouse over role names.'),
  );
  if ($is_container) {
    $form['info2'] = array(
      '#type'          => 'item',
      '#description'   => t('Users who can see any forum or container within this one should get the <strong><em>View</em></strong> grant. <br /> Users who can post to a forum within this container should get the <strong><em>See</em></strong> grant, so that the forum appears in the proper context in the %Forums selection list.', array('%Forums' => $forum_vocabulary->name)),
    );
  }

  drupal_add_css(drupal_get_path('module', 'forum_access') . '/forum_access.css', array('preprocess' => FALSE));
  return $form;
}

function _forum_access_process_grant_form_checkboxes($element, &$form_state, $form_state_complete) {
  $administer_forums_roles = user_roles(FALSE, 'administer forums');
  $bypass_node_access_roles = user_roles(FALSE, 'bypass node access');
  $access = end($element['#parents']);
  if ($access != 'create') {
    foreach (element_children($element) as $rid) {
      if (isset($bypass_node_access_roles[$rid]) && !($access == 'view' && isset($administer_forums_roles[$rid]))) {
        $element[$rid]['#disabled'] = TRUE;
        $element[$rid]['#default_value'] = TRUE;
      }
    }
  }
  return $element;
}

function _forum_access_forum_acl_form(&$form_state, $tid) {
  $tr = 't';
  // Find our moderator ACL:
  if (isset($tid)) { // edit, not new
    $acl_id = _forum_access_get_acl($tid);
    $form = acl_edit_form($form_state, $acl_id, t('Moderators'));
    $form[] = array(
      '#type'   => 'markup',
      '#markup' => '<div>' . t('Moderators receive all grants above.') . '</div>',
      '#weight' => -1,
    );
    $form['note'] = array(
      '#type'   => 'markup',
      '#markup' => '<div><span>' . t('Note: Changes to moderators are not saved until you click [!Save] below.', array('!Save' => $tr('Save'))) . '</span></div>',
    );
    $form['#after_build'][] = '_forum_access_forum_form_after_build_acl0';
    $form['#after_build'] = array_reverse($form['#after_build']);
    $form['#after_build'][] = '_forum_access_forum_form_after_build_acl2';
    return $form;
  }
}

function _forum_access_forum_form_after_build_acl0($element, &$form_state) {
  if (isset($form_state['input']['forum_access']['template']['taxonomy'])) {
    // Get ACL's user_list for the template and replace it before ACL's after_build function gets its shot at it.
    $template_tid = reset($form_state['input']['forum_access']['template']['taxonomy']);
    if ($acl_id = acl_get_id_by_number('forum_access', $template_tid)) {
      $f = _acl_edit_form($acl_id, 'DUMMY');
      $element['user_list']['#value'] = $f['user_list']['#default_value'];
    }
  }
  return $element;
}

function _forum_access_forum_form_after_build_acl2($element, &$form_state) {
  if (!count(unserialize($element['user_list']['#default_value'])) && !count(unserialize($element['user_list']['#value']))) {
    $element['#collapsed'] = TRUE;
    $element['#attributes']['class'][] = 'collapsed'; // We're already 'after build' and have to do this ourselves!
  }
  if ($element['user_list']['#default_value'] != $element['user_list']['#value']) {
    $element['note']['#markup'] = preg_replace('/<span>/', '<span class="warning">', $element['note']['#markup']);
  }
  return $element;
}

function _forum_access_forum_interference_form($is_container, $na_modules, $fa_priority) {
  $tr = 't';
  $l = 'l';
  $form = array();
  if (count($na_modules) && !$is_container) {
    $form = array(
      '#type'        => 'fieldset',
      '#title'       => t('Module interference'),
      '#collapsible' => TRUE,
    );
    $variables = array(
      '%content_type'   => node_type_get_name('forum'),
      '!Forum_Access'   => 'Forum Access',
      '!Content_Access' => $l('Content Access', 'http://drupal.org/project/content_access'),
      '@Content_Access' => 'Content Access',
      '!ACL'            => 'ACL',
      '!module_list'    => '<ul><li>' . implode($na_modules, '</li><li>') . '</li></ul>',
    );
    $description = t('Forums can contain other content types besides %content_type; !Forum_Access will contribute the grants defined above to every node in this forum, but other node access control modules may also contribute their grants, especially to nodes of types other than %content_type.', $variables);
    $form[] = array(
      '#type'        => 'item',
      '#markup'      => '<p>' . t("Besides !Forum_Access (and !ACL) you have installed the following node access module(s): !module_list   The grants of every module are combined for each node. Access can only be granted, not removed &mdash; if a certain module grants a permission, the other(s) cannot deny it.", $variables) . '</p>',
      '#description' => filter_xss($description),
    );

    if (module_exists('content_access')) {
      foreach (array('view', 'update', 'delete', 'per_node') as $type) {
        $value = content_access_get_settings($type, 'forum');
        if (!empty($value)) {
          $ca_interferes = TRUE;
        }
      }
      $ca_priority = content_access_get_settings('priority', 'forum');
      $is_conflict = ($ca_priority >= $fa_priority && !empty($ca_interferes) || $ca_priority > $fa_priority);
      $variables += array(
        '!link'     => l(t('@Content_Access configuration for the %content_type type', $variables), 'admin/structure/types/manage/forum/access', array('html' => TRUE)),
        '%Advanced' => $tr('Advanced'),
      );
      $specifically = ($ca_priority == $fa_priority ? t('Specifically, any grants given by !Content_Access cannot be taken back by !Forum_Access.', $variables) : '');
      if ($is_conflict) {
        $form['by_content_access'] = array(
          '#type'        => 'fieldset',
          '#title'       => 'Content Access',
          '#collapsible' => FALSE,
          '#attributes'  => array('class' => array('error')),
        );
        $form['by_content_access'][] = array(
          '#markup' => '<div>' . t('You have set the !Content_Access module to control access to content of type %content_type&mdash;this can interfere with proper operation of !Forum_Access!', $variables) . " $specifically</div>",
        );
        if ($ca_priority == $fa_priority) {
          $form['by_content_access'][] = array(
            '#markup' => '<div>' . t("Unless you really know what you're doing, we recommend that you go to the !link page and clear all checkboxes. This will instruct @Content_Access to leave the %content_type nodes alone. However, if you put nodes of other content types into forums as well, then these content types will continue to have this problem.", $variables) . '</div>',
          );
        }
        else {
          $variables += array(
            '%ca_priority' => $ca_priority,
            '%fa_priority' => $fa_priority,
          );
          $form['by_content_access'][] = array(
            '#markup' => '<div>' . t("The priority of @Content_Access (%ca_priority) is higher than the priority of !Forum_Access (%fa_priority), which means the latter is <strong>completely disabled</strong> for the %content_type type! Unless you really know what you're doing, we recommend that you go to the !link page, change the priority (under %Advanced) to 0, and clear all checkboxes.", $variables) . '</div>',
          );
        }
        $form['by_content_access'][] = array(
          '#markup' => '<div>' . t("Alternatively, you can give !Forum_Access priority over @Content_Access by either raising the priority of !Forum_Access in every forum above the priority of @Content_Access, or by lowering the priority of @Content_Access for the content types in question below the priority of !Forum_Access.", $variables) . '</div>',
        );
      }
      else {
        $form[] = array(
          '#markup' => '<p>' . t('Note: You have installed the !Content_Access module, which has the capability to grant access to content that would otherwise be protected by !Forum_Access. Be careful when configuring @Content_Access!', $variables) . '</p>',
        );
      }
    }

    $form['advanced'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced'),
      '#collapsible' => TRUE,
      '#collapsed' => !($fa_priority != 0),
    );
    $description = t("If you have no other node access control modules installed, you should leave this at the default 0. <br /> Otherwise you can raise or lower the priority of !Forum_Access' grants. Out of all the grants contributed to a node, only those with the highest priority are used, and all others are discarded.", $variables);
    $form['advanced']['priority'] = array(
      '#type' => 'weight',
      '#title' => check_plain(t('Priority of !Forum_Access node grants in this forum', $variables)),
      '#default_value' => $fa_priority,
      '#description' => filter_xss($description, array('br')),
    );
  }
  return $form;
}

function _forum_access_forum_troubleshooting_form($is_container, $has_interference) {
  if (!$is_container) {
    $tr = 't';
    $l = 'l';
    $variables = array(
      '!Forum_Access'        => $l('Forum Access', 'http://drupal.org/project/forum_access'),
      '!ACL'                 => $l('ACL', 'http://drupal.org/project/acl'),
      '%Module_interference' => t('Module interference'),
      '!Forum_Access-dev'    => $l('Forum&nbsp;Access&nbsp;7.x-1.x-dev', 'http://drupal.org/node/964638', array('html' => TRUE)),
      '!ACL-dev'             => $l('ACL&nbsp;7.x-1.x-dev', 'http://drupal.org/node/694210', array('html' => TRUE)),
      '%devel_node_access'   => 'devel_node_access',
      '!Devel'               => $l('Devel', 'http://drupal.org/project/devel'),
      '!DNA'                 => 'DNA',
      '!debug_mode'          => $l('debug mode', 'admin/config/development/devel', array('fragment' => 'edit-devel-node-access-debug-mode')),
      '!block'               => $l('block', 'admin/structure/block/list'),
      '!dna_summary'         => $l('devel/node_access/summary', 'devel/node_access/summary'),
      '!Rebuild_permissions' => $l($tr('Rebuild permissions'), 'admin/reports/status/rebuild'),
      '!Forum_Access_'       => $l('Forum Access', 'http://drupal.org/project/issues/forum_access'),
      '!ACL_'                => $l('ACL', 'http://drupal.org/project/issues/acl'),
    );
    $form = array(
      '#type' => 'fieldset',
      '#title' => t('Trouble-shooting node access'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[] = array(
      '#type'    => 'item',
      '#markup'  => '<div>' . t("In case of problems, follow these steps until you've got it worked out:") . '<ol style="margin-top: 0"><li>' .
                              t("Update to the 'recommended' !Forum_Access and !ACL releases for your version of Drupal.", $variables) . '</li><li>' .
        ($has_interference ?  t("Read %Module_interference above and update your other node access modules.", $variables) . '</li><li>' : '') .
                              t("Check the release notes of the development snapshots for issues that might have been fixed in !Forum_Access-dev or !ACL-dev since the latest release.", $variables) . '</li><li>' .
                              t("Install the %devel_node_access module (!DNA, part of the !Devel module) and enable its !debug_mode: !DNA will show you all the grants that actually control your nodes in a footer block on each node's page.", $variables) . '</li><li>' .
                              t("Additional insight can be gained from !dna_summary and by enabling the second !DNA !block.", $variables) . '</li><li>' .
                              t("!Rebuild_permissions and check DNA for changes.", $variables) . '</li><li>' .
                              t("Check the issues queues of !Forum_Access_ and !ACL_ for existing reports and possible solutions.", $variables) . '</li><li>' .
                              t("If all of this hasn't helped, then pick ONE node that is misbehaving, look at it using an account that can see the node (and that should NOT have access if that's your problem!), create a new issue in the issues queue, describe the problem... <ul><li> what did you do? </li><li> what did you expect? </li><li> what happened instead? </li></ul> ... and <strong>attach a screenshot of all the DNA records</strong> for that one node. <br /> Be sure to indicate paths (URLs) for every page and module that you mention.") . '</li></ol><br />' .
                              t("Note: You should not keep the !Devel module enabled on a production site.", $variables) . '</div>',
    );
    return $form;
  }
}

function _forum_access_permission_link($module, $permission, $prefix = '', $postfix = '') {
  $permissions = &drupal_static(__FUNCTION__, array());
  if (empty($permissions[$module])) {
    $permissions[$module] = module_invoke($module, 'permission');
  }
  $key = '!' . str_replace(' ', '_', $permission);
  $value = $prefix . l($permissions[$module][$permission]['title'], 'admin/people/permissions', array('fragment' => "module-$module", 'html' => TRUE)) . $postfix;
  return array($key => $value);
}

function theme_forum_access_table($variables) {
  $tr = 't';
  $form = $variables['form'];
  foreach (element_children($form['rows']) as $key) {
    $row = array();
    $row[] = array(
      'data' => drupal_render($form['rows'][$key]),
      'class' => array('grant'),
    );
    foreach (element_children($form['checkboxes']) as $cid) {
      $row[] = array('data' => drupal_render($form['checkboxes'][$cid][$key]), 'class' => array('checkbox'), 'title' => $form['rows'][$key]['#markup']);
    }
    $rows[] = $row;
  }
  $header[] = ($tr('Roles'));
  foreach (element_children($form['col_ids']) as $cid) {
    $header[] = array('data' => drupal_render($form['col_ids'][$cid]), 'class' => array('checkbox'));
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'grants')));
  $output .= drupal_render_children($form);
  return $output;
}

function _forum_access_form_validate($form, &$form_state) {
  global $user;

  if (user_access('bypass node access', $user)) {
    return;
  }
  $access = $form_state['values']['forum_access']['grants']['checkboxes']; // shortcut
  foreach ($access['view'] as $rid => $checked) {
    if ($checked && isset($user->roles[$rid])) {
      return;
    }
  }
  form_set_error('forum_access][view', t('You must assign %View access to a role that you hold.', array('%View' => 'View')));
}

function _forum_access_form_submit($form, &$form_state) {
  $fa_values = $form_state['values']['forum_access']; // shortcut
  $access = $fa_values['grants']['checkboxes']; // shortcut

  // Save template choice.
  $template_tid = reset($fa_values['template']['taxonomy']);
  if ($fa_values['template']['select_by_default']) {
    variable_set('forum_access_default_template_tid', $template_tid);
  }
  elseif (variable_get('forum_access_default_template_tid', 0) == $template_tid) {
    variable_del('forum_access_default_template_tid');
  }
  if ($fa_values['template']['load_for_new']) {
    variable_set('forum_access_new_template_tid', $template_tid);
  }
  elseif (variable_get('forum_access_new_template_tid', 0) == $template_tid) {
    variable_del('forum_access_new_template_tid');
  }

  // Check for changes.
  $is_changed = $is_new = strpos(current_path(), 'admin/structure/forum/add/') === 0;
  $is_changed = $is_changed || !empty($fa_values['force_update']);
  $form_initial_values = $form; // avoid Coder warning
  $form_initial_values = $form_initial_values['forum_access'];
  foreach ($fa_values['grants']['col_ids'] as $grant_type) {
    $defaults = $form_initial_values['grants']['checkboxes'][$grant_type]['#default_value'];
    $defaults = array_flip($defaults);
    foreach ($access[$grant_type] as $rid => $checked) {
      $is_changed = $is_changed || (empty($form_initial_values['grants']['checkboxes'][$grant_type][$rid]['#disabled']) && (!empty($checked) != isset($defaults[$rid])));
    }
  }
  if (!$is_changed && $fa_values['acl']['user_list'] == $form_initial_values['acl']['user_list']['#default_value'] && (empty($fa_values['interference']) || $fa_values['interference']['advanced']['priority'] == $form_initial_values['interference']['advanced']['priority']['#default_value'])) {
    drupal_set_message(t('The content access permissions are unchanged.'));
    return;
  }

  // Remove and re-create records.
  $tid = $form_state['values']['tid'];
  db_delete('forum_access')
    ->condition('tid', $tid)
    ->execute();

  $fa_priority = isset($fa_values['interference']['advanced']['priority']) ? $fa_values['interference']['advanced']['priority'] : 0;
  if (array_key_exists('acl', $fa_values)) {
    $moderators = unserialize($fa_values['acl']['user_list']);
    acl_save_form($fa_values['acl'], $fa_priority);
  }
  $insert = db_insert('forum_access');
  $insert->fields(array('tid', 'rid', 'grant_view', 'grant_update', 'grant_delete', 'grant_create', 'priority'));
  foreach ($access['view'] as $rid => $checked) {
    if (isset($permissions[$rid]['bypass node access'])) {
      // We prefer not to save records for these roles, because they have access anyway.
      if (isset($permissions[$rid]['administer forums']) && $access['view'][$rid]) {
        // For forum administrators, View needs to be saved, ...
      }
      else {
        // ... otherwise forget View.
        $access['view'][$rid] = FALSE;
      }
      if ($access['view'][$rid] || $access['create'][$rid]) {
          $insert->values(array(
          'tid'          => $tid,
          'rid'          => $rid,
          'grant_view'   => (int) (!empty($access['view'][$rid]) && empty($form_initial_values['grants']['checkboxes']['view'][$rid]['#disabled'])),
          'grant_create' => (int) (!empty($access['create'][$rid])),
          'priority'     => (int) $fa_priority,
        ));
      }
    }
    else {
      $insert->values(array(
        'tid'          => $tid,
        'rid'          => $rid,
        'grant_view'   => (int) ($checked && empty($form_initial_values['grants']['checkboxes']['view'][$rid]['#disabled'])),
        'grant_update' => (int) (!empty($access['update'][$rid]) && empty($form_initial_values['grants']['checkboxes']['update'][$rid]['#disabled'])),
        'grant_delete' => (int) (!empty($access['delete'][$rid]) && empty($form_initial_values['grants']['checkboxes']['delete'][$rid]['#disabled'])),
        'grant_create' => (int) (!empty($access['create'][$rid])),
        'priority'     => (int) $fa_priority,
      ));
    }
  }
  $insert->execute();
  $tr = 't';
  $link = l($tr('edit'), 'admin/structure/forum/edit/forum/' . $tid);
  watchdog('access', 'Changed grants for %forum forum.', array('%forum' => $form_state['values']['name']), WATCHDOG_NOTICE, $link);

  if ($form_state['values']['form_id'] != 'forum_form_container') {
    if ($is_new) {
      $acl_id = acl_create_new_acl('forum_access', NULL, $tid);
    }
    else {
      if (!isset($fa_values['update_choice']) || $fa_values['update_choice'] == 2) {
        node_access_needs_rebuild(TRUE);
      }
      elseif ($fa_values['update_choice'] == 0) {
        // update immediately (but use the batch functions anyway
        $save_redirect = $form_state['redirect'];
        $form_state['redirect'] = current_path();
        $context = array();
        $pending_error_messages = drupal_get_messages('error', FALSE);
        $our_error_message_index = (isset($pending_error_messages['error']) ? count($pending_error_messages['error']) : 0);
        _forum_access_update_batch_finished(FALSE, array(), array()); // add our error message (in case we die underway)
        _forum_access_update_batch_operation($tid, 999999, 1, $context);
        $pending_error_messages = drupal_get_messages('error', TRUE); // still alive, get and clear all 'error' messages
        unset($pending_error_messages['error'][$our_error_message_index]); // remove our error message
        $drupal_set_message = 'drupal_set_message';
        foreach ($pending_error_messages['error'] as $message) { // replay any others
          $drupal_set_message($message, 'error');
        }
        _forum_access_update_batch_finished(TRUE, array(), array());
        $form_state['redirect'] = $save_redirect;
      }
      else {
        // mass update in batch mode, modeled after node.module
        $limit = $fa_values['update_limit'];
        $count = db_query("SELECT COUNT(DISTINCT ti.nid) FROM {taxonomy_index} ti WHERE ti.tid = :tid", array(
          ':tid' => (int) $tid,
        ))->fetchField();
        $batch = array(
          'title' => t('Updating content access permissions'),
          'file' => drupal_get_path('module', 'forum_access') . '/forum_access.admin.inc',
          'operations' => array(
            array('_forum_access_update_batch_operation', array($tid, $limit, $count)),
          ),
          'finished' => '_forum_access_update_batch_finished'
        );
        batch_set($batch);
      }
    }
  }
  variable_del('forum_access_rids'); // clear cache
}

/**
 * Batch operation for forum_access_form_submit().
 *
 * This is a multistep operation: we go through all nodes by packs of 20.
 * The batch processing engine interrupts processing and sends progress
 * feedback after 1 second execution time.
 */
function _forum_access_update_batch_operation($tid, $limit, $count, &$context) {
  if (empty($context['sandbox'])) {
    // Initiate multistep processing.
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = $count;
  }

  // Process the next $limit nodes.
  $nids = db_select('taxonomy_index', 'n')
    ->fields('n', array('nid'))
    ->distinct()
    ->condition('n.nid', $context['sandbox']['current_node'], '>')
    ->condition('n.tid', $tid)
    ->orderBy('n.nid')
    ->range(0, $limit)
    ->execute()
    ->fetchCol();
  $nodes = node_load_multiple($nids);
  foreach ($nodes as $node) {
    // To preserve database integrity, only aquire grants if the node
    // loads successfully.
    if (!empty($node)) {
      node_access_acquire_grants($node);
    }
    $context['sandbox']['progress']++;
    $context['sandbox']['current_node'] = $node->nid;
  }

  // Multistep processing: report progress.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Post-processing for forum_access_form_submit().
 */
function _forum_access_update_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The content access permissions have been updated.'));
    cache_clear_all();
  }
  else {
    drupal_set_message(t('The content access permissions have not been properly updated.'), 'error');
  }
}

/**
 * Helper function to retrieve the settings for a forum.
 */
function _forum_access_get_settings($tid = NULL) {
  $return = array('view' => array(), 'create' => array(), 'update' => array(), 'delete' => array(), 'priority' => 0);
  if (!isset($tid)) {
    // Default to all users can read; all logged in users can post.
    $return['view'] = array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID);
    $return['create'] = array(DRUPAL_AUTHENTICATED_RID);
  }
  else {
    $result = db_query("SELECT * FROM {forum_access} where tid = :tid", array(
      ':tid' => $tid,
    ));
    foreach ($result as $access) {
      if ($access->grant_view) {
        $return['view'][] = $access->rid;
      }
      if ($access->grant_update) {
        $return['update'][] = $access->rid;
      }
      if ($access->grant_delete) {
        $return['delete'][] = $access->rid;
      }
      if ($access->grant_create) {
        $return['create'][] = $access->rid;
      }
      if ($access->rid == DRUPAL_AUTHENTICATED_RID) { // this is our reference
        $return['priority'] = $access->priority;
      }
    }
  }
  return $return;
}

/**
 * Remove unusable 'edit' links from overview form.
 */
function _forum_access_forum_overview(&$form, &$form_state) {
  global $user;
  if (user_access('bypass node access', $user)) {
    return;
  }
  foreach ($form as $key => $value) {
    if (preg_match('#^tid:(.*):0$#', $key, $matches)) {
      if (!forum_access_access('view', $matches[1], NULL, FALSE)) {
        $form[$key]['edit']['#access'] = FALSE;
        $form[$key]['view'] = array('#markup' => $form[$key]['view']['#title']);
      }
    }
  }
}

/**
 * Add warnings on Content Access admin forms where CA wants
 * to control the same content types as we do.
 */
function _forum_access_content_access_admin_form($bundle) {
  $tr = 't';
  $l = 'l';
  $variables = array(
    '!Content_Access' => 'Content Access',
    '!Forum_Access' => 'Forum Access',
    '!Forum_Access_link' => $l('Forum Access', 'admin/structure/forum'),
    '%anonymous_user' => $tr('anonymous user'),
    '%authenticated_user' => $tr('authenticated user'),
    '%Advanced' => $tr('Advanced'),
  );
  $dsm = 'drupal_set_message';
  if ($bundle->type == 'forum') {
    $dsm(t('Note: In Drupal, access can only be granted, not taken away. Whatever access you grant here will not be reflected in the !Forum_Access_link settings, but !Forum_Access can only allow <i>more</i> access, not less.', $variables)
      . '<br /><span class="error">' . t('Specifically, any rights granted to the %anonymous_user and/or the %authenticated_user will <b>override</b> the settings of !Forum_Access!', $variables) . '</span>'
      . '<br />' . t('To avoid conflicts with !Forum_Access settings, you may want to lower the priority of !Content_Access (under %Advanced below) below the priority of !Forum_Access for the content types that you want to be controlled by !Forum_Access.', $variables), 'warning');
  }
  else {
    $instances = field_info_instances('node', $bundle->type);
    if (isset($instances['taxonomy_forums'])) {
      $dsm(t('Note: Nodes of this content type can be put inside forums, where access to them will also be controlled by !Forum_Access.<br />In Drupal, access can only be granted, not taken away. Whatever access you grant here will not be reflected on the !Forum_Access_link settings, and vice versa, but any node access module can only allow <i>more</i> access, not less.', $variables), 'warning');
    }
  }
}

