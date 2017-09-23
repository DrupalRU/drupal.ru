<?php

/**
 * @file
 * Encoded polyline utilities.
 */

/**
 * References:
 * [1] http://code.google.com/apis/maps/documentation/polylinealgorithm.html
 * [2] http://facstaff.unca.edu/mcmcclur/GoogleMaps/EncodePolyline/
 * [3] http://mathworld.wolfram.com/
 */

define('GMAP_DP_EPSILON', 0.00001);
define('GMAP_ZOOM_LEVELS', 18);
define('GMAP_ZOOM_FACTOR', 2);

use Drupal\gmap\GmapPolylineToolbox;

/**
 * The following three functions will encode numbers so that they may be used
 * in "Encoded Polylines" on Google Maps. The encoding is described here:
 *   http://code.google.com/apis/maps/documentation/polylinealgorithm.html
 *
 * Numbers for latitudes/longitudes and levels are encoded slightly
 * differently--when generating Encoded Polylines, latitudes and longitudes are
 * encoded with gmap_polyutil_encode_signed(), and "levels" are encoded using
 * gmap_polyutil_encode_unsigned().
 *
 * @deprecated use
 * GmapPolylineToolbox::getInstance()->setLatLonNumber($x)->getEncodedLatLon();
 */

/**
 * Encode latlon function.
 *
 * @deprecated use
 * GmapPolylineToolbox::getInstance()->setLatLonNumber($x)->getEncodedLatLon();
 */
function gmap_polyutil_encode_latlon($x) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapPolylineToolbox.php';
  return GmapPolylineToolbox::getInstance()->setLatLonNumber($x)->getEncodedLatLon();
}

/**
 * Encode levels function.
 *
 * @deprecated use
 * GmapPolylineToolbox::getInstance()->setLatLonNumber($x)->getEncodedLevels();
 */
function gmap_polyutil_encode_levels($x) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapPolylineToolbox.php';
  return GmapPolylineToolbox::getInstance()->setLatLonNumber($x)->getEncodedLevels();
}

/**
 * Encode polyutil function.
 *
 * @deprecated use
 * GmapPolylineToolbox::getInstance()->setLatLonNumber($x)->getEncode();
 */
function _gmap_polyutil_encode($x) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapPolylineToolbox.php';
  return GmapPolylineToolbox::getInstance()->setLatLonNumber($x)->getEncode();
}

/**
 * Distance in two dimensions.
 *
 * √((x1-x0)^2 + (y1-y0)^2)
 *
 * @deprecated use
 * GmapPolylineToolbox::getInstance()->setLinePoints($p1, $p2)->getDist();
 */
function gmap_polyutil_dist($p1, $p2) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapPolylineToolbox.php';
  return GmapPolylineToolbox::getInstance()->setLinePoints($p1, $p2)->getDist();
}

/**
 * Distance between a point and a line segment.
 *
 * @param array $q
 *   Point to measure.
 *
 * @param array $p1
 *   Start point of line segment.
 *
 * @param array $p2
 *   End point of line segment.
 *
 * @return float
 *   Distance.
 *
 * @deprecated use
 * GmapPolylineToolbox::getInstance()
 *  ->setMeasurePoint($q)->setLinePoints($p1, $p2)->getPointLineDist();
 */
function gmap_polyutil_point_line_dist($q, $p1, $p2) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapPolylineToolbox.php';
  return GmapPolylineToolbox::getInstance()->setMeasurePoint($q)->setLinePoints($p1, $p2)->getPointLineDist();
}

/**
 * Implementation of the Douglas-Peucker polyline simplification algorithm.
 *
 * See:
 * http://facstaff.unca.edu/mcmcclur/GoogleMaps/EncodePolyline/algorithm.html
 *
 * @param array $points
 *   An array of coordinate pairs.
 *
 * @return array
 *   An array of keys => weights; the keys correspond with indices of points in
 *   the $points array. Some points may be insignificant according to the
 *   algorithm--they will not have entries in the return array. The "weights"
 *   are actually the point's distance from the line segment that it subdivides.
 *
 * @deprecated use
 * GmapPolylineToolbox::getInstance()->setPoints($points)->getDPEncode();
 */
function gmap_polyutil_dp_encode($points) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapPolylineToolbox.php';
  return GmapPolylineToolbox::getInstance()->setPoints($points)->getDPEncode();
}

/**
 * Simplify a set of points and generate an "Encoded Polyline" for Google Maps.
 *
 * @param array $points
 *   An array of coordinate pairs as latitude, longitude.
 *
 * @return array
 *   An array containing the point and zoom information necessary to display
 *   encoded polylines on Google Maps:
 *   'points', 'levels', 'numLevels', and 'zoomFactor'.
 *
 * @deprecated use
 * GmapPolylineToolbox::getInstance()->setPoints($points)->getPolyline();
 */
function gmap_polyutil_polyline($points) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapPolylineToolbox.php';
  return GmapPolylineToolbox::getInstance()->setPoints($points)->getPolyline();
}

/**
 * Build a logarithmic scale of zoom levels.
 *
 * @deprecated use GmapPolylineToolbox::getInstance()->getZoomLevels();
 */
function _gmap_polyutil_zoom_levels() {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapPolylineToolbox.php';
  return GmapPolylineToolbox::getInstance()->getZoomLevels();
}

/**
 * Place points in levels based on their "weight".
 *
 * A value derived from distance calculations in the simplification algorithm,
 * gmap_polyutil_dp_encode().
 *
 * @deprecated use
 * GmapPolylineToolbox::getInstance()->setWeight($weight)->getZoomLevel();
 */
function _gmap_polyutil_get_zoom_level($weight) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapPolylineToolbox.php';
  return GmapPolylineToolbox::getInstance()->setWeight($weight)->getZoomLevel();
}
