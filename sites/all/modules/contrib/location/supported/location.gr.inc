<?php

/**
 * @file
 * Greece.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_gr() {
  return array(
    'AT' => "Attica",
    'CN' => "Central Greece",
    'CM' => "Central Macedonia",
    'CR' => "Crete",
    'EM' => "East Macedonia and Thrace",
    'EP' => "Epirus",
    'II' => "Ionian Islands",
    'NA' => "North Aegean",
    'PP' => "Peloponnesos",
    'SA' => "South Aegean",
    'TH' => "Thessaly",
    'WG' => "West Greece",
    'WM' => "West Macedonia",
  );
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_gr() {
  return array(
    'minlng' => 19.65595,
    'minlat' => 34.851,
    'maxlng' => 28.16225,
    'maxlat' => 41.885033,
  );
}

/**
 * Google link.
 */
function location_map_link_gr_google($location = array()) {
  $query_params = array();

  foreach (array('street', 'city', 'province', 'postal_code') as $field) {
    if (isset($location[$field]) && $location[$field] != '') {
      $query_params[] = $location[$field];
    }
  }
  // Country always exists.
  if (count($query_params) != 0 && isset($location['country']) && $location['country'] != '') {
    $query_params[] = $location['country'];
  }
  // When no address provided, but coordinates do, link to coordinates.
  if (count($query_params) == 0) {
    foreach (array('latitude', 'longitude') as $field) {
      if (isset($location[$field])) {
        $query_params[] = $location[$field];
      }
    }
  }

  if (count($query_params)) {
    return ('http://maps.google.com?q=' . urlencode(implode(", ", $query_params)));
  }
  else {
    return NULL;
  }
}

/**
 * Providers.
 */
function location_map_link_gr_providers() {
  return array(
    'google' => array(
      'name' => t('Google Maps'),
      'url' => 'http://maps.google.gr',
      'tos' => 'http://www.google.gr/help/terms_maps.html',
    ),
  );
}

/**
 * Default providers.
 */
function location_map_link_gr_default_providers() {
  return array('google');
}
