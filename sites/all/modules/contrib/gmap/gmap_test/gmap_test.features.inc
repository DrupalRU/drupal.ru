<?php
/**
 * @file
 * gmap_test.features.inc
 */

/**
 * Implements hook_views_api().
 */
function gmap_test_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function gmap_test_node_info() {
  $items = array(
    'gmap_person' => array(
      'name' => t('GMapPerson'),
      'base' => 'node_content',
      'description' => t('Defines a \'person\', which has a refernce to a GMapLocation.
Thi si sused to thest the Gmap via referenced entities'),
      'has_title' => '1',
      'title_label' => t('Person name'),
      'help' => '',
    ),
    'gmapplace' => array(
      'name' => t('GmapPlace'),
      'base' => 'node_content',
      'description' => t('A node type that contains locations, in 3 different field types: 
- location
- geofield
- normal fields'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
