<?php
  
/**
 * Implementation of hook_drush_command().
 */
function drupal_deploy_drush_command() {
  $items = array();

  $items['deploy-export'] = array(
    'callback' => 'drupal_deploy_export',
    'description' => dt('Export data from database.'),
    'examples' => array(
      'drush dde menu --name=navigation --file=navigation.file' => dt('Export navigation menu.'),
      'drush dde blocks --theme=garland' => dt('Export blocks.'),
      'drush dde filters' => dt('Export filters.'),
      'drush dde roles' => dt('Export roles.'),
      'drush dde node_types --type=blog' => dt('Export node type blog.'),
      'drush dde taxonomy --name=Forums' => dt('Export taxonomy vocabulary Forums.'),
      'drush dde forum' => dt('Export forum container links to taxonomy tid.'),
      'drush dde cck' => dt('Export cck fields.'),
      'drush dde users' => dt('Export users.'),
      'drush dde users --uid=1,2,3' => dt('Export users with uid=1,2,3.'),
      'drush dde users --rid=1,2,3' => dt('Export users roles rid=1,2,3.'),      
      'drush dde imagecache' => dt('Export imagecache presets.'),
      'drush dde nodes --nid=1,2,3' => dt('Export nodes 1,2,3 with comments and revisions.'),
      'drush dde nodes --type=forum' => dt('Export forum nodes with comments and revisions.'),
      'drush dde nodes --type=forum,book,page' => dt('Export forum,book,page nodes with comments and revisions.'),
      'drush dde language --name=ru --group=default' => dt('Export RU language translation.'),
      'drush dde comment_notify' => dt('Export comment_notify settings.'),
      'drush dde variables --name=superfish_%' => dt('Export superfish variables settings.'),
      'drush dde variables --name=comment_notify_%,node_notify_%' => dt('Export comment_notify variable settings.'),            
      'drush dde variables --name=pathauto_%' => dt('Export pathauto variable settings.'),            
      'drush dde aggregator' => dt('Export aggregator feed only.'),
      'drush dde rules' => dt('Export all rules.'),
      'drush dde book --nid=10' => dt('Export book with root node 10 menu.'),      
    ),
    'aliases' => array('dde'),
    'arguments' => array(
      'type' => 'The name of export type. Supports menu, blocks, filters, roles, node_types.',
    ),
    'options' => array(
      'file' => 'File to export data. File will be overwritten.',
      'name' => 'Variable, Language, Menu or Vocabulary name',
      'theme' => 'Theme name',
      'type' => 'Node type',
      'nid' => 'Node id or ids',
      'uid' => 'User id or ids',
      'rid' => 'Role id or ids',
      'group' => 'Translation group name',
      'status' => 'Status value'
    ),
  );

  $items['deploy-import'] = array(
    'callback' => 'drupal_deploy_import',
    'description' => dt('Import data to database.'),
    'examples' => array(
      'drush ddi menu --name=navigation --file=navigation.file' => dt('Import navigation menu.'),
      'drush ddi blocks' => dt('Import blocks.'),
      'drush ddi filters' => dt('Import filters.'),
      'drush ddi roles' => dt('Import roles.'),
      'drush ddi node_types --type=blog' => dt('Import node type blog.'),
      'drush ddi taxonomy --name=Forums' => dt('Import taxonomy vocabulary Forums.'),      
      'drush ddi forum' => dt('Import forum container links to taxonomy tid.'),
      'drush ddi cck' => dt('Import cck fields.'),
      'drush ddi users' => dt('Import users.'),
      'drush ddi imagecache' => dt('Import imagecache presets.'),
      'drush ddi nodes' => dt('Import nodes with comments and revisions.'),
      'drush ddi language --name=ru --mode=replace' => dt('Import RU language translation. Replace existing ones, new ones are added.'),
      'drush ddi language --name=ru --set-default=true' => dt('Import RU language translation. Replace existing ones, new ones are added.'),
      'drush ddi comment_notify' => dt('Export comment_notify settings.'),
      'drush ddi variables --name=superfish_%' => dt('Import superfish variables settings.'),      
      'drush ddi variables --name=comment_notify_%,node_notify_%' => dt('Import comment_notify variable settings.'),
      'drush ddi variables --name=pathauto_%' => dt('Import pathauto variable settings.'),  
      'drush ddi aggregator' => dt('Import aggregator feed only.'),
      'drush ddi rules' => dt('Import rules from file.'),
      'drush ddi book' => dt('Import book.'),
    ),
    'aliases' => array('ddi'),
    'arguments' => array(
      'type' => 'The name of import type. Supports menu, blocks, filters.',
    ),
    'options' => array(
      'file' => 'File to import data from.',
      'name' => 'Variable, Language, Menu or Vocabulary name',
      'theme' => 'Theme name',
      'type' => 'Node type',
      'nid' => 'Node id or ids', 
      'mode' => 'Import language mode. Values: add, replace',
      'group' => 'Translation group name',
      'set-default' => 'Set language as default',
    ),
  );

  $items['deploy-sync'] = array(
    'callback' => 'drupal_deploy_sync',
    'description' => dt('Sync database state from files.'),
    'examples' => array(
      'drush deploy-sync menu' => dt('Import menu changes from file storage to database.'),
    ),
    'aliases' => array('dds'),
  );

  $items['deploy-revert'] = array(
    'callback' => 'drupal_deploy_revert',
    'description' => dt('Sync database state from files.'),
    'examples' => array(
      'drush deploy-revert menu TIMESTAMP' => dt('Revert changes back to specified timestamp.'),
    ),
    'aliases' => array('dds'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function drupal_deploy_drush_help($section) {
  switch ($section) {
    case 'drush:deploy-export':
      return dt('Export data from database.');
    case 'drush:deploy-import':
      return dt('Import navigation menu to database.');
    case 'drush:deploy-sync':
      return dt('Sync database state from files.');
    case 'drush:deploy-revert':
      return dt('Sync database state from files.');
  }
}

/**
 * Drush callback to perform export.
 */
function drupal_deploy_export($type = '') {
  $filename = drush_get_option('file');
  ini_set('memory_limit', '1G');
  switch($type){
    case 'menu':
      $menu_name = drush_get_option('name');
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_menu.inc');
      drupal_deploy_export_menu($menu_name, $filename);
      break;
    case 'blocks':
      $theme_name = drush_get_option('theme');
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_blocks.inc');
      drupal_deploy_export_blocks($theme_name, $filename);
      break;
    case 'filters':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_filters.inc');
      drupal_deploy_export_filters($filename);
      break;
    case 'roles':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_roles.inc');
      drupal_deploy_export_roles($filename);
      break;
    case 'node_types':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_node_types.inc');
      include_once(drupal_get_path('module', 'node') .'/content_types.inc');
      drupal_deploy_export_node_types($filename);
      break;      
    case 'taxonomy':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_taxonomy.inc');
      drupal_deploy_export_taxonomy($filename);
      break;
    case 'forum':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_forum.inc');
      drupal_deploy_export_forum($filename);
      break;
    case 'cck':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_cck.inc');
      drupal_deploy_export_cck($filename);
      break;
    case 'users':
      include_once(drupal_get_path('module', 'user') .'/user.pages.inc');
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_users.inc');
      drupal_deploy_export_users($filename);
      break;
    case 'imagecache':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_imagecache.inc');
      drupal_deploy_export_imagecache($filename);
      break;
    case 'nodes':
      include_once(drupal_get_path('module', 'node') .'/node.pages.inc');
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_nodes.inc');
      drupal_deploy_export_nodes($filename);
      break;
    case 'language':
      include_once('includes/locale.inc');
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_language.inc');
      drupal_deploy_export_language($filename);
      break;
    case 'variables':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_variables.inc');
      drupal_deploy_export_variables($filename);
      break;
    case 'aggregator':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_aggregator.inc');
      drupal_deploy_export_aggregator($filename);
      break;
    case 'rules':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_rules.inc');
      drupal_deploy_export_rules($filename);
      break;
    case 'book':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_book.inc');
      drupal_deploy_export_book($filename);
      break;
    default:
      if(module_exists($type)) {
        $fileinc = drupal_get_path('module', $type) .'/' . $type. '.dde.inc';
        include_once($fileinc);
        $func_name = $type . '_dde_fetch_data';
        if(function_exists($func_name)){
          $type_items = $func_name();
          
          $filename = $type . '.export';
          if($files = dde_export_files($filename)){
            $items = array(
              'data' => $type_items,
              'files' => $files,
            );
          }else{
            $items = $type_items;
          }

          drupal_deploy_export_save($items, $filename);
        }
      }
  }
}

/**
 * Drush callback to perform import.
 */
function drupal_deploy_import($type = '', $option = '') {
  $filename = drush_get_option('file');
  ini_set('memory_limit', '1G');
  switch($type){
    case 'menu':
      $menu_name = drush_get_option('name');
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_menu.inc');
      drupal_deploy_import_menu($filename, $menu_name);
      break;
    case 'blocks':
      $theme_name = drush_get_option('theme');
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_blocks.inc');
      drupal_deploy_import_blocks($filename, $theme_name);
      break; 
    case 'filters':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_filters.inc');
      drupal_deploy_import_filters($filename);
      break;      
    case 'roles':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_roles.inc');
      drupal_deploy_import_roles($filename);
      break;
    case 'node_types':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_node_types.inc');
      include_once(drupal_get_path('module', 'node') .'/content_types.inc');
      drupal_deploy_import_node_types($filename);
      break;
    case 'taxonomy':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_taxonomy.inc');
      drupal_deploy_import_taxonomy($filename);
      break;
    case 'forum':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_forum.inc');
      drupal_deploy_import_forum($filename);
      break;
    case 'cck':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_cck.inc');
      drupal_deploy_import_cck($filename);
      break;  
    case 'users':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_users.inc');
      drupal_deploy_import_users($filename);
      break;
    case 'imagecache':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_imagecache.inc');
      drupal_deploy_import_imagecache($filename);
      break;
    case 'nodes':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_nodes.inc');
      drupal_deploy_import_nodes($filename);
      break;
    case 'language':
      include_once('includes/locale.inc');    
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_language.inc');
      drupal_deploy_import_language($filename);
      break;
    case 'variables':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_variables.inc');
      drupal_deploy_import_variables($filename);
      break;
    case 'aggregator':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_aggregator.inc');
      drupal_deploy_import_aggregator($filename);
      break;
    case 'rules':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_rules.inc');
      drupal_deploy_import_rules($filename);
      break;  
    case 'book':
      include_once(drupal_get_path('module', 'drupal_deploy') .'/includes/drupal_deploy_book.inc');
      drupal_deploy_import_book($filename);
      break;
    default:
      if(module_exists($type)) {
        $fileinc = drupal_get_path('module', $type) .'/' . $type. '.ddi.inc';
        include_once($fileinc);
        $func_name = $type . '_ddi_data';
        if(function_exists($func_name)){
          if(empty($filename)){
            $filename = $type . '.export';
          }
          if(!is_file($filename)){
            drush_log(dt('File !filename does not exists', array('!filename' => $filename)), 'error');
            return FALSE;
          }
          $data = file_get_contents($filename);
          if(!$data = json_decode($data, TRUE)){
            drush_log(dt('Json decode error!'), 'error');
            return FALSE;
          }
          if(isset($data['data']) && isset($data['files'])){
            ddi_import_files($data['files'], $filename);
            $data = $data['data'];
          }
          $func_name($data);
        }
      }
  }
}

/**
 * Drush callback to perform sync.
 */
function drupal_deploy_sync() {
  $args = func_get_args();
}

/**
 * Drush callback to perform revert.
 */
function drupal_deploy_revert() {
  $args = func_get_args();
}


/**
 * Export files to folder, if provided.
 */
function dde_export_files($filename){
  $items = array();
  $files = dde_files_to_export('get');
  if(!empty($files)){
    $export_folder = $filename . '.files';
    if(!is_dir($export_folder)){
      mkdir($export_folder);
    }
    
    $file_directory_path = variable_get('file_directory_path', conf_path() . '/files');
    $file_directory_path_len = strlen($file_directory_path);
    
    foreach($files as $file){
      if(is_file($file->filepath)){
        
        if(0 == strcmp($file_directory_path, substr($file->filepath, 0,$file_directory_path_len))){
          $store_filepath = $export_folder . '/' . substr($file->filepath, $file_directory_path_len + 1);
        }else{
          $store_filepath = $export_folder . '/' . basename($file->filepath);
        }
        
        $info = pathinfo($store_filepath);
        if(!is_dir($info['dirname'])){
          mkdir($info['dirname'], 0750, TRUE); 
        }
        
        copy($file->filepath, $store_filepath);
        $file->stored_filepath = $store_filepath;
        $items[] = $file;
        drush_log(dt('!filename exported', array('!filename' => $file->filename)),'notice');
      }else{
        drush_log(dt('!filename does not exists', array('!filename' => $file->filename)), 'warning');
      }
    }
  }
  return $items;
}

/**
 * Import files to database, if provided.
 */
function ddi_import_files($files, $filename){
  $info = pathinfo($filename);
  $dir = $info['dirname'];
  if(!is_dir($dir)){
    drush_log(dt('File folder !folder does not exists', array('!folder' => $dir)),'error');
    return FALSE;
  }

  foreach($files as $file){
    $file['uid'] = ddi_get_user_id($file['username']);
    if($stored_file = db_fetch_array(db_query("SELECT * FROM {files} WHERE filepath='%s'", $file['filepath']))){
      print_r($stored_file);
      drush_log(dt('!filename already stored. No changes', array('!filename' => $stored_file['filename'])));
      dde_files_imported('set', $stored_file);
    }else{
      $info = pathinfo($file['filepath']);
      if(!is_dir($info['dirname'])){
        drush_log(dt('Create dir !dirname.', array('!dirname' => $info['dirname'])));
        mkdir($info['dirname'], 0751, TRUE); 
      }
      drush_log(dt('Copy !from to !to.', array('!from' => $dir . '/' .$file['stored_filepath'], '!to' => $file['filepath'])));
      copy($dir . '/' .$file['stored_filepath'], $file['filepath']);
      
      drupal_write_record('files', $file);
      dde_files_imported('set', $file);
      
      drush_log(dt('!filename imported.', array('!filename' => $file['filename'])), 'notice');
    }
  }
}

