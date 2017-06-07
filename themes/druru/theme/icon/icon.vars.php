<?php
/**
 * @file
 * icon.vars.php
 */

/**
 * Implements hook_preprocess_icon().
 *
 * Bootstrap requires an additional "glyphicon" class for all icons.
 *
 * @see icon_preprocess_icon_image()
 * @see template_preprocess_icon()
 */
function druru_preprocess_icon(&$variables) {
  $bundle = &$variables['bundle'];
  if ($bundle['provider'] === 'druru') {
    $variables['attributes']['class'][] = 'glyphicon';
  }
}
