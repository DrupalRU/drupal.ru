<?php
$view = new view();
$view->name = 'events_past';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Events: Past';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Past events';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '100';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* No results behavior: Global: Unfiltered text */
$handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
$handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
$handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
$handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
$handler->display->display_options['empty']['area_text_custom']['content'] = 'There are no events in the near future';
/* Field: Content: Image */
$handler->display->display_options['fields']['field_event_image']['id'] = 'field_event_image';
$handler->display->display_options['fields']['field_event_image']['table'] = 'field_data_field_event_image';
$handler->display->display_options['fields']['field_event_image']['field'] = 'field_event_image';
$handler->display->display_options['fields']['field_event_image']['label'] = '';
$handler->display->display_options['fields']['field_event_image']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_event_image']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['field_event_image']['settings'] = array(
  'image_style' => 'event_teaser',
  'image_link' => 'content',
);
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Field: Content: Date */
$handler->display->display_options['fields']['field_event_date']['id'] = 'field_event_date';
$handler->display->display_options['fields']['field_event_date']['table'] = 'field_data_field_event_date';
$handler->display->display_options['fields']['field_event_date']['field'] = 'field_event_date';
$handler->display->display_options['fields']['field_event_date']['label'] = '';
$handler->display->display_options['fields']['field_event_date']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_event_date']['settings'] = array(
  'format_type' => 'short',
  'custom_date_format' => '',
  'fromto' => 'value',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_remaining_days' => 0,
);
/* Field: Content: Description */
$handler->display->display_options['fields']['body']['id'] = 'body';
$handler->display->display_options['fields']['body']['table'] = 'field_data_body';
$handler->display->display_options['fields']['body']['field'] = 'body';
$handler->display->display_options['fields']['body']['label'] = '';
$handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
/* Field: Content: Address */
$handler->display->display_options['fields']['field_event_address']['id'] = 'field_event_address';
$handler->display->display_options['fields']['field_event_address']['table'] = 'field_data_field_event_address';
$handler->display->display_options['fields']['field_event_address']['field'] = 'field_event_address';
$handler->display->display_options['fields']['field_event_address']['label'] = '';
$handler->display->display_options['fields']['field_event_address']['element_label_colon'] = FALSE;
/* Field: Content: Type */
$handler->display->display_options['fields']['field_event_type']['id'] = 'field_event_type';
$handler->display->display_options['fields']['field_event_type']['table'] = 'field_data_field_event_type';
$handler->display->display_options['fields']['field_event_type']['field'] = 'field_event_type';
$handler->display->display_options['fields']['field_event_type']['label'] = '';
$handler->display->display_options['fields']['field_event_type']['element_label_colon'] = FALSE;
/* Field: Content: Url */
$handler->display->display_options['fields']['field_event_link']['id'] = 'field_event_link';
$handler->display->display_options['fields']['field_event_link']['table'] = 'field_data_field_event_link';
$handler->display->display_options['fields']['field_event_link']['field'] = 'field_event_link';
$handler->display->display_options['fields']['field_event_link']['label'] = '';
$handler->display->display_options['fields']['field_event_link']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_event_link']['click_sort_column'] = 'url';
$handler->display->display_options['fields']['field_event_link']['type'] = 'link_url';
/* Sort criterion: Content: Date -  start date (field_event_date) */
$handler->display->display_options['sorts']['field_event_date_value']['id'] = 'field_event_date_value';
$handler->display->display_options['sorts']['field_event_date_value']['table'] = 'field_data_field_event_date';
$handler->display->display_options['sorts']['field_event_date_value']['field'] = 'field_event_date_value';
$handler->display->display_options['sorts']['field_event_date_value']['order'] = 'DESC';
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'event' => 'event',
);
/* Filter criterion: Content: Date -  start date (field_event_date) */
$handler->display->display_options['filters']['field_event_date_value']['id'] = 'field_event_date_value';
$handler->display->display_options['filters']['field_event_date_value']['table'] = 'field_data_field_event_date';
$handler->display->display_options['filters']['field_event_date_value']['field'] = 'field_event_date_value';
$handler->display->display_options['filters']['field_event_date_value']['operator'] = '<';
$handler->display->display_options['filters']['field_event_date_value']['default_date'] = 'now';

/* Display: Past events*/
$handler = $view->new_display('page', 'Past events', 'page');
$handler->display->display_options['path'] = 'events/past';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Past events';
$handler->display->display_options['menu']['weight'] = '1';
$handler->display->display_options['menu']['name'] = 'main-menu';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;
$translatables['events_past'] = array(
  t('Master'),
  t('Past events'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('There are no events in the near future'),
);
$view->save();
