<?php
/**
 * @file
 * Contains GmapDefaults.php
 */

namespace Drupal\gmap;


class GmapDefaults {
  /**
   * @var array containing gmap defaults
   * former gmap_defaults()
   */
  private $defaults;

  /**
   * @var array containing gmap base js data
   * former _gmap_base_js()
   */
  private $basejs;

  /**
   * @var static Singleton instance
   */
  static private $gmapInstance;

  /**
   * Do not change.
   */
  private function __construct() {
    $this->defaults = array(
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
    $this->defaults['behavior'] = array();
    // @todo refactor this for removal
    $m = array();
    // @todo convert to class GmapBehaviours or method addBehavior
    $behaviors = gmap_module_invoke('behaviors', $m);
    foreach ($behaviors as $k => $v) {
      $this->defaults['behavior'][$k] = $v['default'];
    }
    $this->defaults = array_merge($this->defaults, variable_get('gmap_default', array()));

    // Former _gmap_base_js().
    $this->basejs = array();
    $path = drupal_get_path('module', 'gmap');

    // Convert some language codes.
    // For Google Maps API v3, the drupal language code is not always
    // the same as the google language code.
    // @see
    // https://developers.google.com/maps
    // + /documentation/javascript/basics#Localization
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
      'libraries' => implode(array_merge(variable_get('gmap_api_libraries', array()), gmap_module_invoke('libraries', $m)), ','),
    );
    if ($key = gmap_get_key()) {
      $query['key'] = $key;
    }

    $this->basejs[$path . '/js/gmap.js'] = array('weight' => 1);
    $this->basejs[$path . '/js/icon.js'] = array('weight' => 2);
    $this->basejs[$path . '/js/marker.js'] = array('weight' => 2);
    $this->basejs[$path . '/js/highlight.js'] = array('weight' => 2);
    $this->basejs[$path . '/js/poly.js'] = array('weight' => 2);
    $this->basejs[url(gmap_views_protocol() . '://maps.googleapis.com/maps/' . 'api/js', array('query' => $query))] = array(
      'type' => 'external',
      'weight' => 1,
    );
    $this->basejs[base_path() . variable_get('file_public_path', conf_path() . '/files') . '/js/gmap_markers.js'] = array(
      'type' => 'external',
      'weight' => 4,
    );
  }

  /**
   * Do not clone.
   */
  protected function __clone() {
  }

  /**
   * Getting an instance.
   *
   * @return object
   *   GmapDefaults SingleTon instance
   */
  static public function getInstance() {
    if (is_null(self::$gmapInstance)) {
      self::$gmapInstance = new self();
    }
    return self::$gmapInstance;
  }

  /**
   * Get defaults.
   *
   * @return array
   *   $this->defaults
   */
  public function getDefaults() {
    return $this->defaults;
  }

  /**
   * Get base JS.
   *
   * @return array
   *   $this->basejs
   */
  public function getBaseJs() {
    return $this->basejs;
  }
}
