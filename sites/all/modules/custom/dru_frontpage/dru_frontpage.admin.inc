<?php

/**
 * @file
 * Administer module dru_frontpage.module.
 */

/**
 *
 */
function dru_frontpage_admin_settings($form, &$form_state) {
  $form['dru_frontpage_max_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum items to display'),
    '#default_value' => variable_get('dru_frontpage_max_count', 40),
  );

  $options = node_type_get_names();
  $form['dru_frontpage_node_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display selected node types'),
    '#description' => t('If none selected, will display all types'),
    '#default_value' => variable_get('dru_frontpage_node_type'),
    '#options' => $options,
  );

  return system_settings_form($form);
}
