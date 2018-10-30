<?php
/**
 * @file
 */

/**
 * Display the simple view of rows one after another.
 */
function druru_preprocess_views_view_unformatted(&$vars) {
  $view = $vars['view'];
  $rows = $vars['rows'];
  $result = $vars['view']->result;

  // Populate $result to be used in view tpl files
  $vars['result'] = $view->result;

  if ($view->name == 'events_upcoming_block') {
    foreach ($rows as $id => $row) {
      $vars['classes_array'][$id] = 'event';
    }
  }

  if (in_array($view->name, ['tracker', 'tracker_new', 'tracker_my', 'featured'])) {
    foreach ($rows as $id => $row) {
      $vars['classes_array'][$id] = $result[$id]->node_new_comments ? ' is-updated' : '';
    }
  }
}
