<?php
  
function drupal_deploy_export_taxonomy($filename = ''){
  $vocabulary_name = drush_get_option('name', FALSE);
  $items = drupal_deploy_export_taxonomy_fetch_data($vocabulary_name);
  
   if(empty($filename)){
    $filename = 'taxonomy.export';
    
    if($vocabulary_name){
      $filename = $vocabulary_name . '.taxonomy.export';  
    }
  }
  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_taxonomy($filename = ''){
  $vocabulary_name = drush_get_option('name', FALSE);
  
  if(empty($filename)){
    $filename = 'taxonomy.export';
    if($vocabulary_name){
      $filename = $vocabulary_name . '.' . 'taxonomy.export';
    }
  }
  if(!is_file($filename)){
    drush_log(dt('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drush_log(dt('Json decode error!'), 'error');
  }
  drupal_deploy_import_taxonomy_items($data, $vocabulary_name);
}

function drupal_deploy_import_taxonomy_items($data, $vocabulary_name = ''){
  foreach($data as $item){
    if(!empty($vocabulary_name))
    {
      if($item['name'] != $vocabulary_name)
      {
        continue;
      }
    }
    
    $fields = array();
    if(!empty($item['fields'])){
      $fields['fields'] = $item['fields'];
      $fields['instances'] = $item['instances'];
    }
    unset($item['fields']);
    unset($item['instances']);    
    
    $terms = $item['terms'];
    unset($item['terms']);

    $stored_vocabulary = taxonomy_vocabulary_machine_name_load($item['machine_name']);
    
    if($stored_vocabulary){
      $item['vid'] = $stored_vocabulary->vid;
    }
    
    $vocabulary = (object) $item;
    
    $status = taxonomy_vocabulary_save($vocabulary);
    if($status == SAVED_UPDATED || $status == SAVED_NEW)
    {
      drush_log(dt('Vocabulary !name imported',array('!name' => $vocabulary->name)), 'ok');
      ddi_fields($fields, 'taxonomy_term', $vocabulary->machine_name);

      if(is_array($terms)){
        foreach($terms as $term)
        {
          $term['vid'] = $vocabulary->vid;
          $status = drupal_deploy_save_term($term, $vocabulary->vid);
          if($status == SAVED_UPDATED || $status == SAVED_NEW){
            drush_log(dt('Term !name imported',array('!name' => $term['name'])), 'ok');
          }
        }
      }else{
        drush_log(dt('Vocabulary !name does not have terms',array('!name' => $vocabulary->name)), 'warning');
      }
    }
    
  }
}

function drupal_deploy_save_term($term, $vid, $parent = 0){
  $term['vid'] = $vid;
  $term['parent'] = $parent;
  if(is_array($term['synonyms'])){
    $term['synonyms'] = implode("\n", $term['synonyms']);
  }
  if(isset($term['fields'])){
    foreach($term['fields'] as $name => $value){
      $term[$name] = $value;
    }
    unset($term['fields']);
  }
  
  $query = db_select('taxonomy_term_data', 'td');
  $query->join('taxonomy_term_hierarchy', 'th', 'th.tid = td.tid');
  $query->condition('td.name', $term['name'])
    ->condition('td.vid', $vid)
    ->condition('th.parent', $parent)   
    ->fields('td')
    ->fields('th', array('parent'));
  $stored_term = $query->execute()->fetchObject();
  
  if($stored_term){
    $term['tid'] = $stored_term->tid;
  }
  
  if(isset($term['path_alias'])){
    $term['pathauto_perform_alias'] = FALSE;
  }
  $subterms = $term['terms'];
  unset($term['terms']);
  $term = (object) $term;
  $status = taxonomy_term_save($term);
  if($status == SAVED_UPDATED || $status == SAVED_NEW)
  {
    if(is_array($subterms))
    foreach($subterms as $sub_term)
    {
      $status = drupal_deploy_save_term($sub_term, $vid, $term->tid);
      if($status == SAVED_UPDATED || $status == SAVED_NEW){
        drush_log(dt('Term !name imported',array('!name' => $sub_term['name'])), 'ok');
      }
    }
  }
  return $status;
}

function drupal_deploy_export_taxonomy_fetch_data($vocabulary_name = ''){
  $items = array();
  
  $query = db_select('taxonomy_vocabulary', 'tv')
    ->fields('tv');
  
  if(!empty($vocabulary_name)){
    $query->condition('tv.machine_name', $vocabulary_name);
  }

  $query->orderBy('tv.name');
  
  $vocabularies = $query->execute()->fetchAll();
  
  foreach($vocabularies as $db_item){
  
    $vocabulary = taxonomy_vocabulary_machine_name_load($db_item->machine_name);
    $result = dde_fields('taxonomy_term', $vocabulary->machine_name);
    
    $vocabulary->fields = $result['fields'];
    $vocabulary->instances = $result['instances'];
    
    $vocabulary->terms = dde_terms($vocabulary->vid);
    unset($vocabulary->vid);
    $items[] = $vocabulary;    
  }
  
  return $items;
}  

function dde_terms($vid){
  $query = db_select('taxonomy_term_data', 'td');
  $query->join('taxonomy_term_hierarchy', 'th', 'th.tid = td.tid');
  $query->condition('td.vid', $vid)
    ->fields('td')
    ->fields('th', array('parent'))
    ->orderBy('td.name');
  $terms_result = $query->execute()->fetchAll();

  foreach($terms_result as $term){    
    $term_synonym_result = db_select('taxonomy_term_synonym', 'ts')
      ->fields('ts')
      ->condition('ts.tid', $term->tid)
      ->orderBy('ts.name')
      ->execute()
      ->fetchAll();
      foreach($term_synonym_result as $term_synonym){
        $term->synonyms[] = $term_synonym->name;
      }
      if($fields_data = dde_get_fields_data($term->tid)){
        $term->fields = $fields_data;
      }
      $terms[$term->tid] = (array)$term;
  }
  $tree = array();
  foreach($terms as $term){
    if($term['parent'] == 0){
      $term['terms'] = _get_term_children($term, $terms);
      $store_term = array(
        'name' => $term['name'],
        'description' => $term['description'],
        'weight' => $term['weight'],
      );
      
      if(!empty($term['fields'])){
        $store_term['fields'] = $term['fields'];
      }
      if ($alias = drupal_lookup_path('alias', 'taxonomy/term/' . $term['tid'], $path_language)) {
        $store_term['path_alias'] = $alias;
      }
      if(!empty($term['synonyms'])){
        $store_term['synonyms'] = $term['synonyms'];
      }
      if(!empty($term['terms'])){
        $store_term['terms'] = $term['terms'];
      }
      $tree[] = $store_term;
    }
  }
  return $tree;
}

function dde_get_fields_data($tid){
  $fields_data = array();
  $stored_term = taxonomy_term_load($tid);
  foreach($stored_term as $name => $value){
    if(0 == strncmp($name, "field_", 6)){
      $fields_data[$name] =  $value;
    }
  }
  return $fields_data;
}

function _get_term_children($parent_term, $terms){
  $return = array();
  foreach($terms as $term){
    if($term['parent'] == $parent_term['tid']){
      $term['terms'] = _get_term_children($term, $terms);
      $store_term = array(
        'name' => $term['name'],
        'description' => $term['description'],
        'weight' => $term['weight'],
      );
      
      if($fields_data = dde_get_fields_data($term['tid'])){
        $store_term['fields'] = $fields_data;
      }
      
      if ($alias = drupal_lookup_path('alias', 'taxonomy/term/' . $term['tid'], $path_language)) {
        $store_term['path_alias'] = $alias;
      }
      
      if(!empty($term['synonyms'])){
          $store_term['synonyms'] = $term['synonyms'];
        }
      if(!empty($term['terms'])){
        $store_term['terms'] = $term['terms'];
      }
      $return[] = $store_term;
    }
  }
  return $return;
}