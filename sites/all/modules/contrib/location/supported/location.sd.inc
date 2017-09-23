<?php

/**
 * @file
 * Sudan.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_sd() {
  return array(
    'RS' => "Al Bahr al Ahmar",
    'GZ' => "Al Jazirah ",
    'KH' => "Al Khartum ",
    'GD' => "Al Qadarif ",
    'NR' => "An Nil ",
    'NW' => "An Nil al Abyad",
    'NB' => "An Nil al Azraq",
    'NO' => "Ash Shamaliyah",
    'DW' => "Gharb Darfur",
    'DS' => "Janub Darfur",
    'KS' => "Janub Kurdufan",
    'KA' => "Kassala",
    'DN' => "Shamal Darfur",
    'KN' => "Shamal Kurdufan",
    'DE' => "Sharq  Darfur",
    'SI' => "Sinnar",
    'DC' => "Zalingei"
  );
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_sd() {
  return array(
    'minlng' => 21.8291,
    'minlat' => 8.68694,
    'maxlng' => 38.6007,
    'maxlat' => 22.232201,
  );
}
