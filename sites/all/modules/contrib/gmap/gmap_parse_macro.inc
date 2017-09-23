<?php

/**
 * @file
 * GMap macro parsing routines.
 */

use Drupal\gmap\GmapMacroToolbox;

/**
 * Parse a macro style definition.
 *
 * Example: #111111/1/100/#111111/1
 * @internal
 *
 * @deprecated use
 * GmapMacroToolbox::getInstance()->setStyle($style)->getParsedStyles()
 */
function _gmap_parse_style($style) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapMacroToolbox.php';
  return GmapMacroToolbox::getInstance()->setStyle($style)->getParsedStyles();
}

/**
 * Parse "x.xxxxx , y.yyyyyy (+ x.xxxxx, y.yyyyy ...)" into an array of points.
 *
 * @internal
 *
 * @deprecated use
 * GmapMacroToolbox::getInstance()->setStyle($style)->getParsedStyles()
 */
function _gmap_str2coord($str) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapMacroToolbox.php';
  return GmapMacroToolbox::getInstance()->setCoordString($str)->getCoord();
}

/**
 * Gmap parse macro.
 *
 * @param string $instring
 *   String macro.
 *
 * @param int $ver
 *   Version.
 *
 * @return array
 *   Parsed macro.
 *
 * @deprecated use
 *  GmapMacroToolbox::getInstance()
 *  ->setMacroString($instring, $ver = 2)->getParsedMacro()
 */
function _gmap_parse_macro($instring, $ver = 2) {
  include_once drupal_get_path('module', 'gmap') . '/lib/Drupal/gmap/GmapMacroToolbox.php';
  return GmapMacroToolbox::getInstance()->setMacroString($instring, $ver = 2)->getParsedMacro();
}
