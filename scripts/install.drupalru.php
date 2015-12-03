<?php

echo "This is install script to create dev environment for drupal.ru  code";

$data['github_path'] = get_promt_answer('GITHUB DIR');
$data['site_path'] = get_promt_answer('DOCROOT');
$data['mysql_user'] = get_promt_answer('MySQL User');
$data['mysql_db'] = get_promt_answer('MySQL DB');
$data['mysql_pass'] = get_promt_answer('MySQL Password');
$data['domain'] = get_promt_answer('Domain');
$data['account_name'] = get_promt_answer('Drupal User name');
$data['account_email'] = get_promt_answer('Drupal User email');
$data['account_pass'] = get_promt_answer('Drupal User Password');

// Core version.
$data['core'] = 'drupal-7';
$data['site_name'] = 'Drupal.ru Dev version';

// Contrib modules list.
$data['contrib'] = 'acl bbcode bueditor captcha  comment_notify diff-7.x-3.x-dev fasttoggle geshifilter google_plusone' .
  ' gravatar imageapi noindex_external_links pathauto privatemsg simplenews smtp spambot tagadelic taxonomy_manager' .
  ' jquery_update token rrssb ajax_comments fontawesome transliteration libraries views xmlsitemap bootstrap_lite' .
  ' xbbcode ban_user quote-7.x-1.x-dev';


echo "Full site path: " . $data['site_path'];
echo "Site core: " . $data['core'];
echo "Github DIR: " . $data['github_path'];
chdir($data['site_path']);

echo "Download DRUPAL.";
exec('drush dl ' . $data['core'] . ' --drupal-project-rename="drupal"');
exec('cp -R ' . $data['site_path'] . '/drupal/* ' . $data['site_path'] . '/.');
exec('rm -rf ' . $data['site_path'] . '/drupal');

echo "Install DRUPAL";

exec('drush site-install standard -y --root=' . $data['site_path'] . ' --account-name=' . $data['account_name']
  . ' --account-mail=' . $data['account_email'] . ' --account-pass=' . $data['account_pass'] . ' --uri=http://'
  . $data['domain'] . ' --site-name="' . $data['site_name'] . '" --site-mail=' . $data['account_email']
  . ' --db-url=mysql://' . $data['mysql_user'] . ':' . $data['mysql_pass'] . '@localhost/' . $data['mysql_db']);

echo "make libraries dir";
if (!is_dir($data['site_path'] . '/sites/all/libraries')) {
  mkdir($data['site_path'] . '/sites/all/libraries', 0755, TRUE);
}

echo "Install contrib modules";
if (!is_dir($data['site_path'] . '/sites/all/modules/contrib')) {
  mkdir($data['site_path'] . '/sites/all/modules/contrib', 0755, TRUE);
}

exec('drush dl ' . $data['contrib']);
exec('drush en -y ' . $data['contrib']);

echo "Download geshi library";
chdir($data['site_path'] . '/sites/all/libraries');
exec('wget \'http://sourceforge.net/projects/geshi/files/geshi/GeSHi%201.0.8.10/GeSHi-1.0.8.10.tar.gz/download\'' .
  ' -O geshfilter.tar.gz');
exec('tar -xzpf geshfilter.tar.gz');
exec('rm -f geshfilter.tar.gz');

echo "Install captcha_pack";
exec('drush dl captcha_pack');
exec('drush -y en ascii_art_captcha css_captcha');


echo "Install other modules";
exec('drush -y en imageapi_imagemagick pm_block_user pm_email_notify privatemsg_filter  views_ui book forum');


echo "Prepare github based modules dir";
if (!is_dir($data['site_path'] . '/sites/all/modules/github')) {
  mkdir($data['site_path'] . '/sites/all/modules/github', 0755, TRUE);
}

echo "Install inner poll ";
chdir($data['site_path'] . '/sites/all/modules/github');
exec('git clone --branch master http://git.drupal.org/sandbox/andypost/1413472.git inner_poll');

chdir($data['site_path'] . '/sites/all/modules/github/inner_poll');
exec('git checkout 7.x-1.x');

echo "Deploy module ";
chdir($data['site_path'] . '/sites/all/modules/github');
exec('git clone https://github.com/itpatrol/drupal_deploy.git');

chdir($data['site_path'] . '/sites/all/modules/github/drupal_deploy');
exec('git checkout 7.x');

echo "Altpager";
chdir($data['site_path'] . '/sites/all/modules/github');
exec('git clone https://github.com/itpatrol/altpager');

echo "Alttracker";
chdir($data['site_path'] . '/sites/all/modules/github');
exec('git clone https://github.com/itpatrol/alttracker');

chdir($data['site_path']);
exec('drush -y en inner_poll altpager alttracker drupal_deploy');

echo "Install drupal.ru modules";
if (!is_dir($data['site_path'] . '/sites/all/modules/local')) {
  mkdir($data['site_path'] . '/sites/all/modules/local', 0755, TRUE);
}

exec('ln -s ' . $data['github_path'] . '/modules/* ' . $data['site_path'] . '/sites/all/modules/local/');

echo "Install Latest Font awesome";
chdir($data['site_path'] . '/sites/all/libraries');
// Remove predownloaded fontawesome by installation process
exec('rm -rf ' . $data['site_path'] . '/sites/all/libraries/fontawesome');
exec('git clone https://github.com/FortAwesome/Font-Awesome.git fontawesome');

echo "Install drupal.ru themes";
if (!is_dir($data['site_path'] . '/sites/all/themes/local')) {
  mkdir($data['site_path'] . '/sites/all/themes/local', 0755, TRUE);
}

exec('ln -s ' . $data['github_path'] . '/themes/* ' . $data['site_path'] . '/sites/all/themes/local/');

echo "Set default theme";
chdir($data['site_path']);
exec('drush -y en alpha');

echo "Set default variables";
exec('drush vset theme_default alpha');
exec('drush vset filestore_tmp_dir /tmp');
exec('drush vset admin_theme alpha');

echo "Import META structure via module http://github.com/itpatrol/drupal_deploy.";

echo "Import roles";

exec('drush ddi roles --file=' . $data['github_path'] . '/data/roles.export');

echo "Import filters";
exec('drush ddi filters --file=' . $data['github_path'] . '/data/filters.export');

echo "Import nodetypes";
exec('drush ddi node_types --file=' . $data['github_path'] . '/data/blog.node_types.export');

echo "Import taxonomy";
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_1.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_2.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_3.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_4.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_5.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_7.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_8.taxonomy.export');
exec('drush ddi taxonomy --file=' . $data['github_path'] . '/data/vocabulary_10.taxonomy.export');

echo "Import forum";
exec('drush ddi forum --file=' . $data['github_path'] . '/data/forum.export');

echo "Import menu structure";
exec('drush ddi menu --file=' . $data['github_path'] . '/data/main-menu.menu_links.export');
exec('drush ddi menu --file=' . $data['github_path'] . '/data/user-menu.menu_links.export');

echo "Import theme settings";
exec('drush ddi variables --file=' . $data['github_path'] . '/data/theme_alpha_settings.variables.export');

echo "Disable drupal_deploy";
exec('drush dis -y drupal_deploy');

echo "Generate content and users";
exec('drush dl devel');
exec('drush -y en devel devel_generate');
exec('drush generate-users 100');
exec('drush generate-content 100 100');

echo "Disable toolbar, overlay modules";
exec('drush dis -y overlay, toolbar');

function get_promt_answer($promt) {
  if (PHP_OS == 'WINNT' or !function_exists('readline')) {
    echo $promt . ': ';
    $line = stream_get_line(STDIN, 1024, PHP_EOL);
  }
  else {
    $line = readline($promt . ': ');
  }
  return $line;
}
