<?php
/**
 * @file
 * Administer module alttracker.module.
 */

function alttracker_admin_settings($form, &$form_state) {
  $form['alttracker_max_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum items to display'),
    '#default_value' => variable_get('alttracker_max_count', 1000),
  );

  $form['alttracker_taxonomy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display taxonomy'),
    '#default_value' => variable_get('alttracker_taxonomy'),
  );

  $node_types = node_type_get_names();
  $alttracker_node_type = variable_get('alttracker_node_type');
  $form['alttracker_node_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display selected node types'),
    '#description' => t('If none selected, will display all types'),
    '#default_value' => $alttracker_node_type,
    '#options' => $node_types,
  );

  if ($node_types) {
    $form['alttracker_node_type_fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Fields to compare'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['alttracker_node_type_properties'] = array(
      '#type' => 'fieldset',
      '#title' => t('Properties to compare'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    foreach ($node_types as $type => $node_type_name) {
      // Fields
      $form['alttracker_node_type_fields'][$type] = array(
        '#type' => 'fieldset',
        '#title' => $node_type_name,
        '#collapsible' => TRUE,
        '#collapsed' => !(isset($alttracker_node_type[$type]) && $alttracker_node_type[$type]),
        '#states' => array(
          'visible' => array(
            "input[name=\"alttracker_node_type[{$type}]\"]" => array(
              'checked' => TRUE,
            ),
          ),
        ),
      );

      $form_state['node_type_fields'][$type] = _alttracker_admin_get_node_fields($type);

      $fields = variable_get("alttracker_node_type_fields_{$type}", array());
      foreach ($form_state['node_type_fields'][$type] as $field_name => $field) {
        $disabled = !in_array($field['type'], array_keys(_alttracker_field_types()));

        $form['alttracker_node_type_fields'][$type][$field_name] = array(
          '#type' => 'checkbox',
          '#title' => $field['label'] ?: $type,
          '#default_value' => isset($fields[$field_name]),
          '#disabled' => $disabled,
        );

        if ($disabled) {
          $form['alttracker_node_type_fields'][$type][$field_name]['#description'] = t(
            'Field-type "%field" not supported by alttracker comparing. Please, see %func function.',
            array('%field' => $field['type'], '%func' => '_alttracker_field_types()')
          );
        }
      }

      // Properties
      $form['alttracker_node_type_properties'][$type] = array(
        '#type' => 'fieldset',
        '#title' => $node_type_name,
        '#collapsible' => TRUE,
        '#collapsed' => !(isset($alttracker_node_type[$type]) && $alttracker_node_type[$type]),
        '#states' => array(
          'visible' => array(
            "input[name=\"alttracker_node_type[{$type}]\"]" => array(
              'checked' => TRUE,
            ),
          ),
        ),
      );

      $form_state['node_type_properties'][$type] = _alttracker_get_node_properties();

      $properties = variable_get("alttracker_node_type_properties_{$type}", array());
      foreach ($form_state['node_type_properties'][$type] as $field_name => $field) {
        $form['alttracker_node_type_properties'][$type][$field] = array(
          '#type' => 'checkbox',
          '#title' => drupal_ucfirst($field),
          '#default_value' => isset($properties[$field]),
        );
      }
    }
  }

  $form['alttracker_vocabulary'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ignore terms'),
    '#description' => t('Select taxonomy terms to ignore in tracker outout.')
  );
  $vocabularies = taxonomy_vocabulary_get_names();
  
  foreach($vocabularies as $vocabulary){
    $form['alttracker_vocabulary']['alttracker_vocabulary_' . $vocabulary->vid] = array(
      '#type' => 'textfield',
      '#title' => $vocabulary->name,
      '#autocomplete_path' => 'alttracker/autocomplete/' . $vocabulary->vid,
      '#default_value' => variable_get('alttracker_vocabulary_' . $vocabulary->vid),
      '#description' => t('Comma or space separated'),
      '#size' => 100,
      '#maxlength' => 500,
    );
  }
  $form['#validate'][] = 'alttracker_admin_settings_submit';
  return system_settings_form($form);
}

function alttracker_admin_settings_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Save fields data
  if ($values['alttracker_node_type_fields']) {
    foreach ($values['alttracker_node_type_fields'] as $node_type => $fields) {
      $node_fields = $values['alttracker_node_type'][$node_type] ? array_filter(
        $fields, function ($v) {
          return $v;
        }) : array();

      $set = array_map(function ($val) use ($form_state, $node_type) {
        return $form_state['node_type_fields'][$node_type][$val]['type'];
      }, array_combine(array_keys($node_fields), array_keys($node_fields)));

      variable_set("alttracker_node_type_fields_{$node_type}", $set);
    }
  }

  // Save properties data
  if ($values['alttracker_node_type_properties']) {
    foreach ($values['alttracker_node_type_properties'] as $node_type => $fields) {
      variable_set("alttracker_node_type_properties_{$node_type}", array_filter($fields));
    }
  }
  
  $vocabularies = taxonomy_vocabulary_get_names();
  
  $terms = array();
  foreach($vocabularies as $vocabulary){
    if(!empty($values['alttracker_vocabulary_' . $vocabulary->vid])){
      $tags_typed = drupal_explode_tags($values['alttracker_vocabulary_' . $vocabulary->vid]);
      $result = db_select('taxonomy_term_data', 't')
      ->fields('t', array('tid', 'name'))
      ->condition('vid', $vocabulary->vid, '=')
      ->condition('name', $tags_typed , 'IN')
      ->execute()
      ->fetchAll();

      if($result){
        foreach($result as $term){
          $terms[] = $term->tid;
        }
      }
    }
  }
  
  variable_set('alttracker_ignore_terms', $terms);
}

function alttracker_autocomplete_callback($vid, $str){
  $matches = array();
  $tags_typed = drupal_explode_tags($str);
  $tag_last = drupal_strtolower(array_pop($tags_typed));
  
  $result = db_select('taxonomy_term_data', 't')
    ->fields('t', array('tid', 'name'))
    ->condition('vid', $vid, '=')
    ->condition('name', $tag_last.'%%', 'LIKE')
    ->range(0, 10)
    ->execute();
  $prefix = count($tags_typed) ? drupal_implode_tags($tags_typed) . ', ' : '';
  
  foreach ($result as $term) {
    $n = $term->name;
    // Term names containing commas or quotes must be wrapped in quotes.
    if (strpos($term->name, ',') !== FALSE || strpos($term->name, '"') !== FALSE) {
      $n = '"' . str_replace('"', '""', $term->name) . '"';
    }
    $matches[$prefix . $n] = check_plain($term->name);
  }
  drupal_json_output($matches);
}

/**
 * Get fields with type.
 *
 * @param string $node_type
 *
 * @return array
 */
function _alttracker_admin_get_node_fields($node_type) {
  $results = array();

  if ($fields = field_info_instances('node', $node_type)) {
    $types = db_select('field_config', 'c')
      ->fields('c', array('field_name', 'type', 'module'))
      ->condition('c.field_name', array_keys($fields), 'IN')
      ->execute()
      ->fetchAllAssoc('field_name');

    foreach ($fields as $field_type => $field) {
      $results[$field_type] = array(
        'label' => $field['label'],
      );
    }

    $results = array_merge_recursive($results, $types);
  }

  return $results;
}
