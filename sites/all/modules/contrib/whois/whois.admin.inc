<?php

/**
 * @file
 * Settings functions for the Whois module.
 */


function whois_settings() {
  $form = array();

  $form['whois_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic configuration'),
    '#collapsed' => TRUE,
  );
  $form['whois_settings']['whois_output_method'] = array(
    '#type' => 'radios',
    '#title' => t('Output method'),
    '#default_value' => variable_get('whois_output_method', 'html'),
    '#description' => t('The style in which the whois results are presented to the user.'),
    '#options' => array(
      'basic' => 'Basic',
      'html' => 'HTMLized',
      'html_status' => 'HTMLized status only',
      'object' => 'PHP object',
    ),
  );
  $form['whois_settings']['whois_enable_ajax'] = array(
    '#type' => 'checkbox',
    '#title' => t('Dynamic lookup (AJAX)'),
    '#default_value' => variable_get('whois_enable_ajax', 1),
    '#description' => t('Lookup and view the whois results dynamically (i.e. without reloading page) using AJAX request.'),
  );
  $form['whois_settings']['whois_hourly_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Hourly threshold'),
    '#options' => drupal_map_assoc(array(2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59)),
    '#default_value' => variable_get('whois_hourly_threshold', 13),
    '#description' => t('The maximum number of whois lookups a user can perform per hour.'),
  );
  $form['whois_log'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lookup log'),
    '#description' => t('Log whois lookups.'),
  );
  $form['whois_log']['whois_log_watchdog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log watchdog entry'),
    '#default_value' => variable_get('whois_log_watchdog', 1),
    '#description' => t('Log a watchdog entry for each whois lookup performed.'),
  );
  $form['whois_log']['whois_log_watchdog_cached'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log watchdog entry for cached domains also'),
    '#default_value' => variable_get('whois_log_watchdog_cached', 0),
    '#description' => t('Log a watchdog entry for each whois lookup performed even if it is cached.'),
  );
  $form['whois_cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache configuration'),
    '#collapsed' => TRUE,
  );
  $form['whois_cache']['whois_cache_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable cache'),
    '#default_value' => variable_get('whois_cache_enable', 1),
    '#description' => t('Enable the caching of whois records.'),
  );
  $form['whois_cache']['whois_cache_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache time'),
    '#default_value' => variable_get('whois_cache_time', 86400),
    '#description' => t('The length of time in seconds to cache whois lookup results. Set to "0" to never expire cached entries.'),
  );
  return system_settings_form($form);
}

