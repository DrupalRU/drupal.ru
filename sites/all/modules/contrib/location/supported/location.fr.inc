<?php

/**
 * @file
 * France.
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_fr() {
  return array(
    'A01' => "Ain (01) ",
    'A02' => "Aisne (02)  ",
    'A03' => "Allier (03) ",
    'A04' => "Alpes-de-Haute-Provence (04) ",
    'A05' => "Hautes-Alpes (05) ",
    'A06' => "Alpes-Maritimes (06) ",
    'A07' => "Ardeche(07) ",
    'A08' => "Ardennes (08) ",
    'A09' => "Ariege (09) ",
    'A10' => "Aube (10) ",
    'A11' => " Aude (11) ",
    'A12' => "Aveyron (12) ",
    'A13' => "Bouches-du-Rhone (13) ",
    'A14' => "Calvados (14) ",
    'A15' => "Cantal (15) ",
    'A16' => "Charente (16)  ",
    'A17' => "Charente-Maritime (17) ",
    'A18' => "Cher (18) ",
    'A19' => "Correze (19) ",
    'A21' => "Cote-d'Or (21) ",
    'A22' => "Cotes-d'Armor (22) ",
    'A23' => "Creuse (23) ",
    'A24' => "Dordogne (24) ",
    'A25' => "Doubs (25) ",
    'A26' => "Drome (26) ",
    'A27' => "Eure (27) ",
    'A28' => "Eure-et-Loir (28) ",
    'A29' => "Finistere (29) ",
    'A30' => "Gard (30) ",
    'A31' => "Haute-Garonne (31) ",
    'A32' => "Gers (32) ",
    'A33' => "Gironde (33) ",
    'A34' => "Herault (34) ",
    'A35' => "Ille-et-Vilaine (35) ",
    'A36' => "Indre (36) ",
    'A37' => "Indre-et-Loire (37) ",
    'A38' => "Isere(38) ",
    'A39' => "Jura (39) ",
    'A40' => "Landes (40) ",
    'A41' => "Loir-et-Cher (41) ",
    'A42' => "Loire (42) ",
    'A43' => "Haute-Loire (43) ",
    'A44' => "Loire-Atlantique (44) ",
    'A45' => "Loiret (45) ",
    'A46' => "Lot (46) ",
    'A47' => "Lot-et-Garonne (47) ",
    'A48' => "Lozere(48) ",
    'A49' => "Maine-et-Loire (49) ",
    'A50' => "Manche (50) ",
    'A51' => "Marne (51) ",
    'A52' => "Haute-Marne (52) ",
    'A53' => "Mayenne (53) ",
    'A54' => "Meurthe-et-Moselle (54) ",
    'A55' => "Meuse (55) ",
    'A56' => "Morbihan (56) ",
    'A57' => "Moselle (57) ",
    'A58' => "Nievre(58) ",
    'A59' => "Nord (59) ",
    'A60' => "Oise (60) ",
    'A61' => "Orne (61) ",
    'A62' => "Pas-de-Calais (62) ",
    'A63' => "Puy-de-Dome (63) ",
    'A64' => "Pyrenees-Atlantiques (64) ",
    'A65' => "Hautes-Pyrenees (65) ",
    'A66' => "Pyrenees-Orientales (66) ",
    'A67' => "Bas-Rhin (67) ",
    'A68' => "Haut-Rhin (68) ",
    'A69' => "Rhone (69) ",
    'A70' => "Haute-Saone (70) ",
    'A71' => "Saone-et-Loire (71) ",
    'A72' => "Sarthe (72) ",
    'A73' => "Savoie (73) ",
    'A74' => "Haute-Savoie (74) ",
    'A75' => "Paris (75) ",
    'A76' => "Seine-Maritime (76) ",
    'A77' => "Seine-et-Marne (77) ",
    'A78' => "Yvelines (78) ",
    'A79' => "Deux-Sevres (79) ",
    'A80' => "Somme (80) ",
    'A81' => "Tarn (81) ",
    'A82' => "Tarn-et-Garonne (82) ",
    'A83' => "Var (83) ",
    'A84' => "Vaucluse (84) ",
    'A85' => "Vendee (85) ",
    'A86' => "Vienne (86) ",
    'A87' => "Haute-Vienne (87 ",
    'A88' => "Vosges (88) ",
    'A89' => "Yonne (89) ",
    'A90' => "Territoire de Belfort (90) ",
    'A91' => "Essonne (91) ",
    'A92' => "Hauts-de-Seine (92) ",
    'A93' => "Seine-Saint-Denis (93) ",
    'A94' => "Val-de-Marne (94) ",
    'A95' => "Val-d'Oise (95) ",
    'B2A' => "Corse-du-Sud (2A) ",
    'B2B' => "Haute-Corse (2B) ",
    'C71' => "Guadeloupe (971) ",
    'C72' => "Martinique (972) ",
    'C73' => "Guyane (973) ",
    'C74' => "La Reunion (974) ",
  );
}

/**
 * Providers.
 */
function location_map_link_fr_providers() {
  return array(
    'google' => array(
      'name' => t('Google Maps'),
      'url' => 'http://maps.google.fr/',
      'tos' => 'http://maps.google.fr//help/terms_maps.html',
    ),
  );
}

/**
 * Default providers.
 */
function location_map_link_fr_default_providers() {
  return array('google');
}

/**
 * Google link.
 */
function location_map_link_fr_google($location = array()) {
  $query_params = array();

  foreach (array('street', 'city', 'postal_code', 'country') as $field) {
    if (isset($location[$field])) {
      $query_params[] = $location[$field];
    }
  }

  if (count($query_params)) {
    return ('http://maps.google.fr/?q=' . urlencode(implode(', ', $query_params)));
  }
  else {
    return NULL;
  }
}
