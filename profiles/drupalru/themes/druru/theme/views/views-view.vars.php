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

/**
 * Implements hook_views_pre_render().
 */
function druru_views_pre_render(&$view) {
   dpm($view); // dpm view here to see its properties.

  //if ($view->name == 'homepage' && $view->current_display = 'homepage_page_display') {
  //  $view->header['area']->options['content'] = t('TEST message');
  //}
}
