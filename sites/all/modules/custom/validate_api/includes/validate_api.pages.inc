<?php

/**
 * @file
 * Administer pages
 */

/**
 * Main administer page
 */
function validate_api_form_settings($form, &$form_state) {
  if ($configs = validate_api_get_current_configs()) {
    $form['container'] = [
      '#type' => 'vertical_tabs',
      '#tree' => true,
    ];

    $form['container']['settings'] = [
      '#type' => 'fieldset',
      '#collapsible' => false,
      '#title' => t('Validator settings'),
      '#group' => 'group_tabs',
    ];

    foreach ($configs as $name => $config) {
      $entities = entity_get_info('');

      $type = 'node';
      $bundle_name = substr($name, strlen('validate_api_'));

      $comment_len = strlen('_comment');
      if ($comment = (substr($bundle_name, -$comment_len) == '_comment')) {
        $type = 'comment';
        $bundle_name = 'comment_node_' . substr($bundle_name, 0, -$comment_len);
      }

      $entity_type = $entities[$type]['bundles'][$bundle_name];

      $form['container']['settings']['description'][$type] = [
        '#type' => 'fieldset',
        '#collapsible' => true,
        '#collapsed' => false,
        '#title' => t('@label validators', ['@label' => $entity_type['label']]),
      ];

      $rows = [];
      foreach ($config['validate'] as $field_name => $validators) {
        $field = field_info_instance($type, $field_name, $bundle_name);

        $rows[] = [
          $field_name == 'title' ? t('Title') : $field['label'],
          $field_name == 'title' ? t('Title property') : $field['widget']['type'],
          implode(', ', $validators),
          l('Edit', $entity_type['admin']['real path']),
        ];
      }

      $form['container']['settings']['description'][$type]['fields'] = [
        '#theme' => 'table',
        '#header' => [t('Field'), t('Field type'), t('Enabled validators'), t('Edit')],
        '#rows' => $rows,
      ];
    }


    $form['actions'] = [
      '#type' => 'actions',
      'submit' => [
        '#type' => 'submit',
        '#value' => t('Save'),
      ],
    ];
  }
  else {
    $form = [
      '#type' => 'container',
      'no_validators' => [
        '#type' => 'html_tag',
        '#tag' => 'h2',
        '#value' => t('There are no any validators yet'),
        '#attributes' => [
          'style' => 'text-align: center',
        ],
      ],
      'description' => [
        '#type' => 'html_tag',
        '#tag' => 'p',
        '#value' => t('You should enable Validate API for some !url firstly.', [
          '!url' => l(t('content type'), 'admin/structure/types'),
        ]),
        '#attributes' => [
          'style' => 'text-align: center',
        ],
      ],
    ];
  }

  return $form;
}

/**
 * Main administer page
 */
function validate_api_form_settings_submit($form, &$form_state) {
  drupal_set_message(t('Settings saved.'));
}

/**
 * Get all Validate active configs.
 *
 * @return array
 */
function validate_api_get_current_configs() {
  $q = db_select('variable', 'v')
    ->fields('v')
    ->condition('name', db_like('validate_api_') . '%', 'LIKE')
    ->execute();

  $configs = $q->fetchAllAssoc('name');

  return array_filter(array_map(function ($item) {
    return $item->value = isset($item->value) ? unserialize($item->value) : null;
  }, $configs), function ($config) {
    return ($config['settings'] ?? null);
  });
}
