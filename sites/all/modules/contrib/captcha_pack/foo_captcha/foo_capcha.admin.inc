<?php

/**
 * @file
 * Functionality and helper functions for FOO CAPTCHA administration.
 */

/**
 * Function for the settings form
 */
function foo_captcha_settings_form() {
  $form = array();
  $form['foo_captcha_ignore_spaces'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore spaces in the response'),
    '#default_value' => variable_get('foo_captcha_ignore_spaces', FALSE),
  );
  return system_settings_form($form);
}