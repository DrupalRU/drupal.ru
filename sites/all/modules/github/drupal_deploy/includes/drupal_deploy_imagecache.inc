<?php
  
function drupal_deploy_export_imagecache($filename = ''){
  $items = drupal_deploy_export_imagecache_fetch_data();
  
  if(empty($filename)){
    $filename = 'imagecache.export';
  }
  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_imagecache($filename = ''){
  
  if(empty($filename)){
    $filename = 'imagecache.export';
  }
  if(!is_file($filename)){
    drupal_set_message(t('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drupal_set_message(t('Json decode error!'), 'error');
  }
  drupal_deploy_import_imagecache_items($data);
}


function drupal_deploy_export_imagecache_fetch_data(){
  $items = array();
  
  $result = db_query("SELECT * FROM {imagecache_preset} ORDER BY presetname");
  while ($db_item = db_fetch_object($result)) {
    $actions_result = db_query("SELECT * FROM {imagecache_action} WHERE presetid=%d ORDER BY action", $db_item->presetid);

    $actions = array();
    while ($db_action_item = db_fetch_object($actions_result)) {
      $actions[] = array(
        'weight' => $db_action_item->weight,
        'module' => $db_action_item->module,
        'action' => $db_action_item->action,
        'data' => unserialize($db_action_item->data)
      );
    }    
    $items[] = array(
      'presetname' => $db_item->presetname,
      'actions' => $actions,
    );    
  }
      
  return $items;
}

function drupal_deploy_import_imagecache_items($data){
  foreach($data as $item){
    $existing_preset = imagecache_preset_by_name($item['presetname']);
    if($existing_preset){
      imagecache_preset_delete($existing_preset);
    }
    $preset = array(
      'presetname' => $item['presetname'],
    );
    if($preset = imagecache_preset_save($preset)){
      drupal_set_message(t('Preset !name imported',array('!name' => $item['presetname'])));
      foreach($item['actions'] as $action){
        $action['presetid'] = $preset['presetid'];
        imagecache_action_save($action);
        drupal_set_message(t('Preset !name, !action imported',array('!name' => $preset['presetname'], '!action' => $action['action'])));
      }
    }
  }
}
