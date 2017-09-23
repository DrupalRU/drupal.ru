<?php

/**
 * @file
 * Malaysia.
 */

/**
 * Returns an associative array of states/territories by name.
 */
function location_province_list_my() {
  return array(
    'JH' => "Johor",
    'KD' => "Kedah",
    'KN' => "Kelantan",
    'KL' => "Kuala Lumpur",
    'ML' => "Malacca",
    'NS' => "Negeri Sembilan",
    'PH' => "Pahang",
    'PJ' => "Putrajaya",
    'PK' => "Perak",
    'PS' => "Perlis",
    'PG' => "Pulau Pinang",
    'SL' => "Selangor",
    'TR' => "Terengganu",
    'LB' => "Labuan",
    'SB' => "Sabah",
    'SR' => "Sarawak",
  );
}

/**
 * Returns an associative array of states/territories by number.
 */
function location_province_list_numeric_my() {
  return array(
    '001' => "Johor",
    '002' => "Kedah",
    '003' => "Kelantan",
    '004' => "Malacca",
    '005' => "Negeri Sembilan",
    '006' => "Pahang",
    '007' => "Perak",
    '008' => "Perlis",
    '009' => "Pulau Pinang",
    '010' => "Selangor",
    '011' => "Terengganu",
    '012' => "Sabah",
    '013' => "Sarawak",
    '014' => "Kuala Lumpur",
    '015' => "Labuan",
    '016' => "Putrajaya",
  );
}

/**
 * Link providers.
 */
function location_map_link_my_providers() {
  return array(
    'google' => array(
      'name' => 'Google Maps',
      'url' => 'http://maps.google.com.my',
      'tos' => 'http://www.google.com.my/help/terms_maps.html',
    ),
    'mapquest' => array(
      'name' => 'MapQuest',
      'url' => 'http://www.mapquest.com',
      'tos' => 'http://www.mapquest.com/features/main.adp?page=legal',
    ),
  );
}

/**
 * Default providers.
 */
function location_map_link_my_default_providers() {
  return array('google');
}

/**
 * Google link.
 */
function location_map_link_my_google($location = array()) {
  $query_params = array();
  $q = NULL;

  foreach (array('street', 'city', 'province', 'postal_code', 'country') as $field) {
    if (isset($location[$field])) {
      $query_params[] = $location[$field];
    }
  }

  if (location_has_coordinates($location)) {
    $q = urlencode($location['latitude'] . ' ' . $location['longitude'] . ' (' . implode(', ', $query_params) . ')');
  }
  elseif (count($query_params) > 0) {
    $q = urlencode(implode(", ", $query_params));
  }

  if ($q != NULL) {
    return ('http://maps.google.com.my?q=' . $q);
  }
  else {
    return NULL;
  }
}

/**
 * MapQuest link.
 */
function location_map_link_my_mapquest($location = array()) {
  if (location_has_coordinates($location)) {
    return 'http://www.mapquest.com/maps?l=' . urlencode($location['latitude']) . '&amp;g=' . urlencode($location['longitude']);
  }
  return NULL;
}
