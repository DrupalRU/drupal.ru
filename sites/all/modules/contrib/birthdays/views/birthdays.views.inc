<?php

/**
 * @file
 * Declare Views integration for the Birthdays module.
 */

/**
 * Implements hook_field_views_data_alter().
 */
function birthdays_field_views_data_alter(&$data, &$field, &$module) {
  if ($field['type'] == 'birthdays_date') {
    $name = $field['field_name'];

    // Field handler.
    $data['field_data_' . $name][$name]['field']['handler'] = 'birthdays_field_views_handler_field';

    // Filter handler.
    $data['field_data_' . $name][$name]['filter'] = array(
      'field' => $name,
      'table' => 'field_data_' . $name,
      'handler' => 'birthdays_field_views_handler_filter',
      'additional fields' => array(
        $name . '_day',
        $name . '_month',
        $name . '_year',
      ),
      'field_name' => $name,
      'allow empty' => TRUE,
    );

    // Sort handler.
    $data['field_data_' . $name][$name]['sort'] = array(
      'field' => $name,
      'table' => 'field_data_' . $name,
      'handler' => 'birthdays_field_views_handler_sort',
      'additional fields' => array(
        $name . '_day',
        $name . '_month',
      ),
    );
  }
}
