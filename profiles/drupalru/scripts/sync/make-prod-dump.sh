#!/usr/bin/env bash

drush @dru.prod cc all
drush @dru.prod wd-del all -y
drush sql-sync @dru.prod @dru.temp --sanitize -y
drush @dru.temp scr "profiles/drupalru/scripts/sync/sanitize.php"
drush @dru.temp ucrt admin --password=111
drush @dru.temp sql-dump --result-file="$HOME/domains/drupal.ru/sites/default/files/drupalru-dump.sql" --gzip
