<?php
  
function drupal_deploy_export_variables($filename = ''){

  $name = drush_get_option('name', FALSE);

  if(strpos($name, ',')){
    $names = explode(",", $name);
    $items = drupal_deploy_export_variables_fetch_data($names);
  }else{
    $items = drupal_deploy_export_variables_fetch_data($name);  
  }

  if(empty($filename)){
    $filename = 'variables.export';
    if($name){
      $name = str_replace("_%", "", $name);
      $name = str_replace("%", "", $name);
      $filename = $name . '.variables.export';  
    }
    if($names){
      $name_for_file = str_replace("_%", "", $name);
      $name_for_file = str_replace("%", "", $name_for_file);
      $name_for_file = str_replace(",", ".", $name_for_file);
      $filename = $name_for_file . '.variables.export';
    }
  }
  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_variables($filename = ''){
  
  if(empty($filename)){
    $filename = 'variables.export';
  }
  if(!is_file($filename)){
    drupal_set_message(t('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drupal_set_message(t('Json decode error!'), 'error');
  }
  drupal_deploy_import_variables_items($data);
}


function drupal_deploy_export_variables_fetch_data($name = FALSE){
  $items = array();
  
  $query = db_select('variable', 'v')
    ->fields('v');
  
  if(is_string($name)){
    echo "name:" . $name ."\n";
    $query->condition('v.name', $name, 'LIKE');
  }
  
  $query->orderBy('v.name');
  $variables = $query->execute()->fetchAll();
  
  foreach($variables as $db_item){

    $db_item->value = unserialize($db_item->value);
    $items[] = $db_item;
  }
  return $items;
}

function drupal_deploy_import_variables_items($data){
  $name = drush_get_option('name', FALSE);

  if(strpos($name, ',')){
    $name = explode(",", $name);
  }
  
  foreach($data as $item){
    if($name){
      if(is_string($name)){
        if(strpos($name, '%')){
          ddi_import_mask($name, $item);
        }else{
          variable_set($item['name'],$item['value']);
        }      
      }
      if(is_array($name)){
        foreach($name as $var){
          if(strpos($var, '%')){
            ddi_import_mask($var, $item);
          }else{
            variable_set($item['name'],$item['value']);
          }  
        }
      }
    }else{
      variable_set($item['name'],$item['value']);
    }
  }
}

function ddi_import_mask($name, $item){
  $name = str_replace("%", "", $name);
  if(substr($item['name'], 0, strlen($name)) == $name){
    variable_set($item['name'],$item['value']);
  }
}
