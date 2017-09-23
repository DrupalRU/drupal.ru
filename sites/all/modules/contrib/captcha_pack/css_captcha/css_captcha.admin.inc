<?php

/**
 * @file
 * Functionality and helper functions for CSS CAPTCHA administration.
 */

/**
 * Function for the settings form
 */
function css_captcha_settings_form() {
  $form = array();
  $form['css_captcha_allowed_characters'] = array(
    '#type' => 'textfield',
    '#title' => t('Characters to use in the code'),
    '#default_value' => variable_get('css_captcha_allowed_characters', CSS_CAPTCHA_DEFAULT_ALLOWED_CHARACTERS),
  );
  $form['css_captcha_code_length'] = array(
    '#type' => 'select',
    '#title' => t('Code length'),
    '#options' => drupal_map_assoc(array(4, 5, 6, 7, 8, 9, 10)),
    '#default_value' => variable_get('css_captcha_code_length', 6),
  );
  $form['#validate'][] = 'css_captcha_settings_form_validate';
  return system_settings_form($form);
}

/**
 * Validation function
 */
function css_captcha_settings_form_validate($form, &$form_state) {
  if (preg_match('/\s/', $form_state['values']['css_captcha_allowed_characters'])) {
    form_set_error('css_captcha_allowed_characters', t('The list of characters to use should not contain spaces.'));
  }
}
