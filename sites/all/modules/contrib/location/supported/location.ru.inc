<?php

/**
 * @file
 * Russian Federation principal subdivisions list
 * returns an array of ISO 3166 standard list.
 *
 * @see http://en.wikipedia.org/wiki/ISO_3166-2:RU
 */

/**
 * Returns an associative array of states/territories.
 */
function location_province_list_ru() {
  return array(
    'AD' => t("Adygeya, Respublika"),
    'BA' => t("Bashkortostan, Respublika"),
    'BU' => t("Buryatiya, Respublika"),
    'AL' => t("Altay, Respublika"),
    'DA' => t("Dagestan, Respublika"),
    'IN' => t("Ingushetiya, Respublika"),
    'KB' => t("Kabardino-Balkarskaya Respublika"),
    'KL' => t("Kalmykiya, Respublika"),
    'KC' => t("Karachayevo-Cherkesskaya Respublika"),
    'KR' => t("Kareliya, Respublika"),
    'KO' => t("Komi, Respublika"),
    'ME' => t("Mariy El, Respublika"),
    'MO' => t("Mordoviya, Respublika"),
    'SA' => t("Sakha, Respublika [Yakutiya]"),
    'SE' => t("Severnaya Osetiya-Alaniya, Respublika"),
    'TA' => t("Tatarstan, Respublika"),
    'TY' => t("Tyva, Respublika [Tuva]"),
    'UD' => t("Udmurtskaya Respublika"),
    'KK' => t("Khakasiya, Respublika"),
    'CE' => t("Chechenskaya Respublika"),
    'CU' => t("Chuvashskaya Respublika"),
    'ALT' => t("Altayskiy kray"),
    'KDA' => t("Krasnodarskiy kray"),
    'KYA' => t("Krasnoyarskiy kray"),
    'PRI' => t("Primorskiy kray"),
    'STA' => t("Stavropol'skiy kray"),
    'KHA' => t("Khabarovskiy kray"),
    'AMU' => t("Amurskaya oblast'"),
    'ARK' => t("Arkhangel'skaya oblast'"),
    'AST' => t("Astrakhanskaya oblast'"),
    'BEL' => t("Belgorodskaya oblast'"),
    'BRY' => t("Bryanskaya oblast'"),
    'VLA' => t("Vladimirskaya oblast'"),
    'VGG' => t("Volgogradskaya oblast'"),
    'VLG' => t("Vologodskaya oblast'"),
    'VOR' => t("Voronezhskaya oblast'"),
    'IVA' => t("Ivanovskaya oblast'"),
    'IRK' => t("Irkutskaya oblast'"),
    'KGD' => t("Kaliningradskaya oblast'"),
    'KLU' => t("Kaluzhskaya oblast'"),
    'KAM' => t("Kamchatskiy kray"),
    'KEM' => t("Kemerovskaya oblast'"),
    'KIR' => t("Kirovskaya oblast'"),
    'KOS' => t("Kostromskaya oblast'"),
    'KGN' => t("Kurganskaya oblast'"),
    'KRS' => t("Kurskaya oblast'"),
    'LEN' => t("Leningradskaya oblast'"),
    'LIP' => t("Lipetskaya oblast'"),
    'MAG' => t("Magadanskaya oblast'"),
    'MOS' => t("Moskovskaya oblast'"),
    'MUR' => t("Murmanskaya oblast'"),
    'NIZ' => t("Nizhegorodskaya oblast'"),
    'NGR' => t("Novgorodskaya oblast'"),
    'NVS' => t("Novosibirskaya oblast'"),
    'OMS' => t("Omskaya oblast'"),
    'ORE' => t("Orenburgskaya oblast'"),
    'ORL' => t("Orlovskaya oblast'"),
    'PNZ' => t("Penzenskaya oblast'"),
    'PER' => t("Permskiy kray"),
    'PSK' => t("Pskovskaya oblast'"),
    'ROS' => t("Rostovskaya oblast'"),
    'RYA' => t("Ryazanskaya oblast'"),
    'SAM' => t("Samarskaya oblast'"),
    'SAR' => t("Saratovskaya oblast'"),
    'SAK' => t("Sakhalinskaya oblast'"),
    'SVE' => t("Sverdlovskaya oblast'"),
    'SMO' => t("Smolenskaya oblast'"),
    'TAM' => t("Tambovskaya oblast'"),
    'TVE' => t("Tverskaya oblast'"),
    'TOM' => t("Tomskaya oblast'"),
    'TUL' => t("Tul'skaya oblast'"),
    'TYU' => t("Tyumenskaya oblast'"),
    'ULY' => t("Ul'yanovskaya oblast'"),
    'CHE' => t("Chelyabinskaya oblast'"),
    'YAR' => t("Yaroslavskaya oblast'"),
    'MOW' => t("Moskva"),
    'SPE' => t("Sankt-Peterburg"),
    'YEV' => t("Yevreyskaya avtonomnaya oblast'"),
    'NEN' => t("Nenetskiy avtonomnyy okrug"),
    'KHM' => t("Khanty-Mansiyskiy avtonomnyy okrug-Yugra"),
    'CHU' => t("Chukotskiy avtonomnyy okrug"),
    'YAN' => t("Yamalo-Nenetskiy avtonomnyy okrug"),
    'ZAB' => t("Zabaykal'skiy kray"),
  );
}

/**
 * Returns minimum and maximum latitude and longitude needed to create a bounding box.
 */
function location_bounds_ru() {
  // NaturalEarth 10m Admin 0 - Countries (v1.3.0)
  // EPSG:900913
  // The downside to bounding boxes is that crossing the antimeridian causes
  // serious issues. See http://purl.oclc.org/coordinates/a2.htm for discussion.
  return array(
    'minlng' => -179.99900,
    'minlat' => 41.1926805,
    'maxlng' => 180.000000,
    'maxlat' => 81.8583637,
  );
}
