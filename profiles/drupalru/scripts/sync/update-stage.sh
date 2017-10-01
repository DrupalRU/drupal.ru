#!/usr/bin/env bash

cd "$HOME/"
wget http://drupal.ru/sites/default/files/drupalru-dump.sql.gz ./
drush sql-sync default @dru.stage --no-dump --source-dump="$HOME/drupalru-dump.sql.gz" -y
drush @dru.stage en devel_generate -y
drush @dru.stage genu 10
drush @dru.stage genc 100 5 --types=blog
drush @dru.stage dis devel_generate -y
drush @dru.stage pm-uninstall devel_generate -y