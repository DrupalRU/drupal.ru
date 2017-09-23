<?php
  
function drupal_deploy_export_language($filename = ''){
  $language = drush_get_option('name', 'en');
  $group = drush_get_option('group', 'default');

  $data = drupal_deploy_export_language_fetch_data($language, $group);
  
  if(empty($filename)){
    $filename = $group . '.' .$language . '.language.po';  
  }
  if(file_put_contents($filename, $data)){
    drush_log(dt('Exported to file !filename', array('!filename' => $filename)), 'ok');
  }
}

function drupal_deploy_import_language($filename = ''){
  $language = drush_get_option('name', 'en');
  $group = drush_get_option('group', 'default');
  $mode = drush_get_option('mode', 'add');
  
  $set_default = drush_get_option('set-default', FALSE);
  
  if($mode == 'add'){
    $mode = LOCALE_IMPORT_KEEP;
  }else{
    $mode = LOCALE_IMPORT_OVERWRITE;
  }
  
  if(empty($filename)){
    $filename = $group . '.' .$language . '.language.po';  
  }

  if(!is_file($filename)){
    drush_log(dt('File !filename does not exists', array('!filename' => $filename)), 'error');
    return FALSE;
  }
  
  $languages = language_list('language', TRUE);
  if (!isset($languages[$language])) {
    $predefined = _locale_get_predefined_list();
    locale_add_language($language);
    drush_log(dt('The language %language has been created.', array('%language' => t($predefined[$language][0]))));
  }
  $file->filename = $filename;
  $file->filepath = $filename;  
  if ($ret = _locale_import_po($file, $language, $mode, $group) == FALSE) {
    $variables = array('%filename' => $filename);
    drush_log(dt('The translation import of %filename failed.', $variables), 'error');
  }else{
    drush_log(dt('Translation imported !lancode.', array('!lancode' => $language)), 'success');
  }
  
  if($set_default){
    $language = drush_get_option('name', 'en');
    $languages = language_list('language', TRUE);
    variable_set('language_default', $languages[$language]);
    cache_clear_all('*', 'cache_page', TRUE);
    drush_log(dt('Set default language !lancode.', array('!lancode' => $language)), 'success');
  }
}


function drupal_deploy_export_language_fetch_data($language, $group){
  
  $languages = language_list();
  $language = $languages[$language];
  return _locale_export_po_generate($language, _locale_export_get_strings($language, $group));
}
