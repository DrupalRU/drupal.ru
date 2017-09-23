<?php
  
function drupal_deploy_export_aggregator($filename = ''){
  $items = drupal_deploy_export_aggregator_fetch_data();
  
  if(empty($filename)){
      $filename = 'aggregator.export';
  }
  drupal_deploy_export_save($items, $filename);
}

function drupal_deploy_import_aggregator($filename = ''){
  if(empty($filename)){
      $filename = 'aggregator_links.export';
  }
  if(!is_file($filename)){
    drush_log(dt('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  $data = file_get_contents($filename);
  if(!$data = json_decode($data, TRUE)){
    drush_log(dt('Json decode error!'), 'error');
  }
  drupal_deploy_import_aggregator_feed($data);
}

function drupal_deploy_import_aggregator_feed($data){
  foreach($data as $item){
    
    if($existing_item = db_fetch_array(db_query("SELECT * FROM {aggregator_feed} WHERE url = '%s' and title='%s'", $item['url'], $item['title'])))
    {
      $item['fid'] = $existing_item['fid'];
      drupal_write_record('aggregator_feed', $item, 'fid');
      drush_log(dt('aggregator !title updated', array('!title' => $item['title'])), 'ok');
    }
    else
    {
      drupal_write_record('aggregator_feed', $item);
      drush_log(dt('aggregator !title created', array('!title' => $item['title'])), 'ok');
    }
    
    aggregator_refresh($item);
  }
}

function drupal_deploy_export_aggregator_fetch_data($aggregator_name = ''){
  $items = array();

  $result = db_query("SELECT * FROM {aggregator_feed} ORDER BY fid");

  while ($db_item = db_fetch_object($result)) {
    
    //"INSERT INTO {aggregator_feed} (title, url, refresh, block, description, image)
    $item = array(
      'title' => $db_item->title,
      'url' => $db_item->url,
      'refresh' => $db_item->refresh,
      'block' => $db_item->block,
      'description' => $db_item->description,
      'image' => $db_item->image,
    );
    
    $items[] = $item;
  }
  return $items;
}

