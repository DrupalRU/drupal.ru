<?php
$field = array(
  'translatable' => '0',
  'settings' => array(
    'granularity' => array(
      'month' => 'month',
      'day' => 'day',
      'year' => 'year',
      'hour' => 0,
      'minute' => 0,
      'second' => 0,
    ),
    'tz_handling' => 'none',
    'timezone_db' => '',
    'cache_enabled' => 0,
    'cache_count' => '4',
    'todate' => 'optional',
  ),
  'field_name' => 'field_event_date',
  'type' => 'datetime',
  'active' => '1',
  'locked' => '0',
  'cardinality' => '1',
);
field_create_field($field);

$instance = array(
  'label' => $t('Date'),
  'widget' => array(
    'weight' => '4',
    'type' => 'date_popup',
    'active' => 1,
    'settings' => array(
      'input_format' => 'site-wide',
      'input_format_custom' => '',
      'year_range' => '-10:+10',
      'increment' => '15',
      'label_position' => 'above',
      'text_parts' => array(),
      'no_fieldset' => 0,
    ),
  ),
  'settings' => array(
    'default_value' => 'now',
    'default_value_code' => '',
    'default_value2' => 'blank',
    'default_value_code2' => '',
    'user_register_form' => FALSE,
  ),
  'display' => array(
    'default' => array(
      'label' => 'hidden',
      'type' => 'date_default',
      'weight' => '1',
      'settings' => array(
        'format_type' => 'short',
        'fromto' => 'both',
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'show_remaining_days' => 0,
      ),
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
  'required' => 1,
  'description' => '',
  'field_name' => 'field_event_date',
  'entity_type' => 'node',
  'bundle' => 'event',
);
field_create_instance($instance);