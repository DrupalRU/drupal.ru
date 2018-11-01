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

  if (in_array($view->name, ['tracker', 'tracker_new', 'tracker_my', 'featured'])) {
    if ($field->field == 'created') {
      $timestamp = $row->node_created;

      $today = new DateTime();
      $today->setTime(0, 0, 0); // Reset time part, to prevent partial comparison

      $match_date = new DateTime('@' . $timestamp);
      $match_date->setTime(0, 0, 0); // Reset time part, to prevent partial comparison

      $diff = $today->diff($match_date);
      $diff_days = (integer)$diff->format('%R%a'); // Extract days count in interval
      $diff_years = (integer)$diff->format('%R%y'); // Extract years count in interval

      if ($diff_days == 0) {
        $vars['output'] = format_date($row->node_created, 'current_day');
      }
      elseif ($diff_years == 0) {
        $vars['output'] = format_date($row->node_created, 'current_year');
      }
      else {
        $vars['output'] = format_date($row->node_created, 'years');
      }
    }

    if ($field->field == 'comment_count') {
      if (isset($row->node_new_comments)) {
        $node_mark = node_mark($row->nid, $row->node_changed);
        if ($node_mark == MARK_NEW || $row->node_new_comments == $row->node_comment_statistics_comment_count) {
          $vars['output'] = $row->node_new_comments ? '<span class="node-item__new-comments">' . $row->node_new_comments . '</span>' : '0';
        }
        else {
          $vars['output'] = $row->node_comment_statistics_comment_count . ($row->node_new_comments ? ' / <span class="node-item__new-comments">' . $row->node_new_comments . '</span>' : '');
        }
      }
    }
  }

  if (in_array($view->name, ['tracker', 'tracker_new', 'tracker_my'])) {
    if ($field->field == 'title') {
      $node = node_load($row->nid);
      $icon = drurum_node_icon($node);
      $vars['output'] = $icon . $vars['output'];
    }
  }
}
