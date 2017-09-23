<?php

/**
 * @file
 * Great Britain.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_gb() {
  return array(
    'ABD' => 'Aberdeenshire',
    'ALD' => 'Alderney',
    'AGY' => 'Anglesey (Isle of)',
    'ANS' => 'Angus',
    'AGB' => 'Argyll and Bute',
    'AYR' => 'Ayrshire',
    'BAN' => 'Banffshire',
    'BDF' => 'Bedfordshire',
    'BRK' => 'Berkshire',
    'BEW' => 'Berwickshire',
    'BGW' => 'Blaenau Gwent',
    'BOR' => 'Borders',
    'BGE' => 'Bridgend',
    'BST' => 'Bristol',
    'BKM' => 'Buckinghamshire',
    'CAY' => 'Caerphilly',
    'CAI' => 'Caithness',
    'CAM' => 'Cambridgeshire',
    'CRF' => 'Cardiff',
    'CMN' => 'Carmarthenshire',
    'CEN' => 'Central Scotland',
    'CGN' => 'Ceredigion',
    'CHI' => 'Channel Islands',
    'CHS' => 'Cheshire',
    'CLK' => 'Clackmannanshire',
    'CWD' => 'Clwyd',
    'CWY' => 'Conwy',
    'CON' => 'Cornwall',
    'ANT' => 'County Antrim',
    'ARM' => 'County Armagh',
    'DOW' => 'County Down',
    'DUR' => 'County Durham',
    'TYR' => 'County Tyrone',
    'CMA' => 'Cumbria',
    'DEN' => 'Denbighshire',
    'DER' => 'Derbyshire',
    'DEV' => 'Devon',
    'DOR' => 'Dorset',
    'DGY' => 'Dumfries & Galloway',
    'DNB' => 'Dunbartonshire',
    'DYD' => 'Dyfed',
    'ELN' => 'East Lothian',
    'ERY' => 'East Riding of Yorkshire',
    'ESX' => 'East Sussex',
    'ESS' => 'Essex',
    'FER' => 'Fermanagh',
    'FIF' => 'Fife',
    'FLN' => 'Flintshire',
    'GLA' => 'Glamorgan',
    'GLS' => 'Gloucestershire',
    'GMP' => 'Grampian',
    'GSY' => 'Guernsey',
    'GNT' => 'Gwent',
    'GWN' => 'Gwynedd',
    'HAM' => 'Hampshire',
    'HEF' => 'Herefordshire',
    'HRT' => 'Hertfordshire',
    'HLD' => 'Highlands',
    'INV' => 'Inverness-shire',
    'IOM' => 'Isle of Man',
    'IOW' => 'Isle of Wight',
    'JSY' => 'Jersey',
    'KEN' => 'Kent',
    'KCD' => 'Kincardineshire',
    'KRS' => 'Kinross-shire',
    'KKD' => 'Kirkcudbrightshire',
    'LKS' => 'Lanarkshire',
    'LAN' => 'Lancashire',
    'LEI' => 'Leicestershire',
    'LIN' => 'Lincolnshire',
    'LND' => 'London',
    'LDY' => 'Londonderry',
    'GTM' => 'Manchester (Greater)',
    'MSY' => 'Merseyside',
    'MTY' => 'Merthyr Tydfil',
    'MDX' => 'Middlesex',
    'MLN' => 'Midlothian',
    'MON' => 'Monmouthshire',
    'MOR' => 'Moray',
    'NAI' => 'Nairnshire',
    'NTL' => 'Neath Port Talbot',
    'NWP' => 'Newport',
    'NFK' => 'Norfolk',
    'NYK' => 'North Yorkshire',
    'NTH' => 'Northamptonshire',
    'NBL' => 'Northumberland',
    'NTT' => 'Nottinghamshire',
    'OKI' => 'Orkney',
    'OXF' => 'Oxfordshire',
    'PEE' => 'Peeblesshire',
    'PEM' => 'Pembrokeshire',
    'PER' => 'Perthshire',
    'POW' => 'Powys',
    'RFW' => 'Renfrewshire',
    'RCT' => 'Rhondda Cynon Taff',
    'ROC' => 'Ross and Cromarty',
    'ROX' => 'Roxburghshire',
    'SEL' => 'Selkirkshire',
    'SHI' => 'Shetland',
    'SHR' => 'Shropshire',
    'SOM' => 'Somerset',
    'SYK' => 'South Yorkshire',
    'STS' => 'Staffordshire',
    'STI' => 'Stirlingshire',
    'STD' => 'Strathclyde',
    'SFK' => 'Suffolk',
    'SRY' => 'Surrey',
    'SUT' => 'Sutherland',
    'SWA' => 'Swansea',
    'TAY' => 'Tayside',
    'TOF' => 'Torfaen',
    'TWR' => 'Tyne & Wear',
    'WAR' => 'Warwickshire',
    'WLN' => 'West Lothian',
    'WMD' => 'West Midlands',
    'SXW' => 'West Sussex',
    'WYK' => 'West Yorkshire',
    'WIS' => 'Western Isles',
    'WIG' => 'Wigtownshire',
    'WIL' => 'Wiltshire',
    'WOR' => 'Worcestershire',
    'WRX' => 'Wrexham',
  );
}

/**
 * Providers.
 */
function location_map_link_gb_providers() {
  return array(
    'google' => array(
      'name' => t('Google Maps'),
      'url' => 'http://maps.google.co.uk',
      'tos' => 'http://www.google.co.uk/help/terms_maps.html',
    ),
  );
}

/**
 * Default providers.
 */
function location_map_link_gb_default_providers() {
  return array('google');
}

/**
 * Google link.
 */
function location_map_link_gb_google($location = array()) {
  $query_params = array();

  foreach (array('street', 'city', 'postal_code', 'country') as $field) {
    if (isset($location[$field])) {
      $query_params[] = $location[$field];
    }
  }

  if (count($query_params)) {
    return ('http://maps.google.co.uk?q=' . urlencode(implode(', ', $query_params)));
  }
  else {
    return NULL;
  }
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_gb() {
  // NaturalEarth 10m Admin 0 - Countries (v1.3.0)
  // EPSG:900913
  // From ISO code GB.
  return array(
    'minlng' => -13.691355,
    'minlat' => 49.9096161,
    'maxlng' => 1.77170536,
    'maxlat' => 60.8475532,
  );
}
