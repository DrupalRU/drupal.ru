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

$hash = hash('sha256', microtime() . '' . rand());
$clean_variables = array(
  'values' => array(
    'abuse_warn_bcc' => 's:19:"mail@drupal.loc";',
    'site_mail' => 's:19:"mail@drupal.loc";',
    'pm_email_notify_from' => 's:15:"mail@drupal.loc";',

    'captcha_token' => sprintf('s:32:"%s";', substr($hash, 0, 32)),
    'cron_key' => sprintf('s:64:"%s";', $hash),
    'drupal_private_key' => sprintf('s:64:"%s";', $hash),
    'simplenews_private_key' => sprintf('s:32:"%s";', substr($hash, 0, 32)),
    'spambot_sfs_api_key' => sprintf('s:14:"%s";', substr($hash, 0, 14)),
    'token' => sprintf('s:32:"%s";', substr($hash, 0, 32)),
  ),
  'drop' => array(
    'abuse_%',
    'birthdays_%',
    'color_garland_%',
    'druid_%',
    'googleajaxsearch_%',
    'googleanalytics_%',
    'googlemap_%',
    'listhandler_%',
    'mibbit_%',
    'mysite_%',
    'pearwiki_%',
    'postcard_%',
    'recaptcha_%',
    'relativity_%',
    'reptag_%',
    'signature_%',
    'site_user_%',
    'user_relationship_%',
    'user_relationships_%',
    'xtemplate_%',
  ),
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


print '####### Clean Variables #######' . PHP_EOL;
foreach ($clean_variables['drop'] as $variable => $value) {
  $query = db_delete('variable')->condition('name', $variable, 'LIKE');
  $query->execute();
  print str_replace(PHP_EOL, '', $query->__toString()) . PHP_EOL;
}

print '####### Change sensitive data #######' . PHP_EOL;
foreach ($clean_variables['values'] as $variable => $value) {
  $query = db_update('variable')
    ->fields(array('value' => $value))
    ->condition('name', $variable);
  $query->execute();
  print str_replace(PHP_EOL, '', $query->__toString()) . PHP_EOL;
}