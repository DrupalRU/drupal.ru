<?php

/**
 * Implements hook_views_default_views().
 */
function altpager_example_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'altpager_example') . '/views', '/.*\.view$/');
  $views = array();

  foreach ($files as $absolute => $file) {
    require_once $absolute;

    if (isset($view)) {
      $views[$view->name] = $view;
      $view = NULL;
    }
  }

  return $views;
}
