<?php

/**
 * @file
 * Contain logic for admin pages.
 */

/**
 * Form builder for admin form.
 */
function acal_admin_form($form, &$form_state) {
  $form['wrapper'] = [
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  ];
  $form['wrapper']['acal_collect'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable collection of statistic'),
    '#default_value' => variable_get('acal_collect'),
    '#description' => t('If the statistic collection will disabled, then graphs will show existing data only.'),
  ];
  $form['wrapper']['acal_clear'] = [
    '#type' => 'checkbox',
    '#title' => t('Clear old statistic'),
    '#default_value' => variable_get('acal_clear'),
    '#description' => t('Data older than one year will be deleted by cron, if enabled.'),
  ];
  $form['wrapper']['acal_color'] = [
    '#type' => 'textfield',
    '#title' => t('Color'),
    '#description' => t('Color which will be used for highlight cells. Format: "#000000"'),
    '#default_value' => variable_get('acal_color'),
  ];
  $form['wrapper']['acal_intervals'] = [
    '#type' => 'textfield',
    '#title' => t('Intervals'),
    '#description' => t('Intervals separated by comma, which will be used for calculate color based on activity.'),
    '#default_value' => variable_get('acal_intervals'),
  ];
  $form['wrapper']['collecting'] = [
    '#type' => 'fieldset',
    '#title' => t('Collect statistic'),
    '#desscription' => t('Statistics will be collected for next entities.'),
  ];


  if (module_exists('comment')) {
    $form['wrapper']['collecting']['acal_comment'] = [
      '#type' => 'checkbox',
      '#title' => t('Comment'),
      '#default_value' => variable_get('acal_comment'),
    ];
    $form['wrapper']['collecting']['acal_comment_coeff'] = [
      '#type' => 'textfield',
      '#title' => t('Comment coefficient'),
      '#description' => t('Coefficient which will be used in calculation of comments activity points.'),
      '#default_value' => variable_get('acal_comment_coeff'),
      '#states' => [
        'invisible' => [
          ':input[name="acal_comment"]' => ['checked' => FALSE],
        ],
      ],
    ];
  }

  $form['wrapper']['collecting']['acal_node'] = [
    '#type' => 'checkbox',
    '#title' => t('Node'),
    '#default_value' => variable_get('acal_node'),
  ];
  $form['wrapper']['collecting']['acal_node_coeff'] = [
    '#type' => 'textfield',
    '#title' => t('Node coefficient'),
    '#description' => t('Coefficient which will be used in calculation of node activity points.'),
    '#default_value' => variable_get('acal_node_coeff'),
    '#states' => [
      'invisible' => [
        ':input[name="acal_node"]' => ['checked' => FALSE],
      ],
    ],
  ];

  $form['wrapper']['node_types'] = [
    '#type' => 'fieldset',
    '#title' => t('Node types'),
    '#desscription' => t('Activity will be collected in next node types. This settings will be applicable to comments as well.'),
  ];
  foreach (node_type_get_names() as $type => $name) {
    $form['wrapper']['node_types']['acal_node_type_' . $type] = [
      '#type' => 'checkbox',
      '#title' => $name,
      '#default_value' => variable_get('acal_node_type_' . $type),
    ];
  }

  return system_settings_form($form);
}

/**
 * Form validate for admin form.
 */
function acal_admin_form_validate($form, &$form_state) {
  foreach (['acal_node_coeff', 'acal_comment_coeff'] as $field) {
    if (!is_numeric($form_state['values'][$field])) {
      form_set_error($field, t('The field must be numeric.'));
    }
  }
}
