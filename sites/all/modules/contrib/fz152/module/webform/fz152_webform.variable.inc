<?php

/**
 * @file
 * Variable module hooks.
 */

/**
 * Implements hook_variable_group_info().
 */
function fz152_webform_variable_group_info() {
  $groups['fz152_webform'] = array(
    'title' => 'FZ152 Webforms',
    'description' => t('FZ152 Webform settings'),
    'access' => 'administer site configuration',
  );

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function fz152_webform_variable_info($options) {
  $variables = array();
  $webform_forms = fz152_webform_get_webform_info();

  if (!empty($webform_forms)) {
    foreach ($webform_forms as $form) {
      $variables['fz152_webform_form_' . $form['id'] . '_enable'] = array(
        'type' => 'boolean',
        'title' => $form['label'],
        'default' => FALSE,
        'group' => 'fz152_webform',
        'localize' => TRUE,
      );
    }
  }

  return $variables;
}
