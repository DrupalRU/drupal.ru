#!/usr/bin/env sh

set -e
. ../../helpers.sh

exe "drush @dru.prod cc all"
exe "drush @dru.prod wd-del all -y"
exe "drush sql-sync @dru.prod @dru.temp --sanitize -y"
exe "drush @dru.temp scr profiles/drupalru/scripts/sync/local/sanitize.php"
exe "drush @dru.temp ucrt admin --password=111"
exe "drush @dru.temp sql-dump --result-file=$DRUPALRU_LOCAL_DUMP --gzip"
