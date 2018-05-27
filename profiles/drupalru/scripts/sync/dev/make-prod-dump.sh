#!/usr/bin/env sh

set -e
. ../../helpers.sh

exe "drush @dru.prod cc all"
exe "drush @dru.prod wd-del all -y"
exe "drush sql-sync @dru.prod @dru.temp -y"
exe "drush @dru.temp scr profiles/drupalru/scripts/sync/dev/sanitize.php"
exe "mkdir -p $DRUPALRU_DEV_DUMP"
exe "drush @dru.temp sql-dump --result-file=$DRUPALRU_DEV_DUMP --gzip"
