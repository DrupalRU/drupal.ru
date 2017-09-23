<?php
/**
 * @file
 * gmap_test.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function gmap_test_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'gmap_test_places';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'GMap-test-Places';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Location';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'gmap';
  $handler->display->display_options['style_options']['macro'] = '[gmap ]';
  $handler->display->display_options['style_options']['latfield'] = 'title';
  $handler->display->display_options['style_options']['lonfield'] = 'title';
  $handler->display->display_options['style_options']['markerfield'] = 'title';
  $handler->display->display_options['style_options']['enablermt'] = 0;
  $handler->display->display_options['style_options']['rmtfield'] = 'title';
  $handler->display->display_options['style_options']['tooltipfield'] = 'title';
  $handler->display->display_options['style_options']['bubbletextfield'] = 'title';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_mylocation_target_id']['id'] = 'field_mylocation_target_id';
  $handler->display->display_options['relationships']['field_mylocation_target_id']['table'] = 'field_data_field_mylocation';
  $handler->display->display_options['relationships']['field_mylocation_target_id']['field'] = 'field_mylocation_target_id';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Location: Latitude */
  $handler->display->display_options['fields']['latitude']['id'] = 'latitude';
  $handler->display->display_options['fields']['latitude']['table'] = 'location';
  $handler->display->display_options['fields']['latitude']['field'] = 'latitude';
  $handler->display->display_options['fields']['latitude']['style'] = 'dd';
  /* Field: Location: Longitude */
  $handler->display->display_options['fields']['longitude']['id'] = 'longitude';
  $handler->display->display_options['fields']['longitude']['table'] = 'location';
  $handler->display->display_options['fields']['longitude']['field'] = 'longitude';
  $handler->display->display_options['fields']['longitude']['style'] = 'dd';
  /* Field: Content: mygeofield */
  $handler->display->display_options['fields']['field_mygeofield']['id'] = 'field_mygeofield';
  $handler->display->display_options['fields']['field_mygeofield']['table'] = 'field_data_field_mygeofield';
  $handler->display->display_options['fields']['field_mygeofield']['field'] = 'field_mygeofield';
  $handler->display->display_options['fields']['field_mygeofield']['click_sort_column'] = 'wkt';
  $handler->display->display_options['fields']['field_mygeofield']['type'] = 'geofield_latlon';
  $handler->display->display_options['fields']['field_mygeofield']['settings'] = array(
    'data' => 'full',
    'format' => 'decimal_degrees',
    'labels' => 0,
  );
  /* Field: Content: mylatitude */
  $handler->display->display_options['fields']['field_mylatitude']['id'] = 'field_mylatitude';
  $handler->display->display_options['fields']['field_mylatitude']['table'] = 'field_data_field_mylatitude';
  $handler->display->display_options['fields']['field_mylatitude']['field'] = 'field_mylatitude';
  /* Field: Content: mylongitude */
  $handler->display->display_options['fields']['field_mylongitude']['id'] = 'field_mylongitude';
  $handler->display->display_options['fields']['field_mylongitude']['table'] = 'field_data_field_mylongitude';
  $handler->display->display_options['fields']['field_mylongitude']['field'] = 'field_mylongitude';
  /* Field: Content: MyLocation */
  $handler->display->display_options['fields']['field_mylocation']['id'] = 'field_mylocation';
  $handler->display->display_options['fields']['field_mylocation']['table'] = 'field_data_field_mylocation';
  $handler->display->display_options['fields']['field_mylocation']['field'] = 'field_mylocation';
  $handler->display->display_options['fields']['field_mylocation']['relationship'] = 'field_mylocation_target_id';
  $handler->display->display_options['fields']['field_mylocation']['label'] = 'person\'s Location';
  $handler->display->display_options['fields']['field_mylocation']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: MyAddress */
  $handler->display->display_options['fields']['field_myaddress']['id'] = 'field_myaddress';
  $handler->display->display_options['fields']['field_myaddress']['table'] = 'field_data_field_myaddress';
  $handler->display->display_options['fields']['field_myaddress']['field'] = 'field_myaddress';
  $handler->display->display_options['fields']['field_myaddress']['relationship'] = 'field_mylocation_target_id';
  $handler->display->display_options['fields']['field_myaddress']['label'] = 'person\'s Address';
  $handler->display->display_options['fields']['field_myaddress']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['field_myaddress']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  $handler->display->display_options['fields']['field_myaddress']['delta_offset'] = '0';
  /* Field: Content: mygeofield */
  $handler->display->display_options['fields']['field_mygeofield_1']['id'] = 'field_mygeofield_1';
  $handler->display->display_options['fields']['field_mygeofield_1']['table'] = 'field_data_field_mygeofield';
  $handler->display->display_options['fields']['field_mygeofield_1']['field'] = 'field_mygeofield';
  $handler->display->display_options['fields']['field_mygeofield_1']['relationship'] = 'field_mylocation_target_id';
  $handler->display->display_options['fields']['field_mygeofield_1']['label'] = 'person\'s geofield';
  $handler->display->display_options['fields']['field_mygeofield_1']['click_sort_column'] = 'wkt';
  $handler->display->display_options['fields']['field_mygeofield_1']['type'] = 'geofield_latlon';
  $handler->display->display_options['fields']['field_mygeofield_1']['settings'] = array(
    'data' => 'full',
    'format' => 'decimal_degrees',
    'labels' => 0,
  );
  /* Field: Content: mylatitude */
  $handler->display->display_options['fields']['field_mylatitude_1']['id'] = 'field_mylatitude_1';
  $handler->display->display_options['fields']['field_mylatitude_1']['table'] = 'field_data_field_mylatitude';
  $handler->display->display_options['fields']['field_mylatitude_1']['field'] = 'field_mylatitude';
  $handler->display->display_options['fields']['field_mylatitude_1']['relationship'] = 'field_mylocation_target_id';
  $handler->display->display_options['fields']['field_mylatitude_1']['label'] = 'person\'s latitude';
  /* Field: Content: mylongitude */
  $handler->display->display_options['fields']['field_mylongitude_1']['id'] = 'field_mylongitude_1';
  $handler->display->display_options['fields']['field_mylongitude_1']['table'] = 'field_data_field_mylongitude';
  $handler->display->display_options['fields']['field_mylongitude_1']['field'] = 'field_mylongitude';
  $handler->display->display_options['fields']['field_mylongitude_1']['relationship'] = 'field_mylocation_target_id';
  $handler->display->display_options['fields']['field_mylongitude_1']['label'] = 'person\'s longitude';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'gmap-test-places';

  /* Display: Table */
  $handler = $view->new_display('block', 'Table', 'block_table');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'GMap table overview';
  $handler->display->display_options['display_description'] = 'A simple table to show all data';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'type' => 'type',
    'field_mygeofield' => 'field_mygeofield',
    'field_mylatitude' => 'field_mylatitude',
    'field_mylongitude' => 'field_mylongitude',
    'latitude' => 'latitude',
    'longitude' => 'longitude',
  );
  $handler->display->display_options['style_options']['default'] = 'type';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_mygeofield' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_mylatitude' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_mylongitude' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'latitude' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'longitude' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;

  /* Display: Geofield */
  $handler = $view->new_display('block', 'Geofield', 'block_geofield');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Geofield';
  $handler->display->display_options['display_description'] = 'Uses Geofield as origin';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'gmap';
  $handler->display->display_options['style_options']['macro'] = '[gmap | behavior=+views_autocenter +autozoom | width=100% | height=300px | type=Hybrid]';
  $handler->display->display_options['style_options']['datasource'] = 'geofield';
  $handler->display->display_options['style_options']['latfield'] = 'title';
  $handler->display->display_options['style_options']['lonfield'] = 'title';
  $handler->display->display_options['style_options']['markerfield'] = 'title';
  $handler->display->display_options['style_options']['enablermt'] = 0;
  $handler->display->display_options['style_options']['rmtfield'] = 'title';
  $handler->display->display_options['style_options']['tooltipfield'] = 'title';
  $handler->display->display_options['style_options']['bubbletextfield'] = 'title';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;

  /* Display: Location */
  $handler = $view->new_display('block', 'Location', 'block_location');
  $handler->display->display_options['display_description'] = 'Uses Location field as origin';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'gmap';
  $handler->display->display_options['style_options']['macro'] = '[gmap | behavior=+views_autocenter +autozoom | width=100% | height=300px | type=Hybrid]';
  $handler->display->display_options['style_options']['latfield'] = 'title';
  $handler->display->display_options['style_options']['lonfield'] = 'title';
  $handler->display->display_options['style_options']['markerfield'] = 'title';
  $handler->display->display_options['style_options']['enablermt'] = 0;
  $handler->display->display_options['style_options']['rmtfield'] = 'title';
  $handler->display->display_options['style_options']['tooltipfield'] = 'title';
  $handler->display->display_options['style_options']['bubbletextfield'] = 'title';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;

  /* Display: Field */
  $handler = $view->new_display('block', 'Field', 'block_field');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Fields';
  $handler->display->display_options['display_description'] = 'Uses a Field field as origin.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'gmap';
  $handler->display->display_options['style_options']['macro'] = '[gmap | behavior=+views_autocenter +autozoom | width=100% | height=300px | type=Hybrid]';
  $handler->display->display_options['style_options']['datasource'] = 'fields';
  $handler->display->display_options['style_options']['latfield'] = 'field_mylatitude';
  $handler->display->display_options['style_options']['lonfield'] = 'field_mylongitude';
  $handler->display->display_options['style_options']['markerfield'] = 'title';
  $handler->display->display_options['style_options']['enablermt'] = 0;
  $handler->display->display_options['style_options']['rmtfield'] = 'title';
  $handler->display->display_options['style_options']['center_on_nodearg'] = 1;
  $handler->display->display_options['style_options']['center_on_nodearg_arg'] = 'nid';
  $handler->display->display_options['style_options']['highlight_nodearg'] = 1;
  $handler->display->display_options['style_options']['highlight_nodearg_arg'] = 'nid';
  $handler->display->display_options['style_options']['autoclick_on_nodearg_arg'] = 'nid';
  $handler->display->display_options['style_options']['tooltipfield'] = 'title';
  $handler->display->display_options['style_options']['bubbletextfield'] = 'title';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;

  /* Display: Person's Location */
  $handler = $view->new_display('block', 'Person\'s Location', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Person\'s Location';
  $handler->display->display_options['display_description'] = 'Uses Location as origin of referenced Node';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'gmap';
  $handler->display->display_options['style_options']['macro'] = '[gmap | behavior=+views_autocenter +autozoom | width=100% | height=300px | type=Hybrid]';
  $handler->display->display_options['style_options']['markertype'] = 'letters';
  $handler->display->display_options['style_options']['latfield'] = 'title';
  $handler->display->display_options['style_options']['lonfield'] = 'title';
  $handler->display->display_options['style_options']['markerfield'] = 'title';
  $handler->display->display_options['style_options']['enablermt'] = 0;
  $handler->display->display_options['style_options']['rmtfield'] = 'title';
  $handler->display->display_options['style_options']['tooltipfield'] = 'title';
  $handler->display->display_options['style_options']['bubbletextfield'] = 'title';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;

  /* Display: person's Field */
  $handler = $view->new_display('block', 'person\'s Field', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Person\'s Fields';
  $handler->display->display_options['display_description'] = 'Uses a Field field as origin, of a referenced node.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'gmap';
  $handler->display->display_options['style_options']['macro'] = '[gmap | behavior=+views_autocenter +autozoom | width=100% | height=300px | type=Hybrid]';
  $handler->display->display_options['style_options']['datasource'] = 'fields';
  $handler->display->display_options['style_options']['markertype'] = 'letters';
  $handler->display->display_options['style_options']['latfield'] = 'field_mylatitude_1';
  $handler->display->display_options['style_options']['lonfield'] = 'field_mylongitude_1';
  $handler->display->display_options['style_options']['markerfield'] = 'title';
  $handler->display->display_options['style_options']['enablermt'] = 0;
  $handler->display->display_options['style_options']['rmtfield'] = 'title';
  $handler->display->display_options['style_options']['tooltipfield'] = 'title';
  $handler->display->display_options['style_options']['bubbletextfield'] = 'title';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;

  /* Display: Person's Geofield */
  $handler = $view->new_display('block', 'Person\'s Geofield', 'block_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Person\'s Geofield';
  $handler->display->display_options['display_description'] = 'Uses Geofield as origin, of a referenced  node';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'gmap';
  $handler->display->display_options['style_options']['macro'] = '[gmap | behavior=+views_autocenter +autozoom | width=100% | height=300px | type=Hybrid]';
  $handler->display->display_options['style_options']['datasource'] = 'geofield';
  $handler->display->display_options['style_options']['markertype'] = 'letters';
  $handler->display->display_options['style_options']['latfield'] = 'title';
  $handler->display->display_options['style_options']['lonfield'] = 'title';
  $handler->display->display_options['style_options']['markerfield'] = 'title';
  $handler->display->display_options['style_options']['enablermt'] = 0;
  $handler->display->display_options['style_options']['rmtfield'] = 'title';
  $handler->display->display_options['style_options']['center_on_nodearg'] = 1;
  $handler->display->display_options['style_options']['center_on_nodearg_arg'] = 'nid';
  $handler->display->display_options['style_options']['highlight_nodearg'] = 1;
  $handler->display->display_options['style_options']['highlight_nodearg_arg'] = 'nid';
  $handler->display->display_options['style_options']['autoclick_on_nodearg_arg'] = 'nid';
  $handler->display->display_options['style_options']['tooltipfield'] = 'title';
  $handler->display->display_options['style_options']['bubbletextfield'] = 'title';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $export['gmap_test_places'] = $view;

  return $export;
}
