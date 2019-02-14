<?php
/**
 * @file
 */

/**
 * Preprocess the primary theme implementation for a view.
 */
function druru_preprocess_views_view(&$vars) {
  $view = $vars['view'];

  $views_tracker = ['tracker', 'tracker_new', 'tracker_my', 'tracker_featured'];
  $views_user    = ['user_blog', 'user_comments'];

  if (in_array($view->name, array_merge($views_tracker, $views_user))) {
    $vars['classes_array'] = [];
    $vars['classes_array'][] = 'entity-listing';
    $vars['classes_array'][] = 'entity-listing--' . drupal_clean_css_identifier($vars['name']);
  }

  if (in_array($view->name, $views_tracker)) {
    $vars['classes_array'][] = 'entity-listing--' . $view->current_display;
  }

  if (in_array($view->name, $views_user)) {
    $vars['classes_array'][] = 'entity-listing--extended';
  }
}
