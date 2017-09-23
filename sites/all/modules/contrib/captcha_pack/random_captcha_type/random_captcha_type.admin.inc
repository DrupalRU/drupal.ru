<?php

/**
 * @file
 * Functionality and helper functions for RANDOM CAPTCHA administration.
 */

/**
 * Configuration form
 */
function random_captcha_type_settings_form() {
  $enabled_types = _random_captcha_type_get_enabled_types();
  if (count($enabled_types) < 2) {
    drupal_set_message(t('You need at least two CAPTCHA types (other than %random_captcha_type).', array('%random_captcha_type' => 'Random CAPTCHA type')), 'error');
  }
  $form = array();
  $captcha_types = _random_captcha_type_get_all_types();
  $form['random_captcha_type_enabled_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Randomly switch between'),
    '#options' => $captcha_types,
    '#default_value' => $enabled_types,
  );
  return system_settings_form($form);
}

/**
 * Implementation of form validation
 */
function random_captcha_type_settings_form_validate($form, &$form_state) {
  // At least 2 CAPTCHA types should be selected
  if (count(array_filter($form_state['values']['random_captcha_type_enabled_types'])) < 2) {
    form_set_error('random_captcha_type_enabled_types', t('You should select at least two CAPTCHA types.'));
  }
}
