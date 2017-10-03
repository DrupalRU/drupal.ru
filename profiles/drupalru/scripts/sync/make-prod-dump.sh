#!/usr/bin/env sh

sm() {
  echo "\033[0;32m$@\033[0m";
}

exe() {
  sm "$@";
  $@;
}

DUMP_DIR=$HOME/domains/drupal.ru/sites/default/files/drupalru-dump.sql

exe "drush @dru.prod cc all"
exe "drush @dru.prod wd-del all -y"
exe "drush sql-sync @dru.prod @dru.temp --sanitize -y"
exe "drush @dru.temp scr profiles/drupalru/scripts/sync/sanitize.php"
exe "drush @dru.temp ucrt admin --password=111"
exe "drush @dru.temp sql-dump --result-file=$DUMP_DIR --gzip"
