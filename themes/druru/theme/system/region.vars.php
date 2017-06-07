<?php
/**
 * @file
 * region.vars.php
 */

/**
 * Implements hook_preprocess_region().
 */
function druru_preprocess_region(&$variables) {
  global $theme;
  static $wells;
  if (!isset($wells)) {
    foreach (system_region_list($theme) as $name => $title) {
      $wells[$name] = theme_get_setting('druru_region_well-' . $name);
    }
  }

  switch ($variables['region']) {
    // @todo is this actually used properly?
    case 'content':
      $variables['theme_hook_suggestions'][] = 'region__no_wrapper';
      break;

    case 'help':
      $variables['content'] = druru_icon('question-sign') . $variables['content'];
      $variables['classes_array'][] = 'alert';
      $variables['classes_array'][] = 'alert-info';
      break;
  }
  if (!empty($wells[$variables['region']])) {
    $variables['classes_array'][] = $wells[$variables['region']];
  }
}
