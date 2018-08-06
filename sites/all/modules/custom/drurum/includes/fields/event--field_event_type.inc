<?php
$field = array(
  'translatable' => '0',
  'settings' => array(
    'allowed_values' => array(
      0 => array(
        'vocabulary' => 'event_types',
        'parent' => '0',
      ),
    ),
  ),
  'field_name' => 'field_event_type',
  'type' => 'taxonomy_term_reference',
  'active' => '1',
  'locked' => '0',
  'cardinality' => '1',
);
field_create_field($field);

$instance = array(
  'label' => $t('Type'),
  'widget' => array(
    'weight' => '45',
    'type' => 'options_select',
    'active' => 1,
    'settings' => array(),
  ),
  'settings' => array(
    'user_register_form' => FALSE,
  ),
  'display' => array(
    'default' => array(
      'label' => 'hidden',
      'type' => 'taxonomy_term_reference_link',
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
  'required' => 1,
  'description' => '',
  'default_value' => NULL,
  'field_name' => 'field_event_type',
  'entity_type' => 'node',
  'bundle' => 'event',
);
field_create_instance($instance);
