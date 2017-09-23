<?php
  
function drupal_deploy_export_node_types($filename = ''){
  $items = drupal_deploy_export_node_types_fetch_data();
  
  if(empty($filename)){
    $filename = 'node_types.export';
    $type = drush_get_option('type', FALSE);
    if($type){
      $filename = $type . '.node_types.export';  
    }
  }
  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_node_types($filename){
  if(empty($filename)){
      $filename = 'node_types.export';
  }
  if(!is_file($filename)){
    drush_log(dt('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drush_log(dt('Json decode error!'), 'error');
  }
  drupal_deploy_import_node_types_items($data);
}

function drupal_deploy_import_node_types_items($data)
{
  $not_save_variable = array(
    'type',
    'name',
    'base',
    'description',
    'help',
    'has_title',
    'title_label',
    'custom',
    'modified',
    'locked',
    'orig_type',
    'module',
    'disabled',
  );

  $type = drush_get_option('type', FALSE);
  foreach($data as $item){
    
    $origin_item = $item;
    if(!empty($type)){
      if($type != $item['type']){
        continue;
      }
    }
    
    if(!empty($item['fields'])){
      $fields['fields'] = $item['fields'];
      $fields['instances'] = $item['instances'];
    }
    unset($item['fields']);
    unset($item['instances']);    

    $values_options = drupal_deploy_import_get_variables($item);
    $item = _replace_values($item, $values_options);
    
    $item = _prepare_item_values_to_save($item, $values_options);
            
    $node_type = (object) $item;
    $status = node_type_save($node_type);
    if($status == SAVED_UPDATED || $status == SAVED_NEW)
    {
      foreach ($item as $key => $value) {
        if(in_array($key, $not_save_variable)){
          continue;
        }
        $variable = $key . '_' . $item['type'];
        variable_set($variable, $value);
      }
      
      //Fix menu
      if(empty($origin_item['menu']['menu_options'])){
        $origin_item['menu']['menu_options'] = array();
      }
      variable_set('menu_options_' . $item['type'], $origin_item['menu']['menu_options']);
      variable_set('menu_parent_' . $item['type'], $origin_item['menu']['menu_parent']);
      
      ddi_fields($fields, 'node', $item['type']);
      drush_log(dt('Node type !title imported.', array('!title' => $item['name'])));
      
    }
    else{
      drush_log(dt('Node type !title NOT imported.', array('!title' => $item['name'])));
    }
  }
  node_types_rebuild();
  menu_rebuild();
}

function _prepare_item_values_to_save($item, $values_options){
  $result = array_diff($item, $values_options);
  foreach($values_options as $key => $val){
    if(is_array($val)){
      if(array_key_exists('#tree',$val)){
        if($val['#tree']){
          $result[$key] = _prepare_item_values_to_save($item[$key],$values_options[$key]);
        }
        else
        {
          $tmp_arr = _prepare_item_values_to_save($item[$key],$values_options[$key]);
          foreach($tmp_arr as $tkey => $tval){
            $result[$tkey] = $tval;
          }
        }
      }
      else{
        $result[$key] = $item[$key];
      }
    } else{
      $result[$key] = $item[$key];
    }
  }
  if(array_key_exists('#tree',$result)){
    unset($result['#tree']);
  }
  return $result;
}

function _replace_values($item, $values_options){

  $origin_item = $item;
  if(empty($item)){
    return FALSE;
  }
  
  //if we do not need to process value.
  if(is_array($item) 
  && is_array($values_options) 
  && 1 == count($item) ){
    $item_key = key($item);
    if(!is_numeric($item_key) 
    && isset($values_options[$item_key])
    && !is_array($values_options[$item_key])){
      return $item;
    }
  }
  
  $result = NULL;
  // Checking all settings for $item.
  if(is_array($values_options)){
    //Key switch for reqursion.
    if(!is_array($item)){
      $value_key = array_search($item, $values_options);
      if(FALSE !== $value_key){
        return $value_key;
      }
      return $item;
    }

    foreach($values_options as $key => $val){
      if(empty($val)){
        //skip!
        continue;
      }
      // If value is array, we need to go dipper
      if(is_array($val)){
        $item[$key] = _replace_values($item[$key],$values_options[$key]);
      }else{
        if(!is_array($item)){
          if($item == $val){
            return $key;
          }
        }else{
          if(FALSE !== array_search($val, $item)){
            $result[] = $key;
          }
        }
      }
    }
    if($origin_item == $item && !empty($result)){
      return $result;
    }
  }
  return $item;
}

function drupal_deploy_export_node_types_fetch_data()
{
  $items = array();
  
  $query = db_select('node_type', 'nt')
    ->fields('nt');
  
  $type = drush_get_option('type', FALSE);
  if(!empty($type)){
    $query->condition('nt.type', $type);
  }
  
  $query->orderBy('nt.type');
  
  $node_type_items = $query->execute()->fetchAll();
  
  foreach($node_type_items as $db_item){

    $item = (array) $db_item;
    $item = array_merge($item, drupal_deploy_export_get_variables($db_item));
    
    //Menu fix
    $item['menu']['menu_options'] = variable_get('menu_options_' . $db_item->type);
    $item['menu']['menu_parent'] = variable_get('menu_parent_' . $db_item->type);
    
    $result = dde_fields('node', $db_item->type);
    $item['fields'] = $result['fields'];
    $item['instances'] = $result['instances'];
    $items[] = $item;
  }
  return $items;
}


function drupal_deploy_export_get_variables($type){
  global $language;
  
  $language->language = 'en';
  $form_id = 'node_type_form';
  $form_state = array();
  $form = array();
  $form = node_type_form($form, $form_state, $type);
  drupal_prepare_form($form_id, $form, $form_state);
  

  $return = array();

  unset($form['name']);
  unset($form['type']);
  unset($form['description']);
  unset($form['additional_settings']);
  
  unset($form['submission']['title_label']);
  unset($form['submission']['help']);
  unset($form['old_type']);
  unset($form['orig_type']);
  unset($form['base']);
  unset($form['actions']);
  unset($form['module']);
  unset($form['custom']);
  unset($form['modified']);
  unset($form['locked']);
  unset($form['form_id']);
  unset($form['form_build_id']);
  

  $tmp_array = array();
  foreach($form as $key => $val){
    if($key[0] == '#')
    {
      continue;
    }else{
      $return[$key] = _process_form_variable($val);  
    }
  }
  
  return $return;
}

function _process_form_variable($array)
{
  if(isset($array['#type'])){
    switch($array['#type']){
      case 'fieldset':
      case 'item':
        $return = array();
        foreach($array as $key => $val){
          if($key[0] == '#')
          {
            continue;
          }else{
            $return[$key] = _process_form_variable($val);
          }
        }
        return $return;
        break;
      case 'checkbox':
      case 'textarea':
      case 'textfield':
        return $array['#default_value'];
        break;
      case 'radios':
      case 'select':
        return $array['#options'][$array['#default_value']];
        break;
      case 'checkboxes':
        $return = array();
        foreach($array['#default_value'] as $val){
           $return[] =  $array['#options'][$val];
        }
        return $return;
    }
  }
}

function drupal_deploy_import_get_variables($type){
  
  global $language;
  
  $language->language = 'en';
  $form_id = 'node_type_form';
  $form_state = array();
  $form = array();
  $form = node_type_form($form, $form_state, $type);
  drupal_prepare_form($form_id, $form, $form_state);

  $return = array();
  
  unset($form['name']);
  unset($form['type']);
  unset($form['description']);
  unset($form['additional_settings']);
  
  unset($form['submission']['title_label']);
  unset($form['submission']['help']);
  unset($form['old_type']);
  unset($form['orig_type']);
  unset($form['base']);
  unset($form['actions']);
  unset($form['module']);
  unset($form['custom']);
  unset($form['modified']);
  unset($form['locked']);
  unset($form['form_id']);
  unset($form['form_build_id']);

  foreach($form as $key => $val){
    if($key[0] == '#')
    {
      continue;
    }else{
      $return[$key] = _process_form_variable_import($val);  
    }
  }
  
  return $return;
}

function _process_form_variable_import($array)
{
  if(isset($array['#type'])){
    switch($array['#type']){
      case 'fieldset':
      case 'item':
        $return = array();
        if($array['#tree']){
          $return['#tree'] = $array['#tree'];
        }else{
          $return['#tree'] = FALSE;
        }
        foreach($array as $key => $val){
          if($key[0] == '#')
          {
            continue;
          }else{
            $return[$key] = _process_form_variable_import($val);
          }
        }
        return $return;
        break;
      case 'checkbox':
      case 'textarea':
      case 'textfield':
        return TRUE;
        break;
      case 'radios':
      case 'select':
      case 'checkboxes':
        return $array['#options'];
        break;
    }
  }
}
