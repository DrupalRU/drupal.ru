<?php

/**
 * @file
 * Make tokens about location available.
 */

/**
 * Implements hook_token_info().
 */
function location_token_info() {
  $type = array(
    'name' => t('Location', array(), array('context' => 'geolocation')),
    'description' => t('Tokens related to location.'),
    'needs-data' => 'location',
  );
  $location = array();

  // Core tokens for location.
  $location['name'] = array(
    'name' => t("Location Name"),
    'description' => t('Location Name (If there are multiple locations, N is the iteration, starting with 0).'),
    'dynamic' => TRUE,
  );
  $location['street'] = array(
    'name' => t("Street"),
    'description' => t("Street (If there are multiple locations, N is the iteration, starting with 0)."),
    'dynamic' => TRUE,
  );
  $location['additional'] = array(
    'name' => t("Additional"),
    'description' => t("Additional (If there are multiple locations, N is the iteration, starting with 0)."),
    'dynamic' => TRUE,
  );
  $location['city'] = array(
    'name' => t("City"),
    'description' => t("City (If there are multiple locations, N is the iteration, starting with 0)."),
    'dynamic' => TRUE,
  );
  $location['province'] = array(
    'name' => t("State/Province"),
    'description' => t("State/Province (If there are multiple locations, N is the iteration, starting with 0)."),
    'dynamic' => TRUE,
  );
  $location['province_name'] = array(
    'name' => t("State/Province Name"),
    'description' => t("State/Province Name (If there are multiple locations, N is the iteration, starting with 0)."),
    'dynamic' => TRUE,
  );
  $location['postal_code'] = array(
    'name' => t("Postal Code"),
    'description' => t("Postal Code (If there are multiple locations, N is the iteration, starting with 0)."),
    'dynamic' => TRUE,
  );
  $location['latitude'] = array(
    'name' => t("Latitude"),
    'description' => t("Latitude (If there are multiple locations, N is the iteration, starting with 0)."),
    'dynamic' => TRUE,
  );
  $location['longitude'] = array(
    'name' => t("Longitude"),
    'description' => t("Longitude (If there are multiple locations, N is the iteration, starting with 0)."),
    'dynamic' => TRUE,
  );
  $location['country'] = array(
    'name' => t("Country"),
    'description' => t("Country (If there are multiple locations, N is the iteration, starting with 0)."),
    'dynamic' => TRUE,
  );
  $location['country_name'] = array(
    'name' => t("Country Name"),
    'description' => t("Country Name (If there are multiple locations, N is the iteration, starting with 0)."),
    'dynamic' => TRUE,
  );

  $node = array(
    'location' => array(
      'name' => t('Location', array(), array('context' => 'geolocation')),
      'description' => t("The location for this node."),
      'type' => 'location',
    )
  );
  $user = array(
    'location' => array(
      'name' => t('Location', array(), array('context' => 'geolocation')),
      'description' => t("The location for this user."),
      'type' => 'location',
    )
  );

  return array(
    'types' => array('location' => $type),
    'tokens' => array('location' => $location, 'user' => $user, 'node' => $node),
  );
}

/**
 * Implements hook_tokens().
 */
function location_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ($type == 'node' && ($location_tokens = token_find_with_prefix($tokens, 'location'))) {
    $locations = isset($data['node']->locations) ? $data['node']->locations : array();
    $replacements += token_generate('location', $location_tokens, array('location' => $locations), $options);
  }
  elseif ($type == 'user' && ($location_tokens = token_find_with_prefix($tokens, 'location'))) {
    $locations = isset($data['user']->locations) ? $data['user']->locations : array();
    $replacements += token_generate('location', $location_tokens, array('location' => $locations), $options);
  }
  elseif ($type == 'location' && isset($data['location'])) {
    $locations = $data['location'];

    // Make sure that this is an array of locations and not a single location.
    if (isset($locations['lid'])) {
      $locations = array($locations);
    }

    // Handle all tokens that are dynamic. For example, location:name:1. This does
    // not handle location:name. That is handled lower.
    $tokens_helper = array(
      'name' => token_find_with_prefix($tokens, 'name'),
      'street' => token_find_with_prefix($tokens, 'street'),
      'additional' => token_find_with_prefix($tokens, 'additional'),
      'city' => token_find_with_prefix($tokens, 'city'),
      'province' => token_find_with_prefix($tokens, 'province'),
      'province_name' => token_find_with_prefix($tokens, 'province_name'),
      'postal_code' => token_find_with_prefix($tokens, 'postal_code'),
      'latitude' => token_find_with_prefix($tokens, 'latitude'),
      'longitude' => token_find_with_prefix($tokens, 'longitude'),
      'country' => token_find_with_prefix($tokens, 'country'),
      'country_name' => token_find_with_prefix($tokens, 'country_name'),
    );
    foreach ($tokens_helper as $key => $tokens_find) {
      foreach ($tokens_find as $val => $original) {
        $replacements[$original] = _locations_token_helper($locations, $key, $val, $options);
      }
    }

    // Handle values that do not have a specific location on them. For example,
    // handle location:city but not location:city:0 as it is handled above.
    foreach ($tokens as $key => $original) {
      if (isset($tokens_helper[$key])) {
        $replacements[$original] = _locations_token_helper($locations, $key, 0, $options);
      }
    }
  }

  return $replacements;
}

/**
 * Callback function to handle getting the correct location token.
 */
function _locations_token_helper($locations, $location_key, $which = 0, array $options = array()) {
  $sanitize = !empty($options['sanitize']);

  // That location doesn't exist.
  if (!isset($locations[$which])) {
    return '';
  }

  // Load the country name and return it.
  if ($location_key == 'country_name') {
    if (!empty($locations[$which]['country'])) {
      $val = location_country_name($locations[$which]['country']);

      return $sanitize ? check_plain($val) : $val;
    }

    return '';
  }

  // Load the province name and return it.
  if ($location_key == 'province_name') {
    if (!empty($locations[$which]['country']) && !empty($locations[$which]['province'])) {
      $val = location_province_name($locations[$which]['country'], $locations[$which]['province']);

      return $sanitize ? check_plain($val) : $val;
    }

    return '';
  }

  // Handle all other values.
  if (!empty($locations[$which][$location_key])) {
    return $sanitize ? check_plain($locations[$which][$location_key]) : $locations[$which][$location_key];
  }

  return '';
}
