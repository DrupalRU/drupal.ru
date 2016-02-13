<?php

/**
 * Get user input for variables.
 */
function get_promt_answer($promt) {
  if (PHP_OS == 'WINNT' or !function_exists('readline')) {
    echo $promt .': ';
    $line = stream_get_line(STDIN, 1024, PHP_EOL);
  } else {
    $line = readline($promt . ': ');
  }
  return $line;
}

echo "This is install script to create dev environment for drupal.ru  code\n";

$data['github_url'] = get_promt_answer("Provide url to your drupal.ru fork. \nExample:https://github.com/DrupalRu/drupal.ru\n");
$data['github_branch'] =get_promt_answer("Branch name");

$data['site_path'] = get_promt_answer('DOCROOT');
$data['mysql_host'] = get_promt_answer('MySQL Host');
$data['mysql_user'] = get_promt_answer('MySQL User');
$data['mysql_db'] = get_promt_answer('MySQL DB');
$data['mysql_pass'] = get_promt_answer('MySQL Password');
$data['domain'] = get_promt_answer('Domain');
$data['account_name'] = get_promt_answer('Drupal User name');
$data['account_email'] = get_promt_answer('Drupal User email');
$data['account_pass'] = get_promt_answer('Drupal User Password');

// Some static variables.
$data['site_name'] = 'Drupal.ru Dev version';
$data['github_path'] = 'profiles/drupalru';

echo "Full site path: " . $data['site_path'] . "\n";
echo "Github DIR: " . $data['github_path'] . "\n";

chdir($data['site_path']);

echo "Download DRUPAL.\n";

exec('drush -y make https://raw.githubusercontent.com/DrupalRu/drupal.ru/stage/scripts/drupalru.make');

exec('git clone -b  ' . $data['github_branch'] . ' ' . $data['github_url'] . ' profiles/drupalru');

echo "Install DRUPAL\n";

exec('drush site-install drupalru -y --root=' . $data['site_path'] . ' --account-name=' . $data['account_name'] . ' --account-mail=' . $data['account_email'] . ' --account-pass=' . $data['account_pass'] . ' --uri=http://' . $data['domain'] . ' --site-name="' . $data['site_name'] . '" --site-mail=' . $data['account_email'] . ' --db-url=mysql://' . $data['mysql_user'] . ':' . $data['mysql_pass'] . '@' . $data['mysql_host'] . '/' . $data['mysql_db']);


chdir($data['site_path']);


echo "Import META structure via module http://github.com/itpatrol/drupal_deploy.\n";

echo "Import roles\n";

exec('drush ddi roles --file=' . $data['github_path'] . '/data/roles.export');

echo "Import filters\n";
exec('drush ddi filters --file=' . $data['github_path'] . '/data/filters.export');

echo "Import nodetypes\n";
exec('drush ddi node_types --file=' . $data['github_path'] . '/data/blog.node_types.export');
exec('drush ddi node_types --file=' . $data['github_path'] . '/data/organization.node_types.export');
exec('drush ddi node_types --file=' . $data['github_path'] . '/data/simple_event.node_types.export');
exec('drush ddi node_types --file=' . $data['github_path'] . '/data/ticket.node_types.export');

echo "Import taxonomy\n";
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_1.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_2.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_3.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_4.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_5.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_7.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_8.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_10.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/claim_category.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/event_types.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/organizations.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/ticket_status.taxonomy.export');

echo "Import forum\n";
exec('drush ddi forum --file=' . $data['github_path'] . '/data/forum.export');

echo "Import menu structure\n";
exec('drush ddi menu --file=' . $data['github_path'] . '/data/main-menu.menu_links.export');
exec('drush ddi menu --file=' . $data['github_path'] . '/data/user-menu.menu_links.export');

echo "Import theme blocks settings\n";
exec('drush ddi blocks --file=' . $data['github_path'] . '/data/alpha.blocks.export');


echo "Import theme settings\n";
exec('drush ddi variables --file=' . $data['github_path'] . '/data/theme_settings.variables.export');
exec('drush ddi variables --file=' . $data['github_path'] . '/data/theme_alpha_settings.variables.export');


echo "Import modules settings";

exec('drush ddi variables --file=' . $data['github_path'] . '/data/advanced_sphinx.variables.export');
exec('drush ddi variables --file=' . $data['github_path'] . '/data/darkmatter_notify.variables.export');
exec('drush ddi variables --file=' . $data['github_path'] . '/data/dru_frontpage.variables.export');
exec('drush ddi variables --file=' . $data['github_path'] . '/data/resolve_can.variables.export');
exec('drush ddi variables --file=' . $data['github_path'] . '/data/user_info_notify.variables.export');
exec('drush ddi variables --file=' . $data['github_path'] . '/data/quote.variables.export');
exec('drush ddi variables --file=' . $data['github_path'] . '/data/validate_api.variables.export');


echo "Disable drupal_deploy\n";
exec('drush dis -y drupal_deploy');

echo "Generate content and users\n";
exec('drush generate-users 100');
exec('drush generate-content 100 100');


echo "Update translation\n";

exec('drush -y dl drush_language');
exec('drush language-add ru');
exec('drush language-default ru');
exec('drush -y l10n-update-refresh');
exec('drush -y l10n-update');

exec('drush -y language-import ru ' . $data['github_path'] . '/modules/user_filter/user_filter_notify/translations/user_filter_notify.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . '/modules/validate_api/translations/validate_api.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . '/modules/validate_api/antiswearing_validate/translations/antiswearing_validate.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . '/modules/validate_api/antinoob_validate/translations/antinoob_validate.ru.po');

exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/darkmatter/translations/darkmatter.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/dru_tickets/dru_claim/translations/dru_claim.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/dru_tickets/translations/dru_tickets.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/simple_events/translations/simple_events.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/user_filter/user_filter_notify/translations/user_filter_notify.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/resolve/translations/resolve.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/marketplace/translations/marketplace.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/dru_tnx/translations/dru_tnx.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/validate_api/translations/validate_api.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/validate_api/antiswearing_validate/translations/antiswearing_validate.ru.po');
exec('drush -y language-import ru ' . $data['github_path'] . 'profiles/drupalru/modules/validate_api/antinoob_validate/translations/antinoob_validate.ru.po');

