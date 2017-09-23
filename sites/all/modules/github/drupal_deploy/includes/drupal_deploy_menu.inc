<?php
  
function drupal_deploy_export_menu($menu_name = '', $filename = ''){
  $items = drupal_deploy_export_menu_fetch_data($menu_name);
  
  if(empty($filename)){
    $filename = $menu_name . '.' . 'menu_links.export';
    if(empty($menu_name)){
      $filename = 'menu_links.export';
    }
  }
  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_menu($filename = '', $menu_name = ''){
  if(empty($filename)){
    $filename = $menu_name . '.' . 'menu_links.export';
    if(empty($menu_name)){
      $filename = 'menu_links.export';
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
  drupal_deploy_import_menu_links($data, $menu_name);
}

function drupal_deploy_import_menu_links($data, $menu_name = ''){
  foreach($data as $item){
    if(!empty($menu_name))
    {
      if($item['menu_name'] != $menu_name)
      {
        continue;
      }
    }

    if(ddi_menu_link_save($item)){
      if($item['children']){
        ddi_menu_import_children($item['children'], $item['mlid']);
      }
    }
        
  }
}

function ddi_menu_import_children($children, $plid){
  foreach($children as $item){
    $item['plid'] = $plid;
    if(ddi_menu_link_save($item)){
      if($item['children']){
        ddi_menu_import_children($item['children'], $item['mlid']);
      }
    }
  }
}

function ddi_menu_link_save(&$item){
          
  $item['link_path'] = ddi_menu_alias_origin($item['link_path']);
      
  $existing_item = db_select('menu_links', 'm')
    ->fields('m')
    ->condition('m.link_path', $item['link_path'])
    ->condition('m.menu_name', $item['menu_name'])
    ->condition('m.module', $item['module'])
    ->execute()
    ->fetchObject();
  
  if($existing_item->mlid)
  {
    $item['mlid'] = $existing_item->mlid;
  }
  if(menu_link_save($item))
  {
    drush_log(dt('Menu !link_title  imported', array('!link_title' => $item['link_title'])));
    return TRUE;
  }
  else
  {
    drush_log(dt('Menu !link_title NOT imported', array('!link_title' => $item['link_title'])), 'error');
    return FALSE;
  }
}


function drupal_deploy_export_menu_fetch_data($menu_name = ''){
  $items = array();
  
  $query = db_select('menu_links', 'm')
    ->fields('m')
    ->condition('m.plid', 0);
    
  if(!empty($menu_name)){
    $query->condition('menu_name', $menu_name);
  }
  
  $query->orderBy('m.mlid');
  
  $menu_items = $query->execute()->fetchAll();
  
  foreach($menu_items as $db_item){
    $item = array(
      'menu_name' => $db_item->menu_name,
      'weight' => $db_item->weight,
      'link_title' => $db_item->link_title,
      'link_path' => dde_menu_alias($db_item->link_path),
      'hidden' => $db_item->hidden,
      'has_children' => $db_item->has_children,
      'expanded' => $db_item->expanded,
      'options' => unserialize($db_item->options),
      'module' => $db_item->module,
      'customized' => $db_item->customized,
      'updated' => $db_item->updated,
    );
    if($children = dde_menu_items($db_item->mlid)){
      $item['children'] = $children;
    }    
    
    $items[] = $item;
  }
  return $items;
}

function dde_menu_items($plid){
  $menu_items = db_select('menu_links', 'm')
    ->fields('m')
    ->condition('m.plid', $plid)
    ->orderBy('m.mlid')
    ->execute()
    ->fetchAll();
  foreach($menu_items as $db_item){
    $item = array(
      'menu_name' => $db_item->menu_name,
      'weight' => $db_item->weight,
      'link_title' => $db_item->link_title,
      'link_path' => dde_menu_alias($db_item->link_path),
      'hidden' => $db_item->hidden,
      'has_children' => $db_item->has_children,
      'expanded' => $db_item->expanded,
      'options' => unserialize($db_item->options),
      'module' => $db_item->module,
      'customized' => $db_item->customized,
      'updated' => $db_item->updated,
    );
    drush_log(dt('Menu !title exported', array('!title' =>  $db_item->link_title)));
    if($children = dde_menu_items($db_item->mlid)){
      $item['children'] = $children;
    }
    
    $items[] = $item;
  }
  return $items;
}

