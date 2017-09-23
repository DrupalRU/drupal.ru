<?php

/**
 * @file
 * Argentina.
 */

/**
 * Returns an associative array of states/territories where.
 *
 *   -> the keys are integers starting from 1
 *   -> the values are the English names for those states/territories
 *
 * The states are grouped together at the beginning of the array and sorted
 * alphabetically.
 *
 * The territories are grouped together at the end of the array and sorted
 * alphabetically.
 */
function location_province_list_ar() {
  return array(
    'A' => "Salta",
    'B' => "Buenos Aires",
    'C' => "Ciudad Autónoma de Buenos Aires (CABA)",
    'D' => "San Luis",
    'E' => "Entre Ríos",
    'F' => "La Rioja",
    'G' => "Santiago del Estero",
    'H' => "Chaco",
    'J' => "San Juan",
    'K' => "Catamarca",
    'L' => "La Pampa",
    'M' => "Mendoza",
    'N' => "Misiones",
    'P' => "Formosa",
    'Q' => "Neuquén",
    'R' => "Río Negro",
    'S' => "Santa Fe",
    'T' => "Tucumán",
    'U' => "Chubut",
    'V' => "Tierra del Fuego",
    'W' => "Corrientes",
    'X' => "Córdoba",
    'Y' => "Jujuy",
    'Z' => "Santa Cruz",
  );
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_ar() {
  return array(
    'minlng' => -73.4967,
    'minlat' => -55.049767,
    'maxlng' => -53.64205,
    'maxlat' => -21.829967,
  );
}
