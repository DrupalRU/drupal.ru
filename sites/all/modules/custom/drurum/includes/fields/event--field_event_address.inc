<?php
$field = array(
  'translatable' => '0',
  'settings' => array(
    'max_length' => '255',
  ),
  'field_name' => 'field_event_address',
  'type' => 'text',
  'active' => '1',
  'locked' => '0',
  'cardinality' => '1',
);
field_create_field($field);

$instance = array(
  'label' => $t('Address'),
  'widget' => array(
    'weight' => '44',
    'type' => 'text_textfield',
    'active' => 1,
    'settings' => array(
      'size' => '60',
    ),
  ),
  'settings' => array(
    'text_processing' => '0',
    'user_register_form' => FALSE,
  ),
  'display' => array(
    'default' => array(
      'label' => 'hidden',
      'type' => 'text_default',
      'settings' => array(),
      'weight' => 3,
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
  'required' => 1,
  'description' => '',
  'default_value' => NULL,
  'field_name' => 'field_event_address',
  'entity_type' => 'node',
  'bundle' => 'event',
);
field_create_instance($instance);
