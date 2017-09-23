<?php

/**
 * @file
 *   Defines the cacherouter administrative interface.
 */

function cacherouter_admin_stats_page($cache_name = 'default') {
  global $cache;
  if (empty($cache_name)) {
    $cache_name = 'default';
  }
  $stats = $cache->getStats($cache_name);

  return theme('cacherouter_admin_stats_page', $cache_name, $stats);
}

function theme_cacherouter_admin_stats_page($cache_name, $stats) {
  global $cache;
  
  drupal_add_css(drupal_get_path('module', 'cacherouter') .'/cacherouter.css');
  drupal_add_js(drupal_get_path('module', 'cacherouter') .'/cacherouter.js');
    
  $output = '<div id="cacherouter-stats">';
  $output .= t('<h3 class="cr-hdr">Cache Name: </h3>');
  
  // TODO: should be a formapi form.
  $output .= '<form action="/admin/settings/cacherouter" id="cr-cache-switch"><select name="cacherouter-cache">';

  foreach ($cache->getBins() as $bin) {
    if (arg(3) == $bin) {
      $output .= '<option selected="selected" value="'. url('admin/settings/cacherouter/'. $bin).'">'. $bin .'</option>';
    }
    else { 
      $output .= '<option value="'. url('admin/settings/cacherouter/'. $bin).'">'. $bin .'</option>';
    }
  }
  $output .= '</select></form>';
  
  $output .= '<br />';
  $output .= '<h3 class="cr-hdr">Cache Type: </h3>';
  
  // Get cache type and link to documentation
  $cache_docs = array(
    'apc' => l('APC', 'http://pecl.php.net/package/APC'),
    'db' => l('Database', 'http://www.drupal.org/'),
    'eacc' => l('eAccelerator', 'http://eaccelerator.com/'),
    'file' => 'File system',
    'memcache' => l('Memcache', 'http://www.danga.com/memcached/'),
    'xcache' => l('XCache', 'http://xcache.org/'),
  );
  $output .= $cache_docs[$cache->getType($cache_name)];

  // Get cache stats table with graphs.
  $stats_header = array(t('Memory'), t('Hit / Miss'), t('Get / Set'));
  $stats_attributes = array('id' => 'cacherouter-stats');
  $stats_table = array();
  
  // First do our calculations for percentages and sizes
  $mem_used = round(($stats['bytes_used'] / $stats['bytes_total']) * 100);
  $mem_free = round(100 - $mem_used);
  if ($mem_free == 100) {
    $mem_free = 99;
    $mem_used = 1;
  }
  
  if ($stats['bytes_used'] == 0 && $stats['bytes_total'] == 0) {
    $chart1 = theme('image', drupal_get_path('module', 'cacherouter') .'/images/na.png');
  }
  else {
    $chart1 = theme('image', 'http://chart.apis.google.com/chart?chs=250x100&chd=t:'. $mem_used .','. $mem_free . '&cht=p3&chl=Used|Free&chco=3399cc,cbe2f1', '', '', NULL, FALSE);
  }
  
  if ($stats['hits'] > 0) {
    $hits_hit = round(($stats['hits'] / ($stats['hits'] + $stats['misses'])) * 100);
  }
  else {
    $hits_hit = '0';
  }

  if ($stats['misses'] > 0) {
    $hits_misses = round(($stats['misses'] / ($stats['hits'] + $stats['misses'])) * 100);
  }
  else {
    $hits_misses = '0';
  }
  
  if ($hits_misses == 0 && $hits_hit == 0) {
    $chart2 = theme('image', drupal_get_path('module', 'cacherouter') .'/images/na.png');
  }
  else {
    $chart2 = theme('image', 'http://chart.apis.google.com/chart?chs=250x100&chd=t:'. $hits_hit .','. $hits_misses .'&cht=p3&chl=Hit|Miss&chco=3399cc,cbe2f1', '', '', NULL, FALSE);
  }
  
  if ($stats['gets'] > 0) {
    $req_gets = round(($stats['gets'] / ($stats['gets'] + $stats['sets'])) * 100);  
  }
  else {
    $req_gets = '0';
  }
  
  if ($stats['sets'] > 0) {
    $req_sets = round(($stats['sets'] / ($stats['gets'] + $stats['sets'])) * 100);
  }
  
  if ($req_gets == 0 && $req_sets == 0) {
    $chart3 = theme('image', drupal_get_path('module', 'cacherouter') .'/images/na.png');
  }
  else {
    $chart3 = theme('image', 'http://chart.apis.google.com/chart?chs=250x100&chd=t:'. $req_gets .','. $req_sets .'&cht=p3&chl=Get|Set&chco=3399cc,cbe2f1', '', '', NULL, FALSE);
  }
  
  
  // First row is images
  $stats_table[] = array($chart1, $chart2, $chart3);
  
  // Next row is stats and key for images
  $stats_table[] = array(
    theme('cacherouter_admin_stats_key', t('Used'), _cacherouter_convert_size($stats['bytes_used']) .' ('. $mem_used .'%)', 
                                         t('Free'), _cacherouter_convert_size($stats['bytes_total'] - $stats['bytes_used']) .' ('. $mem_free .'%)'),
    theme('cacherouter_admin_stats_key', t('Hits'), $stats['hits'] .' ('. $hits_hit .'%)', 
                                         t('Misses'), $stats['misses'] . ' ('. $hits_misses .'%)'),
    theme('cacherouter_admin_stats_key', t('Gets'), $stats['gets'] .' ('. $req_gets .'%)', 
                                         t('Sets'), $stats['sets'] .' ('. $req_sets .'%)'),
  );
 
  $output .= theme('table', $stats_header, $stats_table, $stats_attributes);

  $info_header = array(t('Cache Info'), t('Value'));
  $info_attributes = array('id' => 'cacherouter-info');
  $info_table = array();
  
  // Row 1 - Request Rate
  $info_table[] = array(
    t('Request Rate'),
    sprintf('%.2f %s', $stats['req_rate'], t('Requests / second')),
  );
  
  // Row 2 - Hit Rate
  $info_table[] = array(
    t('Hit Rate'),
    sprintf('%.2f %s', $stats['hit_rate'], t('Requests / second')),
  );

  // Row 3 - Miss Rate
  $info_table[] = array(
    t('Miss Rate'),
    sprintf('%.2f %s', $stats['miss_rate'], t('Requests / second')),    
  );
  
  // Row 4 - Set Rate
  $info_table[] = array(
    t('Set Rate'),
    sprintf('%.2f %s', $stats['set_rate'], t('Requests / second')),
  );
  
  $output .= theme('table', $info_header, $info_table, $info_attributes);
  $output .= '</div>';
  return $output;
}

function theme_cacherouter_admin_stats_key($key1, $value1, $key2, $value2) {
  $output  = '<div class="cr-key">';
  $output .= '<div class="cr-primary-1"></div>';
  $output .= '<span><strong>'. $key1 .':</strong> '. $value1 .'</span>';
  $output .= '</div>';
  $output .= '<div class="cr-key">';
  $output .= '<div class="cr-primary-2"></div>';
  $output .= '<span><strong>'. $key2 .':</strong> '. $value2 .'</span>';
  $output .= '</div>';

  return $output;
}

function _cacherouter_convert_size($fs) {
  $return = '';
  
  if ($fs >= 1073741824) {
    $return = round($fs / 1073741824 * 100) / 100 . " GB";
  } 
  else if ($fs >= 1048576) {
    $return = round($fs / 1048576 * 100) / 100 . " MB";
  } 
  else if ($fs >= 1024) {
    $return = round($fs / 1024 * 100) / 100 . " KB";
  } 
  else {
    $return = $fs . " Bytes";
  }
  
  return $return;
}