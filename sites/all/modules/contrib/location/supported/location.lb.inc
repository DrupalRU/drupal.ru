<?php

/**
 * @file
 * Lebanon.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_lb() {
  return array(
    'AA' => "Aakkâr",
    'NA' => "An Nabaţīyah",
    'BH' => "Baalbek-Hermel",
    'BA' => "Beirut",
    'BQ' => "Beqaa",
    'JL' => "Mount Lebanon",
    'NL' => "North Lebanon",
    'JA' => "South Lebanon",
  );
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_lb() {
  return array(
    'minlng' => 35.2636,
    'minlat' => 33.262433,
    'maxlng' => 36.56995,
    'maxlat' => 34.720367,
  );
}
