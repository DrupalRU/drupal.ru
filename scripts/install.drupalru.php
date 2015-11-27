#!/usr/bin/php
<?php
  
echo "This is install script to create dev environment for drupal.ru  code\n";

$data['github_path'] = get_promt_answer('GITHUB DIR');  
$data['site_path'] = get_promt_answer('DOCROOT');
$data['mysql_user'] = get_promt_answer('MySQL User');
$data['mysql_db'] = get_promt_answer('MySQL DB');
$data['mysql_pass'] = get_promt_answer('MySQL Password');

// Core version.
$data['core'] = 'drupal-7';

// Contrib modules list.
$data['contrib'] = 'acl bbcode bueditor captcha  comment_notify diff-7.x-3.x-dev fasttoggle geshifilter google_plusone gravatar imageapi noindex_external_links pathauto privatemsg simplenews smtp spambot tagadelic taxonomy_manager jquery_ui jquery_update token rrssb ajax_comments fontawesome transliteration libraries views xmlsitemap bootstrap_lite xbbcode ban_user';


echo "Full site path: " . $data['site_path'] . "\n";
echo "Site core: " . $data['core'] . "\n";
echo "Github DIR: " . $data['github_path'] . "\n";

print_r($data);

chdir($data['site_path']);

echo "Download DRUPAL.\n";
exec('drush dl ' . $data['core'] . ' --drupal-project-rename="drupal"');
exec('rsync -a ' . $data['site_path'] . '/drupal/ ' . $data['site_path']);
rmdir( $data['site_path'] . '/drupal');

echo "Install DRUPAL\n";

function get_promt_answer($promt){
  if (PHP_OS == 'WINNT' or !function_exists('readline')) {
    echo $promt .': ';
    $line = stream_get_line(STDIN, 1024, PHP_EOL);
  } else {
    $line = readline($promt . ': ');
  }
  return $line;
}
