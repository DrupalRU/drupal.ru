<?php

/**
 * Ping module admin functions.
 */

function ping_settings() {
  $form = array();

  $form['ping_response'] = array(
    '#type' => 'textfield',
    '#title' => t('Response message'),
    '#default_value' => variable_get('ping_response', PING_DEFAULT_RESPONSE),
    '#description' => t('The message returned when the server is pinged.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
