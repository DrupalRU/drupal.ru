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
  
  $ignore_terms = variable_get('alttracker_ignore_terms');
  $form['alttracker_taxonomy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display taxonomy'),
    '#default_value' => variable_get('alttracker_taxonomy'),
  );

  $options = node_type_get_names();
  $form['alttracker_node_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display selected node types'),
    '#description' => t('If none selected, will display all types'),
    '#default_value' => variable_get('alttracker_node_type'),
    '#options' => $options,
  );

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
  
  $vocabularies = taxonomy_vocabulary_get_names();
  
  $terms = array();
  foreach($vocabularies as $vocabulary){
    if(!empty($values['alttracker_vocabulary_' . $vocabulary->vid])){
      
      $tags_typed = drupal_explode_tags($values['alttracker_vocabulary_' . $vocabulary->vid]);
      $result = db_select('taxonomy_term_data', 't')
      -> fields('t', array('tid', 'name'))
      -> condition('vid', $vocabulary->vid, '=')
      -> condition('name', $tags_typed , 'IN')
      -> execute()
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
    -> fields('t', array('tid', 'name'))
    -> condition('vid', $vid, '=')
    -> condition('name', $tag_last.'%%', 'LIKE')
    -> range(0, 10)
    -> execute();
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
