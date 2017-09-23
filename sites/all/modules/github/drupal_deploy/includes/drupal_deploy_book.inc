<?php
  
function drupal_deploy_export_book($filename = ''){
  
  $nid = drush_get_option('nid', FALSE);
  
  if(empty($nid)){
    $result = db_query("SELECT * FROM {book} WHERE nid = bid ORDER BY nid");
    while ($db_item = db_fetch_object($result)) {
      $nid = $db_item->nid;
      $items = drupal_deploy_export_book_fetch_data($nid);
      $filename = $nid . '.' . 'book.export';
      drupal_deploy_export_save($items, $filename); 
    }
  }else{
    $items = drupal_deploy_export_book_fetch_data($nid);
    $filename = $nid . '.' . 'book.export';
    drupal_deploy_export_save($items, $filename); 
  }
}

function drupal_deploy_import_book($filename = ''){
  if(empty($filename)){
    $filename = 'book.export';
  }
  if(!is_file($filename)){
    drupal_set_message(t('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drupal_set_message(t('Json decode error!'), 'error');
  }
  drupal_deploy_import_book_links($data);
}

function drupal_deploy_import_book_links($data){
  
  ddi_book_import_children($data, 0, 0);
  
}

function ddi_book_import_children($children, $plid, $bid){
  foreach($children as $item){
    if($bid == 0){
      if(!$bid = ddi_get_outline($item['link_path'])){
        continue;
      }
    }
    $item['menu_name'] = 'book-toc-' . $bid;
    $item['plid'] = $plid;
    if(ddi_book_menu_link_save($item)){
      if(!$origin_book_item = db_fetch_object(db_query("SELECT * FROM {book} WHERE nid = %d AND bid = %d and mlid = %d", $item['nid'], $bid, $item['mlid']))){
        $outline = array(
          'nid' => $item['nid'],
          'bid' => $bid,
          'mlid' => $item['mlid'],
        );
        drupal_write_record('book', $outline);
        drush_log(dt('Create outline page !mlid', array('!mlid' => $outline['mlid'])));
      }
    }
    
    if($item['children']){
      ddi_book_import_children($item['children'], $item['mlid'], $bid);
    }
  }
}

function ddi_book_menu_link_save(&$item){
          
  $item['link_path'] = ddi_menu_alias_origin($item['link_path']);
  
  $node_link = explode('/', $item['link_path']);
  if($node_link[0] == 'node' && is_numeric($node_link[1])){
    $item['nid'] = $node_link[1];
  }
    
  $existing_item = db_fetch_object(db_query("SELECT * FROM {menu_links} WHERE link_path = '%s' and menu_name='%s'", $item['link_path'], $item['menu_name']));
  
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

function ddi_get_outline($outline_link_origin){
  
  $outline_link = ddi_menu_alias_origin($outline_link_origin);
  if($outline_link == $outline_link_origin){
    drush_log(dt('Path !outline is not created yet.',array('!outline' => $outline_link_origin )),'error');
    return FALSE;
  }
  
  $outline_link = explode('/', $outline_link);
  $nid = $outline_link[1];
  if(is_numeric($nid)){
    return $nid;
  }
  return FALSE;
}

function drupal_deploy_export_book_fetch_data($nid){
  $items = array();
  
  $menu_name = 'book-toc-' . $nid;
  
  $items = dde_book_menu_items($menu_name, 0);
  
  return $items;
}

function dde_book_menu_items($menu_name, $plid){
  $result = db_query("SELECT * FROM {menu_links} WHERE menu_name='%s' and plid = %d ORDER BY mlid", $menu_name, $plid);
  while ($db_item = db_fetch_object($result)) {
    $item = array(
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
    if($children = dde_book_menu_items($menu_name, $db_item->mlid)){
      $item['children'] = $children;
    }
    
    $items[] = $item;
  }
  return $items;
}

