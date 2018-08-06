<?php
/**
 * @file
 */

/**
 * Implements hook_preprocess_views_view_field
 */
function druru_preprocess_views_view_field(&$vars) {
  $view = $vars['view'];
  $field = $vars['field'];
  $row = $vars['row'];

  if ($view->name == 'events_upcoming_block') {
    if ($field->field == 'field_event_date') {
      $event_start = format_date(strtotime($row->field_field_event_date[0]['raw']['value']), 'short');
      $event_end   = format_date(strtotime($row->field_field_event_date[0]['raw']['value2']), 'short');
      $vars['output'] = $event_start . ($event_start != $event_end ? ' &mdash; ' . $event_end : '');
    }
  }
}
