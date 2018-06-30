#!/bin/sh

set -e

CI_COLOR='\033[1;32m'
NO_COLOR='\033[0m'
sm() {
  echo "";
  echo "${CI_COLOR}$@${NO_COLOR}";
}

exe() {
  sm "$@";
  $@;
}

exe "drush @dru.prod cc all"
exe "drush @dru.prod wd-del all -y"
exe "drush sql-sync @dru.prod @dru.temp --sanitize --skip-tables-list=sphinxmain -y"
exe "drush @dru.temp scr profiles/drupalru/scripts/sync/local/sanitize.php"
exe "drush @dru.temp ucrt admin --password=111"
exe "drush @dru.temp sql-dump --result-file=$HOME/domains/drupal.ru/sites/default/files/drupalru-dump.sql --gzip"
