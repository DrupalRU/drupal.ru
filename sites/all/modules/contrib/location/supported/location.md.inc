<?php

/**
 * @file
 * Moldova, Republic of.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_md() {
  return array(
    'C' => "Chișinău",
    'AN' => "Anenii Noi",
    'BS' => "Basarabeasca",
    'BE' => "Bender",
    'BR' => "Briceni",
    'CH' => "Cahul",
    'CC' => "Camenca",
    'CT' => "Cantemir",
    'CL' => "Călărași",
    'CS' => "Căușeni",
    'CM' => "Cimișlia",
    'CR' => "Criuleni",
    'DN' => "Dondușeni",
    'DR' => "Drochia",
    'DB' => "Dubăsari",
    'ED' => "Edineț",
    'FL' => "Fălești",
    'FR' => "Florești",
    'GL' => "Glodeni",
    'GR' => "Grigoriopol",
    'HN' => "Hîncești",
    'IL' => "Ialoveni",
    'LV' => "Leova",
    'NS' => "Nisporeni",
    'OC' => "Ocnița",
    'OR' => "Orhei",
    'RZ' => "Rezina",
    'RB' => "Rîbnița",
    'RS' => "Rîșcani",
    'SG' => "Sîngerei",
    'SL' => "Slobozia",
    'SR' => "Soroca",
    'ST' => "Strășeni",
    'SD' => "Șoldănești",
    'SV' => "Ștefan Vodă",
    'TR' => "Taraclia",
    'TL' => "Telenești",
    'TS' => "Tiraspol",
    'UN' => "Ungheni",
    'GE' => "UTA Găgăuzia",
  );
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_md() {
  // NaturalEarth 10m Admin 0 - Countries (v1.3.0)
  // EPSG:900913
  return array(
    'minlng' => 26.6178890,
    'minlat' => 45.4617739,
    'maxlng' => 30.1315763,
    'maxlat' => 48.4860338,
  );
}
