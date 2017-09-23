<?php
  
function drupal_deploy_export_roles($filename = ''){
  $items = drupal_deploy_export_roles_fetch_data();
  
  if(empty($filename)){
    $filename = 'roles.export';
  }
  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_roles($filename){
  if(empty($filename)){
      $filename = 'roles.export';
  }
  if(!is_file($filename)){
    drush_log(dt('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drush_log(dt('Json decode error!'), 'error');
  }
  drupal_deploy_import_roles_items($data);
}

function drupal_deploy_import_roles_items($data)
{
  foreach($data as $item){
    
    $role_permissions = array();
    
    $item['permissions'] = _ddi_roles_filter_permissions($item['permissions']);
    
    foreach($item['permissions'] as $perm){
      $role_permissions[$perm] = TRUE; 
    }

    $rid = FALSE;
    if(!empty($item['type'])){
      switch($item['type']){
        case 'anonymous user':
          $rid = DRUPAL_ANONYMOUS_RID;
          break;
        case 'authenticated user':
          $rid = DRUPAL_AUTHENTICATED_RID;
          break;
      }
    }
    if($rid){
      $existing_item = new stdClass();
      $existing_item->rid = $rid;
    }else{
      $existing_item = db_select('role', 'r')
        ->fields('r')
        ->condition('r.name', $item['name'])
        ->execute()
        ->fetchObject();
    }
    if($existing_item)
    {
      user_role_change_permissions($existing_item->rid, $role_permissions);
      drush_log(dt('Role !role permissions updated.', array('!role' => $item['name'])));      
    }
    else
    {
      $role = new stdClass();
      $role->name = $item['name'];
      user_role_save($role);
      
      $existing_item = db_select('role', 'r')
        ->fields('r')
        ->condition('r.name', $item['name'])
        ->execute()
        ->fetchObject();
      
      user_role_change_permissions($existing_item->rid, $role_permissions);
      drush_log(dt('Role !role permissions created.', array('!role' => $item['name'])));
    }
  }
}

function _ddi_roles_filter_permissions($import_permissions){
  $result_permissions = array();
  $modules = array();
  foreach (module_implements('permission') as $module) {
    $modules[$module] = $module_info[$module]['name'];
  }
  foreach ($modules as $module => $display_name) {
    if ($permissions = module_invoke($module, 'permission')) {
      $form['permission'][] = array(
        '#markup' => $module_info[$module]['name'],
        '#id' => $module,
      );
      foreach ($permissions as $perm => $perm_item) {
        
        if(in_array($perm, $import_permissions)){
          $result_permissions[] = $perm;  
        }
      }
    }
  }
  return $result_permissions;
}

function drupal_deploy_export_roles_fetch_data()
{
  $items = array();
  $role_names = user_roles();
  $role_permissions = user_role_permissions($role_names);

  foreach($role_names as $rid => $role){
    $item = array(
      'name' => $role,
      'permissions' => array_keys($role_permissions[$rid]),
    );
    
    if(DRUPAL_ANONYMOUS_RID == $rid){
      $item['type'] = 'anonymous user';
    }
    if(DRUPAL_AUTHENTICATED_RID == $rid){
      $item['type'] = 'authenticated user';
    }

    $items[] = $item;
  }
  return $items;
}