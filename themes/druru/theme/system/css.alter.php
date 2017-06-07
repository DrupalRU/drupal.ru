<?php

/**
 * Implements hook_css_alter().
 */
function druru_css_alter(&$css) {
  $theme_path = drupal_get_path('theme', 'druru');
  // Exclude specified CSS files from theme.
  $excludes = druru_get_theme_info(NULL, 'exclude][css');
  // Add Bootstrap CDN file and overrides.
  $bootstrap_cdn = theme_get_setting('druru_bootstrap_cdn');
  if ($bootstrap_cdn) {
    // Add CDN.
    if (theme_get_setting('druru_bootswatch')) {
      $cdn = '//netdna.bootstrapcdn.com/bootswatch/'
        . $bootstrap_cdn
        . '/'
        . theme_get_setting('druru_bootswatch')
        . '/bootstrap.min.css';
    }
    else {
      $cdn = '//netdna.bootstrapcdn.com/bootstrap/'
        . $bootstrap_cdn
        . '/css/bootstrap.min.css';
    }
    $css[$cdn] = array(
      'data'       => $cdn,
      'type'       => 'external',
      'every_page' => TRUE,
      'media'      => 'all',
      'preprocess' => FALSE,
      'group'      => CSS_THEME,
      'browsers'   => array('IE' => TRUE, '!IE' => TRUE),
      'weight'     => -2,
    );
    // Add overrides.
    $override = $theme_path . '/css/overrides.css';
    $css[$override] = array(
      'data'       => $override,
      'type'       => 'file',
      'every_page' => TRUE,
      'media'      => 'all',
      'preprocess' => TRUE,
      'group'      => CSS_THEME,
      'browsers'   => array('IE' => TRUE, '!IE' => TRUE),
      'weight'     => -1,
    );
  }
  if (!empty($excludes)) {
    $css = array_diff_ukey($css, drupal_map_assoc($excludes), function ($key1, $key2) {
      return mb_strpos($key1, $key2) === FALSE ? 1 : 0;
    });
  }
}
