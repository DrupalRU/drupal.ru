<?php

/**
 * Implements hook_preprocess_views_view().
 */
function druru_preprocess_views_view(&$vars) {
  $view = &$vars['view'];

  if ($view->name == 'user_comments') {
    drupal_add_js(drupal_get_path('theme', 'druru') . '/js/modules/views/js/user-profile-comments.js');
  }
}
