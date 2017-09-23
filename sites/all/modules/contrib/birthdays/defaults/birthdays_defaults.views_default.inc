<?php

/**
 * @file
 * A default birthdays block and page as a view.
 */

/**
 * Implements hook_views_default_views().
 */
function birthdays_defaults_views_default_views() {
  $view = new view;
  $view->name = 'birthdays';
  $view->description = 'A page displaying all birthdays and a block with the next five.';
  $view->tag = 'default, birthdays';
  $view->base_table = 'users';
  $view->human_name = 'Birthdays';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Birthdays';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_text'] = 'All birthdays';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'birthdays' => 'birthdays',
  );
  $handler->display->display_options['style_options']['default'] = 'birthdays';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'birthdays' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: User: Birthday */
  $handler->display->display_options['fields']['birthdays_1']['id'] = 'birthdays_1';
  $handler->display->display_options['fields']['birthdays_1']['table'] = 'field_data_birthdays';
  $handler->display->display_options['fields']['birthdays_1']['field'] = 'birthdays';
  $handler->display->display_options['fields']['birthdays_1']['label'] = 'Age';
  $handler->display->display_options['fields']['birthdays_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['birthdays_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['birthdays_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['birthdays_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['birthdays_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['type'] = 'birthdays_age';
  $handler->display->display_options['fields']['birthdays_1']['field_api_classes'] = 0;
  /* Field: User: Birthday */
  $handler->display->display_options['fields']['birthdays']['id'] = 'birthdays';
  $handler->display->display_options['fields']['birthdays']['table'] = 'field_data_birthdays';
  $handler->display->display_options['fields']['birthdays']['field'] = 'birthdays';
  $handler->display->display_options['fields']['birthdays']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['external'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['birthdays']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['birthdays']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['html'] = 0;
  $handler->display->display_options['fields']['birthdays']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['birthdays']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['birthdays']['hide_empty'] = 0;
  $handler->display->display_options['fields']['birthdays']['empty_zero'] = 0;
  $handler->display->display_options['fields']['birthdays']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['birthdays']['field_api_classes'] = 0;
  $handler->display->display_options['fields']['birthdays']['click_sort_mode'] = 'now';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Birthday */
  $handler->display->display_options['filters']['birthdays']['id'] = 'birthdays';
  $handler->display->display_options['filters']['birthdays']['table'] = 'field_data_birthdays';
  $handler->display->display_options['filters']['birthdays']['field'] = 'birthdays';
  $handler->display->display_options['filters']['birthdays']['operator'] = 'not empty';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'birthdays' => 'birthdays',
    'birthdays_2' => 'birthdays_2',
    'birthdays_1' => 'birthdays_1',
  );
  $handler->display->display_options['style_options']['default'] = 'birthdays';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'birthdays' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'birthdays_2' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'birthdays_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: User: Birthday */
  $handler->display->display_options['fields']['birthdays']['id'] = 'birthdays';
  $handler->display->display_options['fields']['birthdays']['table'] = 'field_data_birthdays';
  $handler->display->display_options['fields']['birthdays']['field'] = 'birthdays';
  $handler->display->display_options['fields']['birthdays']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['external'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['birthdays']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['birthdays']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['birthdays']['alter']['html'] = 0;
  $handler->display->display_options['fields']['birthdays']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['birthdays']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['birthdays']['hide_empty'] = 0;
  $handler->display->display_options['fields']['birthdays']['empty_zero'] = 0;
  $handler->display->display_options['fields']['birthdays']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['birthdays']['field_api_classes'] = 0;
  $handler->display->display_options['fields']['birthdays']['click_sort_mode'] = 'now';
  /* Field: User: Birthday */
  $handler->display->display_options['fields']['birthdays_2']['id'] = 'birthdays_2';
  $handler->display->display_options['fields']['birthdays_2']['table'] = 'field_data_birthdays';
  $handler->display->display_options['fields']['birthdays_2']['field'] = 'birthdays';
  $handler->display->display_options['fields']['birthdays_2']['label'] = 'Current age';
  $handler->display->display_options['fields']['birthdays_2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['alter']['external'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['birthdays_2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['birthdays_2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['birthdays_2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['birthdays_2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['empty_zero'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['type'] = 'birthdays_age';
  $handler->display->display_options['fields']['birthdays_2']['field_api_classes'] = 0;
  $handler->display->display_options['fields']['birthdays_2']['click_sort_mode'] = 'all';
  /* Field: User: Birthday */
  $handler->display->display_options['fields']['birthdays_1']['id'] = 'birthdays_1';
  $handler->display->display_options['fields']['birthdays_1']['table'] = 'field_data_birthdays';
  $handler->display->display_options['fields']['birthdays_1']['field'] = 'birthdays';
  $handler->display->display_options['fields']['birthdays_1']['label'] = 'Starsign';
  $handler->display->display_options['fields']['birthdays_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['birthdays_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['birthdays_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['birthdays_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['birthdays_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['birthdays_1']['type'] = 'birthdays_starsign';
  $handler->display->display_options['fields']['birthdays_1']['field_api_classes'] = 0;
  $handler->display->display_options['path'] = 'birthdays';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Next birthdays';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'birthdays' => 'birthdays',
  );
  $handler->display->display_options['style_options']['default'] = 'birthdays';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'birthdays' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $translatables['birthdays'] = array(
    t('Master'),
    t('Birthdays'),
    t('All birthdays'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Name'),
    t('Age'),
    t('Birthday'),
    t('Page'),
    t('more'),
    t('Current age'),
    t('Starsign'),
    t('Block'),
    t('Next birthdays'),
  );

  return array('birthdays' => $view);
}
