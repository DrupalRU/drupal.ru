<?php

/**
 * @file
 * China.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_cn() {
  return array(
    '11' => t("Beijing"),
    '12' => t("Tianjin"),
    '13' => t("Hebei"),
    '14' => t("Shanxi"),
    '15' => t("Nei Mongol"),
    '21' => t("Liaoning"),
    '22' => t("Jilin"),
    '23' => t("Heilongjiang"),
    '31' => t("Shanghai"),
    '32' => t("Jiangsu"),
    '33' => t("Zhejiang"),
    '34' => t("Anhui"),
    '35' => t("Fujian"),
    '36' => t("Jiangxi"),
    '37' => t("Shandong"),
    '41' => t("Henan"),
    '42' => t("Hubei"),
    '43' => t("Hunan"),
    '44' => t("Guangdong"),
    '45' => t("Guangxi"),
    '46' => t("Hainan"),
    '51' => t("Sichuan"),
    '52' => t("Guizhou"),
    '53' => t("Yunnan"),
    '54' => t("Xizang (Tibet)"),
    '61' => t("Shaanxi"),
    '62' => t("Gansu"),
    '63' => t("Qinghai"),
    '64' => t("Ningxia"),
    '65' => t("Xinjiang"),
    '71' => t("Chinese Taipei"),
    '91' => t("Xianggang"),
    '92' => t("Aomen"),
    '97' => t("Chongqing"),
  );
}

/**
 * Providers.
 */
function location_map_link_cn_providers() {
  return array(
    'google' => array(
      'name' => t('Google Maps'),
      'url' => 'http://maps.google.com.hk',
      'tos' => 'http://www.google.com.hk/help/terms_maps.html',
    ),
  );
}

/**
 * Default providers.
 */
function location_map_link_cn_default_providers() {
  return array('google');
}

/**
 * Google link.
 */
function location_map_link_cn_google($location = array()) {
  $query_params = array();

  foreach (array('street', 'city', 'postal_code', 'country') as $field) {
    if (isset($location[$field])) {
      $query_params[] = $location[$field];
    }
  }

  if (count($query_params)) {
    return ('http://maps.google.com.hk?q=' . urlencode(implode(', ', $query_params)));
  }
  else {
    return NULL;
  }
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_cn() {
  return array(
    'minlng' => 73.5403,
    'minlat' => 16.0968,
    'maxlng' => 134.7658,
    'maxlat' => 53.608867,
  );
}
