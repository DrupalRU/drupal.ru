<?php
/**
 * @file
 */

/**
 * Implements hook_preprocess_views_view_field
 */
function druru_preprocess_views_view_fields(&$vars) {
  $view = $vars['view'];
  $fields = $vars['fields'];
  $row = $vars['row'];

  if (in_array($view->name, ['tracker', 'tracker_my'])) {
    $vars['fields']['display_date'] = strip_tags($fields['last_updated']->content);
  }

  if (in_array($view->name, ['tracker_new', 'featured'])) {
    $vars['fields']['display_date'] = strip_tags($fields['created']->content);
  }
}
