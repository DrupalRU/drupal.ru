<?php
$field = array(
  'translatable' => '0',
  'settings' => array(
    'attributes' => array(
      'target' => 'default',
      'class' => '',
      'rel' => '',
    ),
    'url' => 0,
    'title' => 'optional',
    'title_value' => '',
    'title_maxlength' => 128,
    'enable_tokens' => 1,
    'display' => array(
      'url_cutoff' => 80,
    ),
  ),
  'field_name' => 'field_event_link',
  'type' => 'link_field',
  'active' => '1',
  'locked' => '0',
  'cardinality' => '1',
);
field_create_field($field);

$instance = array(
  'label' => $t('URL'),
  'widget' => array(
    'weight' => '5',
    'type' => 'link_field',
    'active' => 0,
    'settings' => array(),
  ),
  'settings' => array(
    'absolute_url' => 1,
    'validate_url' => 1,
    'url' => 0,
    'title' => 'none',
    'title_value' => '',
    'title_allowed_values' => '',
    'title_label_use_field_label' => 0,
    'title_maxlength' => '128',
    'enable_tokens' => 1,
    'display' => array(
      'url_cutoff' => '80',
    ),
    'attributes' => array(
      'target' => 'default',
      'rel' => '',
      'configurable_class' => 0,
      'class' => '',
      'configurable_title' => 0,
      'title' => '',
    ),
    'rel_remove' => 'default',
    'user_register_form' => FALSE,
  ),
  'display' => array(
    'default' => array(
      'label' => 'hidden',
      'type' => 'link_url',
      'weight' => '2',
      'settings' => array(),
    ),
    'teaser' => array(
      'type' => 'hidden',
      'label' => 'above',
      'settings' => array(),
      'weight' => 0,
    ),
    'frontpage' => array(
      'type' => 'hidden',
      'label' => 'above',
      'settings' => array(),
      'weight' => 0,
    ),
    'block' => array(
      'type' => 'hidden',
      'label' => 'above',
      'settings' => array(),
      'weight' => 0,
    ),
  ),
  'required' => 0,
  'description' => '',
  'default_value' => NULL,
  'field_name' => 'field_event_link',
  'entity_type' => 'node',
  'bundle' => 'event',
);
field_create_instance($instance);
