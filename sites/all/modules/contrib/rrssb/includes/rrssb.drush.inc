<?php
/**
 * @file
 * Provides drush commands for Ridiculously Responsive Social Share Buttons.
 *
 * Courtesy of cluke009 at https://drupal.org/node/1231378 .
 */

/**
 * Implements hook_drush_command().
 */
function rrssb_drush_command() {
  $items['rrssb-plugin'] = array(
    'callback' => 'drush_rrssb_plugin',
    'description' => dt("Downloads the Ridiculously Responsive Social Share Buttons library from Github."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Path is optional. Will use default location if omitted. Default location is sites/all/libraries.')
    ),
    'aliases' => array('rrssbdl'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function rrssb_drush_help($section) {
  switch ($section) {
    case 'drush:rrssb-plugin':
      return dt("Downloads the Ridiculously Responsive Social Share Buttons library from Github. Downloads to sites/all/libraries unless a location is provided.");
  }
}

/**
 * Implements drush_MODULE_post_pm_enable().
 */
function drush_rrssb_post_pm_enable() {
  $modules = func_get_args();
  if (in_array('rrssb', $modules)) {
    drush_rrssb_plugin(NULL, TRUE);
  }
}

/**
 * Callback for the drush command to download the RRSSB library.
 */
function drush_rrssb_plugin($destination = NULL, $only_if_missing = FALSE) {
  if (!$destination) {
    $destination = 'sites/all/libraries';
  }

  // The zip extracts to create a temporary directory rrssb-VERSION which we then rename.
  // Delete existing files, including temporary ones if we failed part way through.
  $temp_library = "$destination/rrssb-" . RRSSB_LIBRARY_VERSION;
  $library = "$destination/rrssb";

  if (is_dir($library)) {
    if ($only_if_missing) {
      return;
    }
    drush_log(dt('Deleting existing RRSSB plugin'), 'notice');
    drush_delete_dir($library, TRUE);
  }
  drush_delete_dir($temp_library, TRUE);

  $zip = drush_download_file(RRSSB_LIBRARY_URI);
  if (!$zip) {
    return drush_set_error(dt('Failed to download @url', array('@url' => RRSSB_LIBRARY_URI)));
  }

  if (!drush_tarball_extract($zip, $destination)) {
    return drush_set_error(dt('Failed to extract to @dest', array('@dest' => $destination)));
  }
  unlink($zip);

  // Move to the correct location.
  drush_move_dir($temp_library, $library, TRUE);

  if (!file_exists("$library/js/rrssb.min.js")) {
    return(drush_set_error(dt('Drush was unable to download the RRSSB library to @path', array('@path' => $library))));
  }

  drush_log(dt('The RRSSB library has been downloaded to @path', array('@path' => $library)), 'success');
}