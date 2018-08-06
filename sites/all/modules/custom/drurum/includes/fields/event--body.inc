<?php
$instance = array(
  'label' => $t('Description'),
  'widget' => array(
    'type' => 'text_textarea_with_summary',
    'settings' => array(
      'rows' => 20,
      'summary_rows' => 5,
    ),
    'weight' => '46',
  ),
  'display' => array(
    'default' => array(
      'label' => 'hidden',
      'type' => 'text_default',
      'settings' => array(),
      'weight' => 4,
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
  'settings' => array(
    'text_processing' => 1,
    'display_summary' => 0,
    'user_register_form' => FALSE,
  ),
  'required' => FALSE,
  'description' => '',
  'field_name' => 'body',
  'entity_type' => 'node',
  'bundle' => 'event',
  'default_value' => NULL,
);
field_create_instance($instance);
