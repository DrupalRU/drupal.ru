<?php
/**
 * @file
 */

/**
 * Display the simple view of rows one after another.
 */
function druru_preprocess_views_view_unformatted(&$vars) {
  $view = $vars['view'];

  if ($view->name == 'events_upcoming_block') {
    foreach ($view->result as $id => $row) {
      $vars['classes_array'][$id] = 'event';
    }
  }
}
