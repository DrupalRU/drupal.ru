#!/usr/bin/env bash

cd "$HOME/"
wget http://drupal.ru/sites/default/files/drupalru-dump.sql.gz
drush sql-sync @dru.stage @dru.dev --no-dump --source-dump="$HOME/drupalru-dump.sql.gz" -y
drush @dru.dev en devel_generate -y
drush @dru.dev genu 10
drush @dru.dev genc 100 5 --types=blog
drush @dru.dev dis devel_generate -y
drush @dru.dev pm-uninstall devel_generate -y