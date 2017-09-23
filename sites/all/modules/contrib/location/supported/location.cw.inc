<?php

/**
 * @file
 * Curaçao.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_cw() {
  return array();
}

/**
 * Returns minimum and maximum latitude and longitude needed for bounding box.
 */
function location_bounds_cw() {
  return array(
    'minlng' => -69.157204,
    'minlat' => 12.385672,
    'maxlng' => -68.733948,
    'maxlat' => 12.385672,
  );
}
