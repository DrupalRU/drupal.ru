<?php
$field = array(
  'translatable' => '0',
  'settings' => array(
    'uri_scheme' => 'public',
    'default_image' => 0,
  ),
  'field_name' => 'field_event_image',
  'type' => 'image',
  'active' => '1',
  'locked' => '0',
  'cardinality' => '1',
);
field_create_field($field);

$instance = array(
  'label' => $t('Image'),
  'widget' => array(
    'weight' => '41',
    'type' => 'image_image',
    'active' => 1,
    'settings' => array(
      'progress_indicator' => 'throbber',
      'preview_image_style' => 'thumbnail',
    ),
  ),
  'settings' => array(
    'file_directory' => 'events_images',
    'file_extensions' => 'png gif jpg jpeg',
    'max_filesize' => '',
    'max_resolution' => '300x200',
    'min_resolution' => '300x200',
    'alt_field' => 0,
    'title_field' => 0,
    'default_image' => '54136',
    'user_register_form' => FALSE,
  ),
  'display' => array(
    'default' => array(
      'label' => 'hidden',
      'type' => 'image',
      'settings' => array(
        'image_style' => '',
        'image_link' => '',
      ),
      'weight' => 0,
    ),
    'teaser' => array(
      'type' => 'hidden',
      'label' => 'hidden',
      'settings' => array(),
      'weight' => 0,
    ),
    'frontpage' => array(
      'type' => 'hidden',
      'label' => 'hidden',
      'settings' => array(),
      'weight' => 0,
    ),
    'block' => array(
      'type' => 'hidden',
      'label' => 'hidden',
      'settings' => array(),
      'weight' => 0,
    ),
  ),
  'required' => 0,
  'description' => '',
  'field_name' => 'field_event_image',
  'entity_type' => 'node',
  'bundle' => 'event',
);
field_create_instance($instance);
