<?php

/**
 * @file
 * Czech Republic.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_cz() {
  return array(
    'JC' => "South Bohemian Region (Jihocesky kraj)",
    'JM' => "South Moravian Region (Jihomoravsky kraj)",
    'KA' => "Carlsbad Region  (Karlovarsky kraj)",
    'VY' => "Vysoc(ina Region (kraj Vysoc(ina)",
    'KR' => "Hradec Kralove Region (Kralovehradecky kraj)",
    'LI' => "Liberec Region (Liberecky kraj)",
    'MO' => "Moravian-Silesian Region (Moravskoslezsky kraj)",
    'OL' => "Olomouc Region (Olomoucky kraj)",
    'PA' => "Pardubice Region (Pardubicky kraj)",
    'PL' => "Plzen( Region Plzensky kraj)",
    'PR' => "Prague - the Capital (Praha - hlavni mesto)",
    'ST' => "Central Bohemian Region (Stredocesky kraj)",
    'US' => "Usti nad Labem Region (Ustecky kraj)",
    'ZL' => "Zlin Region (Zlinsky kraj)",
  );
}

/**
 * Providers.
 */
function location_map_link_cz_providers() {
  return array(
    'google' => array(
      'name' => t('Google Maps'),
      'url' => 'http://maps.google.cz',
      'tos' => 'http://www.google.cz/help/terms_maps.html',
    ),
  );
}

/**
 * Default providers.
 */
function location_map_link_cz_default_providers() {
  return array('google');
}

/**
 * Google link.
 */
function location_map_link_cz_google($location = array()) {
  $query_params = array();

  foreach (array('street', 'city', 'postal_code', 'country') as $field) {
    if (isset($location[$field])) {
      $query_params[] = $location[$field];
    }
  }

  if (count($query_params)) {
    return ('http://maps.google.cz?q=' . urlencode(implode(', ', $query_params)));
  }
  else {
    return NULL;
  }
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_cz() {
  return array(
    'minlng' => 12.0978,
    'minlat' => 48.5857,
    'maxlng' => 18.90825,
    'maxlat' => 51.041333,
  );
}
