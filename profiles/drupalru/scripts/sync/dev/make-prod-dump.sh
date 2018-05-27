#!/usr/bin/env sh

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
exe "drush sql-sync @dru.prod @dru.temp -y"
exe "drush @dru.temp scr profiles/drupalru/scripts/sync/dev/sanitize.php"
sh "Create dump"
drush @dru.temp sql-dump --result-file=$DRUPALRU_DEV_DUMP --gzip
