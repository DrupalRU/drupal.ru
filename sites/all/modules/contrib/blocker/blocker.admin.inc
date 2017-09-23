<?php

/**
 * Admin settings for blocker module.
 * 
 * @return type
 */
function blocker_admin_settings() {
  $theme = variable_get('theme_default', 'bartik');

  if (module_exists('block_class')) {
    $form['blocker_block_class_version'] = array(
      '#type' => 'textfield',
      '#title' => t('Block Class major version'),
      '#description' => t('The current version of the Block Class module. Some functionality depends on the version.'),
      '#default_value' => variable_get('blocker_block_class_version', blocker_block_class_version()),
      '#disabled' => TRUE,
    );
  }

  $form['blocker_regions'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available regions'),
    '#description' => t('Select the available regions for placing blocks in using Blocker module.'),
    '#options' => system_region_list($theme, REGIONS_VISIBLE),
    '#default_value' => variable_get('blocker_regions', blocker_regions()),
  );
  
  $options = blocker_blocks();
  unset($options['']);
  
  $form['blocker_blocks'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available blocks'),
    '#description' => t('Select the available blocks to put in regions with Blocker module.'),
    '#options' => $options,
    '#default_value' => variable_get('blocker_blocks', blocker_blocks()),
  );
  
  return system_settings_form($form);  
}

