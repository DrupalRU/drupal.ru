<?php

/**
 * @file
 * Functionality and helper functions for ASCII ART CAPTCHA administration.
 */

/**
 * Function for the settings form
 */
function ascii_art_captcha_settings_form() {
  $form = array();
  $available_fonts = _ascii_art_captcha_available_fonts();
  $form['ascii_art_captcha_code_length'] = array(
    '#type' => 'select',
    '#title' => t('Code length'),
    '#options' => drupal_map_assoc(array(4, 5, 6, 7, 8, 9, 10)),
    '#default_value' => variable_get('ascii_art_captcha_code_length', 6),
  );
  $form['ascii_art_captcha_font'] = array(
    '#type' => 'select',
    '#title' => t('Font'),
    '#options' => $available_fonts,
    '#default_value' => variable_get('ascii_art_captcha_font', 'standard'),
    '#description' => t('Define the ASCII art font to use. Note that some characters are not very recognizable in some (small/weird) fonts. Make sure to disable the right character sets in these cases.'),
  );
  // font size
  $font_sizes = array(0 => t('default'));
  foreach (array(4, 6, 8, 9, 10, 11, 12) as $pt) {
    $font_sizes[$pt] = $pt . 'pt';
  }
  $form['ascii_art_captcha_font_size'] = array(
    '#type' => 'select',
    '#title' => t('Font size'),
    '#options' => $font_sizes,
    '#default_value' => variable_get('ascii_art_captcha_font_size', 0),
    '#description' => t('Set the font size for the ASCII art.'),
  );
  $form['ascii_art_captcha_allowed_characters'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Character sets to use'),
    '#options' => array(
      'upper' => t('upper case characters'),
      'lower' => t('lower case characters'),
      'digit' => t('digits'),
    ),
    '#default_value' => variable_get('ascii_art_captcha_allowed_characters', drupal_map_assoc(array('upper', 'lower', 'digit'))),
    '#description' => t('Enable the character sets to use in the code. Choose wisely by taking the recognizability of the used font into account.'),
  );
  $form['#validate'][] = 'ascii_art_captcha_settings_form_validate';
  return system_settings_form($form);
}

/**
 * Validation function for the settings
 */
function ascii_art_captcha_settings_form_validate($form, &$form_state) {
  if (count(array_filter($form_state['values']['ascii_art_captcha_allowed_characters'])) < 1) {
    form_set_error('ascii_art_captcha_allowed_characters', t('You should select at least one type of characters to use.'));
  }
}

/**
 * Function to get a list of available fonts
 */
function _ascii_art_captcha_available_fonts() {
  $available_fonts = array();
  $fontsdirectory = drupal_get_path('module', 'ascii_art_captcha') . '/fonts';
  $pattern = '/ascii_art_captcha_font_([a-zA-Z0-9]+)\.[iI][nN][cC]$/';
  $directory_files = file_scan_directory($fontsdirectory, $pattern);
  foreach ($directory_files as $font) {
    $regs = array();
    preg_match($pattern, $font->filename, $regs);
    $available_fonts[$regs[1]] = $regs[1];
  }
  asort($available_fonts);
  return $available_fonts;
}
