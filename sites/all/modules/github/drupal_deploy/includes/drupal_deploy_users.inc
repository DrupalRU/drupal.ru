<?php
  
function drupal_deploy_export_users($filename = ''){
  
  if(empty($filename)){
    $filename = 'users.export';
  }

  $items = drupal_deploy_export_users_fetch_data($filename);

  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_users($filename){
  if(empty($filename)){
      $filename = 'users.export';
  }
  if(!is_file($filename)){
    drupal_set_message(t('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drupal_set_message(t('Json decode error!'), 'error');
  }
  drupal_deploy_import_users_items($data, $filename);
}

function drupal_deploy_import_users_items($data, $import_filename)
{
  global $user;
  if ($user->uid != 1) {
    drush_log('Switching to uid=1 now', 'notice');
    $user = user_load(1);
  }
  
  $import_folder = $import_filename . '.files';
  
  foreach($data as $item){
    $item['signature_format'] = ddi_get_format_id($item['signature_format']);
    $existing_item = db_fetch_object(db_query("SELECT * FROM {users} WHERE name = '%s'", $item['name']));
    if($existing_item)
    {
      $item['uid'] = $existing_item->uid;
      if(drupal_write_record('users', $item, 'uid'))
      {
        drupal_set_message(t('User !user updated.', array('!user' => $item['name'])));
        db_query('DELETE FROM {users_roles} WHERE uid=%d', $item['uid']);
      }
    }
    else
    {
      if(drupal_write_record('users', $item))
      {
        drupal_set_message(t('User !user created.', array('!user' => $item['name'])));
      }
    }
    if(!empty($item['roles'])){
      foreach($item['roles'] as $role){
        $role_data = db_fetch_object(db_query("SELECT * FROM {role} WHERE name = '%s'", $role));
        if($role_data){
          $user_roles = array('uid' => $item['uid'], 'rid' => $role_data->rid);
          if(drupal_write_record('users_roles', $user_roles)){
            drupal_set_message(t('Role !role permissions granted to !user.', array('!role' => $role_data->name, '!user' => $item['name'])));
          }
        }
      }
    }
    if(!empty($item['picture'])){
      
      $info = pathinfo($item['picture']);
      
      $directory = file_directory_path() . '/' . variable_get('user_picture_path', 'pictures');
      $destination = $directory . '/picture-' . $item['uid'] . '.' . $info ['extension'];
      
      if(file_exists($destination)){
        file_delete($destination);
      }
      file_check_directory($directory, FILE_CREATE_DIRECTORY);
      $stored_avatar = $import_folder .'/' . basename($item['picture']);
      
      copy($import_folder .'/'. basename($item['picture']), $destination);
      $item['picture'] = $destination;
      drupal_write_record('users', $item, 'uid');
    }
    
    $data = drupal_deploy_import_get_user_forms($item['uid']);
    $modules = $item['modules'];
    foreach($data as $module => $settings){

      $modules[$module] = _replace_values($modules[$module], $settings);
      $modules[$module] = _prepare_item_values_to_save($modules[$module], $settings);

      $account = $item;      
      foreach($modules[$module] as $key => $val){
        $account[$key] = $val;
      }
      
      drupal_deploy_import_form_hook_send($module, $account);
      
    }
  }
}

function drupal_deploy_import_form_hook_send($module, $edit){
  $function = $module . '_user';
  $account = (object)$edit;
  if (function_exists($function)){
    $function('submit', $edit, $account, 'account');
  }
}

function drupal_deploy_export_users_fetch_data($filename)
{
  
  global $user;
  if ($user->uid != 1) {
    drush_log('Switching to uid=1 now', 'notice');
    $user = user_load(1);
  }
  
  $items = array();
  
  if($uids = drush_get_option('uid', FALSE)){
    $result = db_query("SELECT * FROM {users} WHERE uid in (%s) ORDER BY uid", $uids);
  }elseif($rids = drush_get_option('rid', FALSE)){
    $result = db_query("SELECT ur.uid FROM {users_roles} ur LEFT JOIN {role} r ON r.rid = ur.rid WHERE r.rid in (%s) GROUP BY ur.uid ORDER BY ur.uid", $rids);
    $uids = array();
    while ($uid = db_fetch_object($result)) {
      $uids[] = $uid->uid;
    }
    $uids = implode(",", $uids);
    drush_log(dt('Exporting users uids: !uids', array('!uids' => $uids)));
    $result = db_query("SELECT * FROM {users} WHERE uid in (%s) ORDER BY uid", $uids);
  }else{
    $result = db_query("SELECT * FROM {users} ORDER BY uid");
  }
  while ($db_item = db_fetch_object($result)) {
    $data = drupal_deploy_export_get_user_forms($db_item->uid);

    $result_roles = db_query("SELECT r.* FROM {users_roles} ur LEFT JOIN {role} r ON r.rid = ur.rid WHERE ur.uid = %d ORDER BY r.name", $db_item->uid);
    $roles = array();
    while ($role_item = db_fetch_object($result_roles)) {
      $roles[] = $role_item->name;
    }
    if(is_file($db_item->picture)){
      dde_save_avatar($db_item->picture, $filename);
    }
    $item = array(
      'name' => $db_item->name,
      'pass' => $db_item->pass,
      'mail' => $db_item->mail,
      'mode' => $db_item->mode,
      'sort' => $db_item->sort,
      'theme' => $db_item->theme,
      'signature' =>  $db_item->signature,
      'signature_format' =>  dde_get_format_name($db_item->signature_format),
      'created' => $db_item->created,
      'access' => $db_item->access,
      'login' => $db_item->login,
      'status' => $db_item->status,
      'timezone' => $db_item->timezone,
      'language' => $db_item->language,
      'picture' => $db_item->picture,
      'init' => $db_item->init,
      'roles' => $roles,
      'modules' => $data,
    );

    $items[] = $item;
  }
  return $items;
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
  if(is_array($values_options)){
    $values_array = FALSE;
    foreach($values_options as $key => $val){
      if(is_array($val)){
        $values_array = TRUE;
        $item[$key] = _replace_values($item[$key],$values_options[$key]);
      }
    }
    if($values_array == FALSE){
      $result = NULL;
      foreach($values_options as $key => $val){
        if(is_array($item)){
          if($val == TRUE){
            $result = FALSE;
            break;
          }
          foreach($item as $item_val){
            if($item_val == $val){
              $result[] = $key;
            }
          }
        }else{
          if($item == $val){
            return $key;
          }
        }
      }
      if(!$result){
        return $item;
      }     
      return $result;
    }
  }
  return $item;
}

function drupal_deploy_import_get_user_forms($uid){
  $account = user_load($uid);
  $groups = array();
  foreach (module_implements('user') as $module) {
    if( in_array($module, array('user','system', 'locale'))){
      continue;
    }
    $function = $module . '_user';
    $edit = (array)$account;
    if (function_exists($function) && ($data = $function('form', $edit, $account, 'account'))) {
      $return = array();
      foreach($data as $key => $val){
        if($key[0] == '#')
        {
          continue;
        }else{
          $return[$key] = _process_form_variable_import($val);  
        }
      }
  
      $groups[$module] = $return;
      
    }
  }
  return $groups;
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

function drupal_deploy_export_get_user_forms($uid){
  global $language;
  
  $language->language = 'en';
    
  $account = user_load($uid);
  $groups = array();
  foreach (module_implements('user') as $module) {
    if( in_array($module, array('user','system', 'locale'))){
      continue;
    }
    $function = $module . '_user';
    $edit = (array)$account;
    if (function_exists($function) && ($data = $function('form', $edit, $account, 'account'))) {
      $return = array();
      foreach($data as $key => $val){
        if($key[0] == '#')
        {
          continue;
        }else{
          $return[$key] = _process_form_variable($val);  
        }
      }
  
      $groups[$module] = $return;
      
    }
  }
  return $groups;
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

function dde_save_avatar($avatarfile, $export_filename){
  $export_folder = $export_filename . '.files';
  if(!is_dir($export_folder)){
    mkdir($export_folder);
  }
  copy($avatarfile, $export_folder . '/'. basename($avatarfile));
}
