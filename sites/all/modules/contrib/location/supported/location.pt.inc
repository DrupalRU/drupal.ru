<?php

/**
 * @file
 * Portugal.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_pt() {
  return array(
    'Aveiro' => "Aveiro",
    'Açores' => "Açores",
    'Beja' => "Beja",
    'Braga' => "Braga",
    'Bragança' => "Bragança",
    'Castelo Branco' => "Castelo Branco",
    'Coimbra' => "Coimbra",
    'Évora' => "Évora",
    'Faro' => "Faro",
    'Guarda' => "Guarda",
    'Leiria' => "Leiria",
    'Lisboa' => "Lisboa",
    'Madeira' => "Madeira",
    'Portalegre' => "Portalegre",
    'Porto' => "Porto",
    'Santarém' => "Santarém",
    'Setúbal' => "Setúbal",
    'Viana do Castelo' => "Viana do Castelo",
    'Vila Real' => "Vila Real",
    'Viseu' => "Viseu",
  );
}

/**
 * Link providers.
 */
function location_map_link_pt_providers() {
  return array(
    'google' => array(
      'name' => t('Google Maps'),
      'url' => 'http://maps.google.pt',
      'tos' => 'http://www.google.pt/help/terms_maps.html',
    ),
  );
}

/**
 * Default providers.
 */
function location_map_link_pt_default_providers() {
  return array('google');
}

/**
 * Google link.
 */
function location_map_link_pt_google($location = array()) {
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
  else {
    if (count($query_params) > 0) {
      $q = urlencode(implode(", ", $query_params));
    }
  }

  if ($q != NULL) {
    return ('http://maps.google.pt?q=' . $q);
  }
  else {
    return NULL;
  }

  if (count($query_params)) {
    return ('http://maps.google.pt?q=' . urlencode(implode(', ', $query_params)));
  }
  else {
    return NULL;
  }
}
