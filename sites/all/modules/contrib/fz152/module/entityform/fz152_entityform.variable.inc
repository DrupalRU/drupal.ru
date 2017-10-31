<?php

/**
 * @file
 * Variable module hooks.
 */

/**
 * Implements hook_variable_group_info().
 */
function fz152_entityform_variable_group_info() {
  $groups['fz152_entityform'] = array(
    'title' => 'FZ152 Entityforms',
    'description' => t('FZ152 Entityform settings'),
    'access' => 'administer site configuration',
  );

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function fz152_entityform_variable_info($options) {
  $variables = array();
  $entityform_forms = fz152_entityform_get_entityform_bundle_info();

  if (!empty($entityform_forms)) {
    foreach ($entityform_forms as $form) {
      $variables['fz152_entityform_form_' . $form['name'] . '_enable'] = array(
        'type' => 'boolean',
        'title' => $form['label'],
        'default' => FALSE,
        'group' => 'fz152_entityform',
        'localize' => TRUE,
      );

      $variables['fz152_entityform_form_' . $form['name'] . '_weight'] = array(
        'type' => 'number',
        'title' => $form['label'] . ': checkbox weight',
        'default' => '',
        'group' => 'fz152_entityform',
        'localize' => TRUE,
      );
    }
  }

  return $variables;
}
