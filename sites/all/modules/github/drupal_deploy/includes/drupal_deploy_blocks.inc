<?php
  
function drupal_deploy_export_blocks($theme_name = '', $filename = ''){
  $items = drupal_deploy_export_blocks_fetch_data($theme_name);
  
  if(empty($filename)){
    $filename = $theme_name . '.' . 'blocks.export';
    if(empty($theme_name)){
      $filename = 'blocks.export';
    }
  }
  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_blocks($filename = '', $theme_name = ''){
  if(empty($filename)){
    $filename = $menu_name . '.' . 'blocks.export';
    if(empty($menu_name)){
      $filename = 'blocks.export';
    }
  }
  if(!is_file($filename)){
    drupal_set_message(t('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drupal_set_message(t('Json decode error!'), 'error');
  }
  drupal_deploy_import_blocks_items($data, $theme_name);
}

function  drupal_deploy_import_blocks_items($data, $theme_name = ''){

  $format_names = array();
  $filter_formats = db_select('filter_format', 'ff')
    ->fields('ff')
    ->orderBy('ff.format')
    ->execute()
    ->fetchAll();
    
  foreach($filter_formats as $db_item){
    $format_names[$db_item->name] = $db_item->format;
  }
  
  $roles_names = array();
  
  $roles_result = db_select('role', 'rr')
    ->fields('rr')
    ->orderBy('rr.rid')
    ->execute()
    ->fetchAll();
    
  foreach($roles_result as $db_item){
    $roles_names[$db_item->name] = $db_item->rid;
  }

  foreach($data as $item){
    if(!empty($theme_name))
    {
      if($item['theme'] != $theme_name)
      {
        continue;
      }
    }
    if($item['module'] == 'block'){
      $exists_custom = db_select('block_custom', 'bc')
        ->fields('bc')
        ->condition('bc.info', $item['info'])
        ->execute()
        ->fetchObject();
        
      if($exists_custom){
        $existing_item = db_select('block', 'b')
          ->fields('b')
          ->condition('b.module', $item['module'])
          ->condition('b.theme', $item['theme'])
          ->condition('b.delta', $exists_custom->bid)
          ->execute()
          ->fetchObject();
      }
    }else{
      $existing_item = db_select('block', 'b')
        ->fields('b')
        ->condition('b.module', $item['module'])
        ->condition('b.theme', $item['theme'])
        ->condition('b.delta', $item['delta'])
        ->execute()
        ->fetchObject();
    }
        
    if($existing_item)
    {
      // Update existing block.
      if($item['module'] == 'block'){
        // Update Boxes
        $format = 1;
        if($format_names[$item['format']]){
          $format = $format_names[$item['format']];
        }
        $box = array(
          'bid' => $item['delta'],
          'body' => $item['body'],
          'info' => $item['info'],
          'format' => $format,
        );
        drupal_write_record('block_custom', $box, 'bid');
      }
      $item['bid'] = $existing_item->bid;
      if(drupal_write_record('block', $item, 'bid')){
        if(isset($item['roles']))
        {
          db_delete('block_role')
            ->condition('module', $item['module'])
            ->condition('delta', $item['delta'])
            ->execute();
        
          $query = db_insert('block_role')->fields(array('rid', 'module', 'delta'));
          foreach($item['roles'] as $role_name)
          {
            if(isset($roles[$role_name]))
            {
              $query->values(array(
                'rid' => $roles[$role_name],
                'module' => $item['module'],
                'delta' => $item['delta'],
              ));
            }
          }
          $query->execute();
        }
      }
      drupal_set_message(t('Block module=!module delta=!delta  updated.', array(
          '!module' => $item['module'], 
          '!delta' => $item['delta'])
        )
      );
    }
    else
    {
      if($item['module'] == 'block'){
        // Update Boxes
        $format = 1;
        if(isset($format_names[$item['format']])){
          $format = $format_names[$item['format']];
        }
        $box = array(
          'body' => $item['body'],
          'info' => $item['info'],
          'format' => $format,
        );
        drupal_write_record('block_custom', $box);
        $item['delta'] = $box['bid'];
      }
      if(drupal_write_record('block', $item))
      {
        if(isset($item['roles']))
        {
          db_delete('block_role')
            ->condition('module', $item['module'])
            ->condition('delta', $item['delta'])
            ->execute();
        
          $query = db_insert('block_role')->fields(array('rid', 'module', 'delta'));
          foreach($item['roles'] as $role_name)
          {
            if(isset($roles[$role_name]))
            {
              $query->values(array(
                'rid' => $roles[$role_name],
                'module' => $item['module'],
                'delta' => $item['delta'],
              ));
            }
          }
          $query->execute();
        }
      }
      drupal_set_message(t('Block module=!module delta=!delta  added.', array(
          '!module' => $item['module'], 
          '!delta' => $item['delta'])
        )
      );
    }
    

  }
}

function drupal_deploy_export_blocks_fetch_data($theme_name = ''){
  $items = array();
  
  $format_names = array();
  $filter_formats = db_select('filter_format', 'ff')
    ->fields('ff')
    ->orderBy('ff.format')
    ->execute()
    ->fetchAll();
    
  foreach($filter_formats as $db_item){
    $format_names[$db_item->format] = $db_item->name;
  }
  
  $roles_names = array();
  
  $roles_result = db_select('role', 'rr')
    ->fields('rr')
    ->orderBy('rr.rid')
    ->execute()
    ->fetchAll();
    
  foreach($roles_result as $db_item){
    $roles_names[$db_item->rid] = $db_item->name;
  }
  
  $query = db_select('block', 'b')
    ->fields('b');

  if(is_string($theme_name)){
    $query->condition('b.theme', $theme_name);
  }
  
  if($status = drush_get_option('status', FALSE)){
    $query->condition('b.status', $status);
  }

  $query->orderBy('b.bid');
  $blocks = $query->execute()->fetchAll();

  foreach ($blocks as $db_item) {
    $item = array(
      'module' => $db_item->module,
      'delta' => $db_item->delta,
      'theme' => $db_item->theme,
      'status' => $db_item->status,
      'weight' => $db_item->weight,
      'region' => $db_item->region,
      'custom' => $db_item->custom,
      'visibility' => $db_item->visibility,
      'pages' => $db_item->pages,
      'title' => $db_item->title,
      'cache' => $db_item->cache,
    );
    
    if($db_item->module == 'block')
    {
      $custom = db_select('block_custom', 'bc')
        ->fields('bc')
        ->condition('bc.bid', $db_item->delta)
        ->execute()
        ->fetchObject();
      
      if($custom)
      {
        $item['body'] = $custom->body;
        $item['info'] = $custom->info;
        if(isset($format_names[$custom->format]))
        {
          $item['format'] = $format_names[$custom->format];
        }                
      }
    }
    
    $block_roles = db_select('block_role', 'br')
      ->fields('br')
      ->condition('br.module', $db_item->module)
      ->condition('br.delta', $db_item->delta)
      ->execute()
      ->fetchAll();
    
    $roles = array();

    foreach ($block_roles as $db_role) {
      if(isset($roles_names[$db_role->rid])){
        $roles[] = $roles_names[$db_role->rid];
      }
    }
    if(!empty($roles))
    {
      $item['roles'] = $roles;
    }
    
    $items[] = $item;
  }
  return $items;
}

