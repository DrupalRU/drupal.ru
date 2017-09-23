<?php

/**
 * Implements hook_js_alter().
 */
function druru_js_alter(&$js) {
  // Exclude specified JavaScript files from theme.
  $excludes = druru_get_theme_info(NULL, 'exclude][js');

  $theme_path = drupal_get_path('theme', 'druru');

  // Add or replace JavaScript files when matching paths are detected.
  // Replacement files must begin with '_', like '_node.js'.
  $files = file_scan_directory($theme_path . '/js', '/\.js$/');
  foreach ($files as $file) {
    $path = str_replace($theme_path . '/js/', '', $file->uri);
    // Detect if this is a replacement file.
    $replace = FALSE;
    if (preg_match('/^[_]/', $file->filename)) {
      $replace = TRUE;
      $path = dirname($path) . '/' . preg_replace('/^[_]/', '', $file->filename);
    }
    $matches = array();
    if (preg_match('/^modules\/([^\/]*)/', $path, $matches)) {
      if (!module_exists($matches[1])) {
        continue;
      }
      else {
        $path = str_replace('modules/' . $matches[1], drupal_get_path('module', $matches[1]), $path);
      }
    }
    // Path should always exist to either add or replace JavaScript file.
    if (!empty($js[$path])) {
      // Replace file.
      if ($replace) {
        $js[$file->uri] = $js[$path];
        $js[$file->uri]['data'] = $file->uri;
        unset($js[$path]);
      }
      // Add file.
      else {
        $js[$file->uri] = drupal_js_defaults($file->uri);
        $js[$file->uri]['group'] = JS_THEME;
      }
    }
  }

  // Always add druru.js last.
  $script = $theme_path . '/js/druru.js';
  $js[$script] = drupal_js_defaults($script);
  $js[$script]['group'] = JS_THEME;
  $js[$script]['scope'] = 'footer';

  if (!empty($excludes)) {
    $js = array_diff_key($js, drupal_map_assoc($excludes));
  }

  // Add Bootstrap settings.
  $js['settings']['data'][]['druru'] = array(
    'anchorsFix' => theme_get_setting('druru_anchors_fix'),
    'anchorsSmoothScrolling' => theme_get_setting('druru_anchors_smooth_scrolling'),
    'popoverEnabled' => theme_get_setting('druru_popover_enabled'),
    'popoverOptions' => array(
      'animation' => (int) theme_get_setting('druru_popover_animation'),
      'html' => (int) theme_get_setting('druru_popover_html'),
      'placement' => theme_get_setting('druru_popover_placement'),
      'selector' => theme_get_setting('druru_popover_selector'),
      'trigger' => implode(' ', array_filter(array_values((array) theme_get_setting('druru_popover_trigger')))),
      'title' => theme_get_setting('druru_popover_title'),
      'content' => theme_get_setting('druru_popover_content'),
      'delay' => (int) theme_get_setting('druru_popover_delay'),
      'container' => theme_get_setting('druru_popover_container'),
    ),
    'tooltipEnabled' => theme_get_setting('druru_tooltip_enabled'),
    'tooltipOptions' => array(
      'animation' => (int) theme_get_setting('druru_tooltip_animation'),
      'html' => (int) theme_get_setting('druru_tooltip_html'),
      'placement' => theme_get_setting('druru_tooltip_placement'),
      'selector' => theme_get_setting('druru_tooltip_selector'),
      'trigger' => implode(' ', array_filter(array_values((array) theme_get_setting('druru_tooltip_trigger')))),
      'delay' => (int) theme_get_setting('druru_tooltip_delay'),
      'container' => theme_get_setting('druru_tooltip_container'),
    ),
  );

  // Add CDN.
  if (theme_get_setting('druru_bootstrap_cdn')) {
    $cdn = '//netdna.bootstrapcdn.com/bootstrap/' . theme_get_setting('druru_bootstrap_cdn')  . '/js/bootstrap.min.js';
    $js[$cdn] = drupal_js_defaults();
    $js[$cdn]['data'] = $cdn;
    $js[$cdn]['type'] = 'external';
    $js[$cdn]['every_page'] = TRUE;
    $js[$cdn]['weight'] = -100;
  }

  druru_add_js_settings();
  foreach ($js as $path => &$data) {
    $data['scope'] = 'footer';
  }
}
