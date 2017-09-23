<?php
/**
 * @file
 * Contains gmap_defaults.inc
 */

namespace tests\inc;

/**
 * Get the defaults for a gmap.
 */
function legacy_gmap_defaults() {
  $defaults = array(
    'width' => '300px',
    'height' => '200px',
    'zoom' => 3,
    'maxzoom' => 14,
    'controltype' => 'Small',
    'pancontrol' => 1,
    'streetviewcontrol' => 0,
    'align' => 'None',
    'latlong' => '40,0',
    'maptype' => 'Map',
    'mtc' => 'standard',
    'baselayers' => array('Map', 'Satellite', 'Hybrid'),
    'styles' => array(
      'line_default' => array('0000ff', 5, 45, '', 0, 0),
      'poly_default' => array('000000', 3, 25, 'ff0000', 45),
      'highlight_color' => 'ff0000',
    ),
    'line_colors' => array('#00cc00', '#ff0000', '#0000ff'),
  );
  $defaults['behavior'] = array();
  $m = array();
  $behaviors = legacy_gmap_module_invoke('behaviors', $m);
  foreach ($behaviors as $k => $v) {
    $defaults['behavior'][$k] = $v['default'];
  }
  $defaults = array_merge($defaults, variable_get('gmap_default', array()));
  return $defaults;
}

/**
 * Invokes hook_gmap() in every module.
 *
 * We can't use module_invoke_all() because we pass $map by reference.
 */
function legacy_gmap_module_invoke($op, &$map) {
  $return = array();
  foreach (module_implements('gmap') as $module) {
    $function = $module . '_gmap';
    $result = $function($op, $map);
    if (isset($result) && is_array($result)) {
      $return = array_merge_recursive($return, $result);
    }
    elseif (isset($result)) {
      $return[] = $result;
    }
  }
  return $return;
}

/**
 * Adds the basic js files needed for a GMap.
 *
 * Is called by hook_element_info().
 * To add js-files for a specific Views display,
 * please use _gmap_pre_render_map().
 */
function legacy__gmap_base_js() {
  $ret = array();
  $path = drupal_get_path('module', 'gmap');

  // Convert some language codes.
  // For Google Maps API v3, the drupal language code
  // is not always the same as the google language code.
  // @see https://developers.google.com/maps/documentation/javascript/basics#Localization
  global $language;
  switch ($language->language) {
    // 'Chinese, Simplified'.
    case 'zh-hans':
      $langcode = 'zh-CN';
      break;

    // 'Chinese, Traditional'.
    case 'zh-hant':
      $langcode = 'zh-TW';
      break;

    // Hebrew.
    case 'he':
      $langcode = 'iw';
      break;

    // 'Norwegian Bokm�l', 'Bokm�l'.
    case 'nb':
      // 'Norwegian Nynorsk', 'Nynorsk'.
    case 'nn':
      // 'Norwegian'.
      $langcode = 'no';
      break;

    default:
      $langcode = $language->language;
      break;
  }

  $m = array();
  $query = array(
    'v' => variable_get('gmap_api_version', GMAP_API_VERSION),
    'language' => $langcode,
    'sensor' => 'false',
    'libraries' => implode(array_merge(variable_get('gmap_api_libraries', array()), legacy_gmap_module_invoke('libraries', $m)), ','),
  );
  if ($key = legacy_gmap_get_key()) {
    $query['key'] = $key;
  }

  $ret[$path . '/js/gmap.js'] = array('weight' => 1);
  $ret[$path . '/js/icon.js'] = array('weight' => 2);
  $ret[$path . '/js/marker.js'] = array('weight' => 2);
  $ret[$path . '/js/highlight.js'] = array('weight' => 2);
  $ret[$path . '/js/poly.js'] = array('weight' => 2);
  $ret[url(legacy_gmap_views_protocol() . '://maps.googleapis.com/maps/' . 'api/js', array('query' => $query))] = array(
    'type' => 'external',
    'weight' => 1,
  );
  $ret[base_path() . variable_get('file_public_path', conf_path() . '/files') . '/js/gmap_markers.js'] = array(
    'type' => 'external',
    'weight' => 4,
  );

  return $ret;
}

/**
 * Retrieve the Google Maps key that is in use for the site.
 */
function legacy_gmap_get_key() {
  $key = variable_get('gmap_api_key', FALSE);
  if (module_exists('keys_api')) {
    $key = keys_api_get_key('gmap', $_SERVER['HTTP_HOST']);
  }
  elseif (module_exists('keys')) {
    $key = keys_get_key('google_maps');
  }
  return $key;
}

/**
 * Determine the site protocol (http or https).
 */
function legacy_gmap_views_protocol() {
  return (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? 'https' : 'http';
}
