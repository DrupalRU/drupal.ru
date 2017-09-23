<?php

/**
 * Implements hook_views_default_views().
 */
function flag_abuse_views_default_views() {
  $views = $functions = array();

  if (module_exists('flag')) {
    $api_type = flag_abuse_get_api_type();
    foreach (array_filter(flag_abuse_get_abuse_flags()) as $flag_name) {
      if ($flag = flag_get_flag($flag_name)) {
        $type = $flag->$api_type;
        $function = "_flag_abuse_views_default_abuse_$type";
        if (function_exists($function)) {
          $functions[$function][$flag_name] = $flag->title;
        }
      }
    }
  }

  if (!empty($functions)) {
    foreach ($functions as $function => $flags) {
      $function($views, $flags);
    }
  }

  return $views;
}

function _flag_abuse_views_default_abuse_comment(&$views, $flags) {
  if(FLAG_API_VERSION == 3) {
    $view = new view();
    $view->name = 'all_flag_abuse_comment';
    $view->description = 'View for flag: Comment Abuse ';
    $view->tag = 'flag.abuse';
    $view->base_table = 'comment';
    $view->human_name = '';
    $view->core = 0;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'Comment Abuse';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'administer_nodes';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['pager']['options']['items_per_page'] = 25;
    $handler->display->display_options['style_plugin'] = 'table';
    $handler->display->display_options['style_options']['columns'] = array(
      'timestamp' => 'timestamp',
      'name_1' => 'name_1',
      'title' => 'title',
      'name_2' => 'name_2',
      'entity_id' => 'entity_id',
      'delete_comment' => 'delete_comment',
      'ops' => 'ops',
    );
    $handler->display->display_options['style_options']['default'] = 'timestamp';
    $handler->display->display_options['style_options']['info'] = array(
      'timestamp' => array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'name_1' => array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'name_2' => array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'entity_id' => array(
        'sortable' => 0,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'delete_comment' => array(
        'align' => '',
        'separator' => '',
      ),
      'ops' => array(
        'align' => '',
        'separator' => '',
      ),
    );
    /* No results behavior: Global: Text area */
    $handler->display->display_options['empty']['text']['id'] = 'text';
    $handler->display->display_options['empty']['text']['table'] = 'views';
    $handler->display->display_options['empty']['text']['field'] = 'area';
    $handler->display->display_options['empty']['text']['content'] = 'No comments have been reported...';
    $handler->display->display_options['empty']['text']['format'] = 'plain_text';
    /* Relationship: Flags: abuse_comment */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'comment';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'abuse_comment';
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Comment: Content */
    $handler->display->display_options['relationships']['nid']['id'] = 'nid';
    $handler->display->display_options['relationships']['nid']['table'] = 'comment';
    $handler->display->display_options['relationships']['nid']['field'] = 'nid';
    $handler->display->display_options['relationships']['nid']['required'] = TRUE;
    /* Relationship: Flags: User */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'flagging';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['uid']['required'] = TRUE;
    /* Field: Flags: Flagged time */
    $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['table'] = 'flagging';
    $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['timestamp']['date_format'] = 'custom';
    $handler->display->display_options['fields']['timestamp']['custom_date_format'] = 'j M, Y';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
    $handler->display->display_options['fields']['name_1']['table'] = 'users';
    $handler->display->display_options['fields']['name_1']['field'] = 'name';
    $handler->display->display_options['fields']['name_1']['relationship'] = 'uid';
    $handler->display->display_options['fields']['name_1']['label'] = 'Reported by';
    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['relationship'] = 'nid';
    $handler->display->display_options['fields']['title']['label'] = 'Parent Node';
    /* Field: Comment: Author */
    $handler->display->display_options['fields']['name_2']['id'] = 'name_2';
    $handler->display->display_options['fields']['name_2']['table'] = 'comment';
    $handler->display->display_options['fields']['name_2']['field'] = 'name';
    /* Field: Comment: Comment */
    $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
    $handler->display->display_options['fields']['entity_id']['table'] = 'field_data_comment_body';
    $handler->display->display_options['fields']['entity_id']['field'] = 'comment_body';
    /* Field: Comment: Delete link */
    $handler->display->display_options['fields']['delete_comment']['id'] = 'delete_comment';
    $handler->display->display_options['fields']['delete_comment']['table'] = 'comment';
    $handler->display->display_options['fields']['delete_comment']['field'] = 'delete_comment';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops']['id'] = 'ops';
    $handler->display->display_options['fields']['ops']['table'] = 'flagging';
    $handler->display->display_options['fields']['ops']['field'] = 'ops';
    $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops_1']['id'] = 'ops_1';
    $handler->display->display_options['fields']['ops_1']['table'] = 'flagging';
    $handler->display->display_options['fields']['ops_1']['field'] = 'ops';
    $handler->display->display_options['fields']['ops_1']['relationship'] = 'flag_content_rel_1';
    $handler->display->display_options['fields']['ops_1']['label'] = 'Whitelist';
    /* Sort criterion: Flags: Flagged time */
    $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
    $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
    $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
    $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_content_rel';
    /* Filter criterion: Comment: Approved */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'comment';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = '1';

    /* Display: Page: Comment Abuse */
    $handler = $view->new_display('page', 'Page: Comment Abuse', 'page_abuse_comment');
    $handler->display->display_options['defaults']['relationships'] = FALSE;
    /* Relationship: Flags: abuse_comment */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'comment';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'abuse_comment';
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Comment: Content */
    $handler->display->display_options['relationships']['nid']['id'] = 'nid';
    $handler->display->display_options['relationships']['nid']['table'] = 'comment';
    $handler->display->display_options['relationships']['nid']['field'] = 'nid';
    $handler->display->display_options['relationships']['nid']['required'] = TRUE;
    /* Relationship: Flags: User */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'flagging';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['uid']['required'] = TRUE;
    /* Relationship: Flags: abuse_whitelist_comment */
    $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
    $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'comment';
    $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'whitelist flag';
    $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
    $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'abuse_whitelist_comment';
    $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
    $handler->display->display_options['path'] = 'admin/content/comment/flag-abuse/abuse_comment';
    $handler->display->display_options['menu']['type'] = 'default tab';
    $handler->display->display_options['menu']['title'] = 'Comment Abuse';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['tab_options']['type'] = 'tab';
    $handler->display->display_options['tab_options']['title'] = 'Reported comments';
    $handler->display->display_options['tab_options']['weight'] = '10';
    $views[$view->name] = $view;
  }elseif(FLAG_API_VERSION == 2) {
    $view = new view();
    $view->name = 'all_flag_abuse_comment';
    $view->description = 'View for flag: Comment Abuse ';
    $view->tag = 'flag.abuse';
    $view->base_table = 'comment';
    $view->human_name = '';
    $view->core = 0;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'Comment Abuse';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'administer_nodes';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['pager']['options']['items_per_page'] = 25;
    $handler->display->display_options['style_plugin'] = 'table';
    $handler->display->display_options['style_options']['columns'] = array(
      'timestamp' => 'timestamp',
      'name_1' => 'name_1',
      'title' => 'title',
      'name_2' => 'name_2',
      'entity_id' => 'entity_id',
      'delete_comment' => 'delete_comment',
      'ops' => 'ops',
    );
    $handler->display->display_options['style_options']['default'] = 'timestamp';
    $handler->display->display_options['style_options']['info'] = array(
      'timestamp' => array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'name_1' => array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'name_2' => array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'entity_id' => array(
        'sortable' => 0,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'delete_comment' => array(
        'align' => '',
        'separator' => '',
      ),
      'ops' => array(
        'align' => '',
        'separator' => '',
      ),
    );
    /* No results behavior: Global: Text area */
    $handler->display->display_options['empty']['text']['id'] = 'text';
    $handler->display->display_options['empty']['text']['table'] = 'views';
    $handler->display->display_options['empty']['text']['field'] = 'area';
    $handler->display->display_options['empty']['text']['content'] = 'No comments have been reported...';
    $handler->display->display_options['empty']['text']['format'] = 'plain_text';
    /* Relationship: Flags: abuse_comment */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'comment';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'abuse_comment';
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Comment: Content */
    $handler->display->display_options['relationships']['nid']['id'] = 'nid';
    $handler->display->display_options['relationships']['nid']['table'] = 'comment';
    $handler->display->display_options['relationships']['nid']['field'] = 'nid';
    $handler->display->display_options['relationships']['nid']['required'] = TRUE;
    /* Relationship: Flags: User */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'flag_content';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['uid']['required'] = TRUE;
    /* Field: Flags: Flagged time */
    $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['table'] = 'flag_content';
    $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['timestamp']['date_format'] = 'custom';
    $handler->display->display_options['fields']['timestamp']['custom_date_format'] = 'j M, Y';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
    $handler->display->display_options['fields']['name_1']['table'] = 'users';
    $handler->display->display_options['fields']['name_1']['field'] = 'name';
    $handler->display->display_options['fields']['name_1']['relationship'] = 'uid';
    $handler->display->display_options['fields']['name_1']['label'] = 'Reported by';
    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['relationship'] = 'nid';
    $handler->display->display_options['fields']['title']['label'] = 'Parent Node';
    /* Field: Comment: Author */
    $handler->display->display_options['fields']['name_2']['id'] = 'name_2';
    $handler->display->display_options['fields']['name_2']['table'] = 'comment';
    $handler->display->display_options['fields']['name_2']['field'] = 'name';
    /* Field: Comment: Comment */
    $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
    $handler->display->display_options['fields']['entity_id']['table'] = 'field_data_comment_body';
    $handler->display->display_options['fields']['entity_id']['field'] = 'comment_body';
    /* Field: Comment: Delete link */
    $handler->display->display_options['fields']['delete_comment']['id'] = 'delete_comment';
    $handler->display->display_options['fields']['delete_comment']['table'] = 'comment';
    $handler->display->display_options['fields']['delete_comment']['field'] = 'delete_comment';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops']['id'] = 'ops';
    $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
    $handler->display->display_options['fields']['ops']['field'] = 'ops';
    $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops_1']['id'] = 'ops_1';
    $handler->display->display_options['fields']['ops_1']['table'] = 'flag_content';
    $handler->display->display_options['fields']['ops_1']['field'] = 'ops';
    $handler->display->display_options['fields']['ops_1']['relationship'] = 'flag_content_rel_1';
    $handler->display->display_options['fields']['ops_1']['label'] = 'Whitelist';
    $handler->display->display_options['fields']['ops_1']['element_label_colon'] = FALSE;
    /* Sort criterion: Flags: Flagged time */
    $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
    $handler->display->display_options['sorts']['timestamp']['table'] = 'flag_content';
    $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
    $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_content_rel';
    /* Filter criterion: Comment: Approved */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'comment';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = '1';

    /* Display: Page: Comment Abuse */
    $handler = $view->new_display('page', 'Page: Comment Abuse', 'page_abuse_comment');
    $handler->display->display_options['defaults']['relationships'] = FALSE;
    /* Relationship: Flags: abuse_comment */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'comment';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'abuse_comment';
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Comment: Content */
    $handler->display->display_options['relationships']['nid']['id'] = 'nid';
    $handler->display->display_options['relationships']['nid']['table'] = 'comment';
    $handler->display->display_options['relationships']['nid']['field'] = 'nid';
    $handler->display->display_options['relationships']['nid']['required'] = TRUE;
    /* Relationship: Flags: User */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'flag_content';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['uid']['required'] = TRUE;
    /* Relationship: Flags: abuse_whitelist_comment */
    $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
    $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'comment';
    $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'whitelist flag';
    $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
    $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'abuse_whitelist_comment';
    $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
    $handler->display->display_options['path'] = 'admin/content/comment/flag-abuse/abuse_comment';
    $handler->display->display_options['menu']['type'] = 'default tab';
    $handler->display->display_options['menu']['title'] = 'Comment Abuse';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['tab_options']['type'] = 'tab';
    $handler->display->display_options['tab_options']['title'] = 'Reported comments';
    $handler->display->display_options['tab_options']['weight'] = '10';
    $views[$view->name] = $view;
  }
}

function _flag_abuse_views_default_abuse_node(&$views) {
  if(FLAG_API_VERSION == 3) {
    $view = new view();
    $view->name = 'all_flag_abuse_node';
    $view->description = 'View for flag: Node Abuse';
    $view->tag = 'flag.abuse';
    $view->base_table = 'node';
    $view->human_name = '';
    $view->core = 0;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'Node abuse';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'role';
    $handler->display->display_options['access']['role'] = array(
      2 => '2',
    );
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['style_plugin'] = 'table';
    $handler->display->display_options['style_options']['grouping'] = 'type';
    $handler->display->display_options['style_options']['columns'] = array(
      'name_1' => 'name_1',
      'type' => 'type',
      'title' => 'title',
      'name' => 'name',
      'body' => 'body',
      'edit_node' => 'edit_node',
      'delete_node' => 'edit_node',
      'ops' => 'edit_node',
    );
    $handler->display->display_options['style_options']['default'] = '-1';
    $handler->display->display_options['style_options']['info'] = array(
      'name_1' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'type' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'body' => array(
        'separator' => '',
      ),
      'edit_node' => array(
        'separator' => ' | ',
      ),
      'delete_node' => array(
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
    );
    $handler->display->display_options['style_options']['override'] = FALSE;
    /* No results behavior: Global: Text area */
    $handler->display->display_options['empty']['text']['id'] = 'area';
    $handler->display->display_options['empty']['text']['table'] = 'views';
    $handler->display->display_options['empty']['text']['field'] = 'area';
    $handler->display->display_options['empty']['text']['content'] = 'No content has been reported.';
    $handler->display->display_options['empty']['text']['format'] = '1';
    /* Relationship: Flags: abuse_node */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'abuse_node';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'abuse_node';
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Flags: User */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'flagging';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['uid']['label'] = 'user';
    /* Relationship: Content: Author */
    $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
    $handler->display->display_options['relationships']['uid_1']['table'] = 'node';
    $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
    /* Relationship: Flags: abuse_whitelist_node */
    $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
    $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'node';
    $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'whitelist flag';
    $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
    $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'abuse_whitelist_node';
    $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
    $handler->display->display_options['fields']['name_1']['table'] = 'users';
    $handler->display->display_options['fields']['name_1']['field'] = 'name';
    $handler->display->display_options['fields']['name_1']['relationship'] = 'uid';
    $handler->display->display_options['fields']['name_1']['label'] = 'Report by';
    /* Field: Content: Type */
    $handler->display->display_options['fields']['type']['id'] = 'type';
    $handler->display->display_options['fields']['type']['table'] = 'node';
    $handler->display->display_options['fields']['type']['field'] = 'type';
    $handler->display->display_options['fields']['type']['exclude'] = TRUE;
    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['relationship'] = 'uid_1';
    $handler->display->display_options['fields']['name']['label'] = 'Author';
    /* Field: Content: Body */
    $handler->display->display_options['fields']['body']['id'] = 'body';
    $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
    $handler->display->display_options['fields']['body']['field'] = 'body';
    /* Field: Content: Edit link */
    $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
    $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
    $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
    $handler->display->display_options['fields']['edit_node']['label'] = 'Actions';
    $handler->display->display_options['fields']['edit_node']['text'] = 'edit';
    /* Field: Content: Delete link */
    $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
    $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
    $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
    $handler->display->display_options['fields']['delete_node']['label'] = '';
    $handler->display->display_options['fields']['delete_node']['text'] = 'remove';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops']['id'] = 'ops';
    $handler->display->display_options['fields']['ops']['table'] = 'flagging';
    $handler->display->display_options['fields']['ops']['field'] = 'ops';
    $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['ops']['label'] = '';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops_1']['id'] = 'ops_1';
    $handler->display->display_options['fields']['ops_1']['table'] = 'flagging';
    $handler->display->display_options['fields']['ops_1']['field'] = 'ops';
    $handler->display->display_options['fields']['ops_1']['relationship'] = 'flag_content_rel_1';
    $handler->display->display_options['fields']['ops_1']['label'] = 'Whitelist';
    /* Filter criterion: Content: Published */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'node';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = 1;
    $handler->display->display_options['filters']['status']['group'] = '0';
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

    /* Display: Page */
    $handler = $view->new_display('page', 'Page', 'page_1');
    $handler->display->display_options['path'] = 'admin/content/flags/abuse_node';
    $handler->display->display_options['menu']['type'] = 'default tab';
    $handler->display->display_options['menu']['title'] = 'Reported node abuse';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['tab_options']['type'] = 'normal';
    $handler->display->display_options['tab_options']['title'] = 'Reported abuse';
    $handler->display->display_options['tab_options']['weight'] = '';
    $views[$view->name] = $view;
  }elseif(FLAG_API_VERSION == 2) {
    $view = new view();
    $view->name = 'all_flag_abuse_node';
    $view->description = 'View for flag: Node Abuse';
    $view->tag = 'flag.abuse';
    $view->base_table = 'node';
    $view->human_name = '';
    $view->core = 0;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'Node abuse';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'role';
    $handler->display->display_options['access']['role'] = array(
      2 => '2',
    );
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['style_plugin'] = 'table';
    $handler->display->display_options['style_options']['grouping'] = 'type';
    $handler->display->display_options['style_options']['columns'] = array(
      'name_1' => 'name_1',
      'type' => 'type',
      'title' => 'title',
      'name' => 'name',
      'body' => 'body',
      'edit_node' => 'edit_node',
      'delete_node' => 'edit_node',
      'ops' => 'edit_node',
    );
    $handler->display->display_options['style_options']['default'] = '-1';
    $handler->display->display_options['style_options']['info'] = array(
      'name_1' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'type' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'body' => array(
        'separator' => '',
      ),
      'edit_node' => array(
        'separator' => ' | ',
      ),
      'delete_node' => array(
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
    );
    $handler->display->display_options['style_options']['override'] = FALSE;
    /* No results behavior: Global: Text area */
    $handler->display->display_options['empty']['text']['id'] = 'area';
    $handler->display->display_options['empty']['text']['table'] = 'views';
    $handler->display->display_options['empty']['text']['field'] = 'area';
    $handler->display->display_options['empty']['text']['content'] = 'No content has been reported.';
    $handler->display->display_options['empty']['text']['format'] = '1';
    /* Relationship: Flags: abuse_node */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'abuse_node';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'abuse_node';
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Flags: User */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'flag_content';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['uid']['label'] = 'user';
    /* Relationship: Content: Author */
    $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
    $handler->display->display_options['relationships']['uid_1']['table'] = 'node';
    $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
    /* Relationship: Flags: abuse_whitelist_node */
    $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
    $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'node';
    $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'whitelist flag';
    $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
    $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'abuse_whitelist_node';
    $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
    $handler->display->display_options['fields']['name_1']['table'] = 'users';
    $handler->display->display_options['fields']['name_1']['field'] = 'name';
    $handler->display->display_options['fields']['name_1']['relationship'] = 'uid';
    $handler->display->display_options['fields']['name_1']['label'] = 'Report by';
    /* Field: Content: Type */
    $handler->display->display_options['fields']['type']['id'] = 'type';
    $handler->display->display_options['fields']['type']['table'] = 'node';
    $handler->display->display_options['fields']['type']['field'] = 'type';
    $handler->display->display_options['fields']['type']['exclude'] = TRUE;
    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['relationship'] = 'uid_1';
    $handler->display->display_options['fields']['name']['label'] = 'Author';
    /* Field: Content: Body */
    $handler->display->display_options['fields']['body']['id'] = 'body';
    $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
    $handler->display->display_options['fields']['body']['field'] = 'body';
    /* Field: Content: Edit link */
    $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
    $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
    $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
    $handler->display->display_options['fields']['edit_node']['label'] = 'Actions';
    $handler->display->display_options['fields']['edit_node']['text'] = 'edit';
    /* Field: Content: Delete link */
    $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
    $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
    $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
    $handler->display->display_options['fields']['delete_node']['label'] = '';
    $handler->display->display_options['fields']['delete_node']['text'] = 'remove';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops']['id'] = 'ops';
    $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
    $handler->display->display_options['fields']['ops']['field'] = 'ops';
    $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['ops']['label'] = '';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops_1']['id'] = 'ops_1';
    $handler->display->display_options['fields']['ops_1']['table'] = 'flag_content';
    $handler->display->display_options['fields']['ops_1']['field'] = 'ops';
    $handler->display->display_options['fields']['ops_1']['relationship'] = 'flag_content_rel_1';
    $handler->display->display_options['fields']['ops_1']['label'] = 'Whitelist';
    $handler->display->display_options['fields']['ops_1']['element_label_colon'] = FALSE;
    /* Filter criterion: Content: Published */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'node';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = 1;
    $handler->display->display_options['filters']['status']['group'] = '0';
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

    /* Display: Page */
    $handler = $view->new_display('page', 'Page', 'page_1');
    $handler->display->display_options['path'] = 'admin/content/flags/abuse_node';
    $handler->display->display_options['menu']['type'] = 'default tab';
    $handler->display->display_options['menu']['title'] = 'Reported node abuse';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['tab_options']['type'] = 'normal';
    $handler->display->display_options['tab_options']['title'] = 'Reported abuse';
    $handler->display->display_options['tab_options']['weight'] = '';
    $views['all_flag_abuse_node'] = $view;
  }
}

function _flag_abuse_views_default_abuse_user(&$views) {
  if(FLAG_API_VERSION == 3) {
    $view = new view();
    $view->name = 'all_flag_abuse_user';
    $view->description = 'View for flag: User Abuse';
    $view->tag = 'flag.abuse';
    $view->base_table = 'users';
    $view->human_name = '';
    $view->core = 0;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'User abuse';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'role';
    $handler->display->display_options['access']['role'] = array(
      2 => '2',
    );
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['style_plugin'] = 'table';
    $handler->display->display_options['style_options']['grouping'] = '';
    $handler->display->display_options['style_options']['columns'] = array(
      'name' => 'name',
      'ops' => 'ops',
    );
    $handler->display->display_options['style_options']['default'] = 'name';
    $handler->display->display_options['style_options']['info'] = array(
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
    );
    /* No results behavior: Global: Text area */
    $handler->display->display_options['empty']['text']['id'] = 'area';
    $handler->display->display_options['empty']['text']['table'] = 'views';
    $handler->display->display_options['empty']['text']['field'] = 'area';
    $handler->display->display_options['empty']['text']['content'] = 'No users have been reported.';
    $handler->display->display_options['empty']['text']['format'] = '1';
    /* Relationship: Flags: abuse_user */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'abuse_user';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'abuse_user';
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Flags: abuse_whitelist_user */
    $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
    $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'users';
    $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'whitelist flag';
    $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
    $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'abuse_whitelist_user';
    $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops']['id'] = 'ops';
    $handler->display->display_options['fields']['ops']['table'] = 'flagging';
    $handler->display->display_options['fields']['ops']['field'] = 'ops';
    $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['ops']['label'] = 'Ops';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops_1']['id'] = 'ops_1';
    $handler->display->display_options['fields']['ops_1']['table'] = 'flagging';
    $handler->display->display_options['fields']['ops_1']['field'] = 'ops';
    $handler->display->display_options['fields']['ops_1']['relationship'] = 'flag_content_rel_1';
    $handler->display->display_options['fields']['ops_1']['label'] = 'Whitelist';

    /* Display: Page */
    $handler = $view->new_display('page', 'Page', 'page_1');
    $handler->display->display_options['path'] = 'admin/content/flags/abuse_user';
    $handler->display->display_options['menu']['type'] = 'tab';
    $handler->display->display_options['menu']['title'] = 'Reported user abuse';
    $handler->display->display_options['menu']['weight'] = '0';
    $views[$view->name] = $view;
  }elseif(FLAG_API_VERSION == 2) {
    $view = new view();
    $view->name = 'all_flag_abuse_user';
    $view->description = 'View for flag: User Abuse';
    $view->tag = 'flag.abuse';
    $view->base_table = 'users';
    $view->human_name = '';
    $view->core = 0;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'User abuse';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'role';
    $handler->display->display_options['access']['role'] = array(
      2 => '2',
    );
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['style_plugin'] = 'table';
    $handler->display->display_options['style_options']['grouping'] = '';
    $handler->display->display_options['style_options']['columns'] = array(
      'name' => 'name',
      'ops' => 'ops',
    );
    $handler->display->display_options['style_options']['default'] = 'name';
    $handler->display->display_options['style_options']['info'] = array(
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
    );
    /* No results behavior: Global: Text area */
    $handler->display->display_options['empty']['text']['id'] = 'area';
    $handler->display->display_options['empty']['text']['table'] = 'views';
    $handler->display->display_options['empty']['text']['field'] = 'area';
    $handler->display->display_options['empty']['text']['content'] = 'No users have been reported.';
    $handler->display->display_options['empty']['text']['format'] = '1';
    /* Relationship: Flags: abuse_user */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'abuse_user';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'abuse_user';
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Flags: abuse_whitelist_user */
    $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
    $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'users';
    $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'whitelist flag';
    $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
    $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'abuse_whitelist_user';
    $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops']['id'] = 'ops';
    $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
    $handler->display->display_options['fields']['ops']['field'] = 'ops';
    $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['fields']['ops']['label'] = 'Ops';
    /* Field: Flags: Flag link */
    $handler->display->display_options['fields']['ops_1']['id'] = 'ops_1';
    $handler->display->display_options['fields']['ops_1']['table'] = 'flag_content';
    $handler->display->display_options['fields']['ops_1']['field'] = 'ops';
    $handler->display->display_options['fields']['ops_1']['relationship'] = 'flag_content_rel_1';
    $handler->display->display_options['fields']['ops_1']['label'] = 'Whitelist';
    $handler->display->display_options['fields']['ops_1']['element_label_colon'] = FALSE;

    /* Display: Page */
    $handler = $view->new_display('page', 'Page', 'page_1');
    $handler->display->display_options['path'] = 'admin/content/flags/abuse_user';
    $handler->display->display_options['menu']['type'] = 'tab';
    $handler->display->display_options['menu']['title'] = 'Reported user abuse';
    $handler->display->display_options['menu']['weight'] = '0';
    $views[$view->name] = $view;
  }
}
