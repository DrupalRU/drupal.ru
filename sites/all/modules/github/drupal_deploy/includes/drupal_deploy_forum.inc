<?php
  
function drupal_deploy_export_forum($filename = ''){
  $items = drupal_deploy_export_forum_fetch_data($vocabulary_name);
  
  if(empty($filename)){
    $filename = 'forum.export';
  }
  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_forum($filename = ''){
  
  if(empty($filename)){
    $filename = 'forum.export';
  }
  if(!is_file($filename)){
    drupal_set_message(t('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drupal_set_message(t('Json decode error!'), 'error');
  }
  drupal_deploy_import_forum_items($data);
}


function drupal_deploy_export_forum_fetch_data(){
  $items = array();
  
  $terms = variable_get('forum_containers', FALSE);
  $vocabulary_id = variable_get('forum_nav_vocabulary', FALSE);
  
  $stored_vocabulary = taxonomy_vocabulary_load($vocabulary_id);
  
  $items['vocabulary'] = $stored_vocabulary->name;
  
  $containers = db_select('taxonomy_term_data', 'td')
    ->fields('td')
    ->condition('td.tid', $terms, 'IN')
    ->execute()
    ->fetchAll();
    
  foreach($containers as $db_item){
    $item = array(
      'name' => $db_item->name,
    );
    if ($alias = drupal_lookup_path('alias', 'forum/' . $db_item->tid, $path_language)) {
      $item['path_alias'] = $alias;
    }
    if($sub_terms = dde_forum_terms($db_item->tid)){
      $item['terms'] = $sub_terms;
    }

    $items['terms'][] = $item;    
  }
  
  return $items;
}

function dde_forum_terms($parent_id){
  $items = array();
  $query = db_select('taxonomy_term_data', 'td');
  $query->join('taxonomy_term_hierarchy', 'th', 'th.tid = td.tid');
  $query->condition('th.parent', $parent_id)
    ->fields('td');
    
  $terms_result = $query->execute()->fetchAll();

  foreach($terms_result as $db_item){
    $item = array(
      'name' => $db_item->name,
    );
    if ($alias = path_load('forum/' . $db_item->tid)) {
      $item['path_alias'] = $alias['alias'];
    }
    if($sub_terms = dde_forum_terms($db_item->tid)){
      $item['terms'] = $sub_terms;
    }

    $items[] = $item;    
  }
  return $items;
}

function drupal_deploy_import_forum_items($data){
  $forum_containers = array();
  $vocabulary = $data['vocabulary'];
  
  $stored_vocabulary = db_select('taxonomy_vocabulary', 'tv')
    ->fields('tv')
    ->condition('tv.name', $data['vocabulary'])
    ->execute()
    ->fetchObject();
    
  if($stored_vocabulary){
    foreach($data['terms'] as $item){
      $stored_term = db_select('taxonomy_term_data', 'td')
        ->fields('td')
        ->condition('td.name', $item['name'])
        ->condition('td.vid', $stored_vocabulary->vid)
        ->execute()
        ->fetchObject();
      if($stored_term){
        $forum_containers[] = $stored_term->tid;
        drupal_set_message(t('Container !name imported',array('!name' => $stored_term->name)));
        if(isset($item['path_alias'])){
          
          $path = array(
            'alias' => $item['path_alias'],
            'source' => 'forum/' . $stored_term->tid,
            'language' => LANGUAGE_NONE
          );
          if ($alias = path_load('forum/' . $stored_term->tid)) {
            $path['pid'] = $alias['pid'];
            $path['origin'] = $alias['alias'];
          }
          path_save($path);
        }
        if($item['terms']){
          ddi_forum_terms($item['terms'], $stored_term->tid);
        }
      }
    }
    variable_set('forum_containers', $forum_containers);
    variable_set('forum_nav_vocabulary', $stored_vocabulary->vid);
  }
}

function ddi_forum_terms($terms, $parent_id){
  foreach($terms as $item){
    $query = db_select('taxonomy_term_data', 'td');
    $query->join('taxonomy_term_hierarchy', 'th', 'th.tid = td.tid');
    $query->condition('th.parent', $parent_id)
      ->condition('td.name', $item['name'])
      ->fields('td');
      
    $stored_term = $query->execute()->fetchObject();
    if($stored_term){
      if(isset($item['path_alias'])){
        $path = array(
          'alias' => $item['path_alias'],
          'source' => 'forum/' . $stored_term->tid,
          'language' => LANGUAGE_NONE
        );
        if ($alias = path_load('forum/' . $stored_term->tid)) {
          $path['pid'] = $alias['pid'];
          $path['origin'] = $alias['alias'];
        }
        path_save($path);
      }
      if($item['terms']){
        ddi_forum_terms($item['terms'], $stored_term->tid);
      }
    }
  }
}
