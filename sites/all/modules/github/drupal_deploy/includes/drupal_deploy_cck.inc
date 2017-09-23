<?php
  
function drupal_deploy_export_cck($filename = ''){
  $items = drupal_deploy_export_cck_fetch_data();
  
  if(empty($filename)){
    $filename = 'cck.export';
  }
  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_cck($filename){
  if(empty($filename)){
      $filename = 'cck.export';
  }
  if(!is_file($filename)){
    drush_log(dt('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drush_log(dt('Json decode error!'), 'error');
  }
  drupal_deploy_import_cck_items($data);
}

function drupal_deploy_import_cck_items($data)
{

  include_once('./'. drupal_get_path('module', 'content') .'/includes/content.admin.inc');

  $content_instances = array();
  
  foreach($data['instance'] as $item){
    $existing_item = db_fetch_object(db_query("SELECT * FROM {" . content_instance_tablename() . "} WHERE field_name = '%s' AND type_name='%s'", $item['field_name'], $item['type_name']));
    if($existing_item)
    {
      if(drupal_write_record(content_instance_tablename(), $item, array('field_name','type_name')))
      {
        drush_log(dt('Field instance !field_name !type_name updated.', array('!field_name' => $item['field_name'], '!type_name' => $item['type_name'])), 'ok');
      }
    }
    else
    {
      if(drupal_write_record(content_instance_tablename(), $item))
      {
        drush_log(dt('Field instance !field_name !type_name created.', array('!field_name' => $item['field_name'], '!type_name' => $item['type_name'])), 'ok');
      }
    }
  }

  
  foreach($data['fields'] as $item){  
    
    $existing_item = db_fetch_array(db_query("SELECT * FROM {" . content_field_tablename() . "} nf LEFT JOIN {".content_instance_tablename()."} nfi ON nfi.field_name = nf.field_name WHERE nfi.field_name = '%s'", $item['field_name']));
    
    $item['columns'] = $item['db_columns'];
    
    if($existing_item)
    {
      foreach (array('widget_settings', 'display_settings', 'global_settings', 'db_columns') as $key) {
        $existing_item[$key] = (!empty($existing_item[$key])) ? (array) unserialize($existing_item[$key]) : array();
      }
      // 'columns' is a reserved word in MySQL4, so our column is named 'db_columns'.
      $existing_item['columns'] = $existing_item['db_columns'];
      unset($existing_item['db_columns']);
  
      // Unfold 'global_settings'.
      foreach ($existing_item['global_settings'] as $key => $value) {
        $existing_item[$key] = $value;
      }
      unset($existing_item['global_settings']);
      
      if(drupal_write_record(content_field_tablename(), $item, 'field_name'))
      {
        ddi_cck_get_instance_type_name($item, $data['instance']);
        content_alter_schema((array)$existing_item, (array)$item);
        drush_log(dt('Field !field_name updated.', array('!field_name' => $item['field_name'])), 'ok');
      }
    }
    else
    {
      if(drupal_write_record(content_field_tablename(), $item))
      {
        ddi_cck_get_instance_type_name($item, $data['instance']);
        content_alter_schema(array(), (array)$item);
        drush_log(dt('Field !field_name created.', array('!field_name' => $item['field_name'])), 'ok');
      }
    }
  }
  
  
  content_clear_type_cache(TRUE);
  menu_rebuild();
}

function ddi_cck_get_instance_type_name(&$field, $instances){
  foreach($instances as $instance){
    if($instance['field_name'] == $field['field_name']){
      foreach($instance as $key => $val){
        $field[$key] = $val;
      }
    }
  }
}

function drupal_deploy_export_cck_fetch_data()
{
  $items = array();
  $result = db_query("SELECT * FROM {" . content_field_tablename() . "} ORDER BY field_name");
  while ($db_item = db_fetch_array($result)) {
    $db_item['global_settings'] = unserialize($db_item['global_settings']);
    $db_item['db_columns'] = unserialize($db_item['db_columns']);
    
    $items['fields'][] = $db_item;
  }

  $result = db_query("SELECT * FROM {" . content_instance_tablename() . "} ORDER BY field_name");
  while ($db_item = db_fetch_array($result)) {
    $db_item['widget_settings'] = unserialize($db_item['widget_settings']);
    $db_item['display_settings'] = unserialize($db_item['display_settings']);
    
    $items['instance'][] = $db_item;
  }

  return $items;
}
