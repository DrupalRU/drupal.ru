<?php

/**
 * @file
 * Contains list of admin forms and related logic.
 */

/**
 * Form builder for admin form.
 */
function drurum_admin_form($form, &$form_state) {
  $overrides = array();
  foreach (variable_get('drurum_actions_override') as $path => $name) {
    $overrides[] = $path.'|'.$name;
  }
  $form['#validate'][] = 'drurum_admin_form_validate';
  $form['drurum_actions_override'] = array(
    '#type'          => 'textarea',
    '#title'         => t('List of action names'),
    '#description'   => t('Here you can override title of action names using format "path|title" per each line. '
      . 'You need to use relative path like that "node/add/blog". Also, you should to know that each row it\'s separated rule.'),
    '#default_value' => implode(PHP_EOL, $overrides),
  );
  return system_settings_form($form);
}

/**
 * Form validate for drurum_admin_form().
 */
function drurum_admin_form_validate($form, &$form_state) {
  form_state_values_clean($form_state);
  $vals = &$form_state['values'];
  if (!empty($vals['drurum_actions_override'])) {
    $rules = explode(PHP_EOL, $vals['drurum_actions_override']);
    $bad_rows = array();
    $rows = array();
    // Validate rules. Each rule (row) must have pipeline.
    foreach ($rules as $row => $rule) {
      if (strpos($rule, '|') === FALSE) {
        $bad_rows[] = $row + 1;
      }
      else {
        $rule = explode('|', $rule);
        // If row has a few pipelines '|'.
        $rows[array_shift($rule)] = implode('|', $rule);
      }
    }

    // If some rows has no pipeline, show error message.
    if ($bad_rows) {
      form_set_error('drurum_actions_override', t('You have bad rules in next rows: @rows', array(
        '@rows' => implode(', ', $bad_rows),
      )));
    }
    else {
      $vals['drurum_actions_override'] = $rows;
    }
  }
}
