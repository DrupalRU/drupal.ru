<?php

/**
 * @file
 * Sanitize dump of drupal.ru database.
 */

$rules = array(
  'actions',
  'block',
  'block_custom',
  'bueditor_editors',
  'bueditor_buttons',
  'contact',
  'date_formats',
  'date_format_type',
  'field_config',
  'field_config_instance',
  'filter',
  'filter_format',
  'image_effects',
  'image_styles',
  'languages',
  'locales_source',
  'locales_target',
  'menu_custom',
  'menu_links',
  'menu_router',
  'metatag_config',
  'node_type',
  'profile_field',
  'registry',
  'registry_file',
  'role',
  'role_permission',
  'simplenews_category',
  'sphinxmain', // view
  'system',
  'taxonomy_term_data',
  'taxonomy_term_hierarchy',
  'taxonomy_vocabulary',
  'trigger_assignments',
  'variable',
  'users' => array(array('uid', 0, '!=')),
);

$rules_keys = array_keys($rules);
foreach (db_find_tables('%') as $table) {
  if (!in_array($table, $rules_keys, TRUE) && !in_array($table, $rules, TRUE)) {
    print "TRUNCATE TABLE $table" . PHP_EOL;
    db_truncate($table)->execute();
  }
  elseif (isset($rules[$table]) && is_array($rules[$table])) {
    $query = db_delete($table);
    foreach ($rules[$table] as $condition) {
      switch (count($condition)) {
        case 2:
          $query->condition(reset($condition), next($condition));
          break;
        case 3:
          $query->condition(reset($condition), next($condition), next($condition));
          break;
      }
    }
    $query->execute();
    print str_replace(PHP_EOL, '', $query->__toString()) . PHP_EOL;
    unset($query);
  }
  else {
    print "Skip table $table" . PHP_EOL;
  }
}
