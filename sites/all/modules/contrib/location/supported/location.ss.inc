<?php

/**
 * @file
 * South Sudan.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_ss() {
  return array(
    'EC' => "Central Equatoria",
    'EE' => "Eastern Equatoria",
    'JG' => "Jonglei",
    'LK' => "Lakes",
    'BN' => "Northern Bahr el-Ghazal",
    'UY' => "Unity",
    'NU' => "Upper Nile",
    'WR' => "Warrap",
    'BW' => "Western Bahr el-Ghazal",
    'EW' => "Western Equatoria"
  );
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_ss() {
  return array(
    'minlng' => 24.1849,
    'minlat' => 3.49339,
    'maxlng' => 35.94060,
    'maxlat' => 12.2377,
  );
}
