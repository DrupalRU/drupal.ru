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

if [ -z $(which drush) ]; then
  sm "\"drush\" program need to be installed."
  exit 1;
fi

DRUSH_ALIAS=$1
PROJECT=$(drush $DRUSH_ALIAS status | grep "Drupal root" | awk -F: '{ gsub(/ /, "", $2); print $2 }')
DUMP_DIR=$HOME/sync-from-prod/$(date +%Y%m%d%H%M%S)
DUMP_FILE_STATUS=$(curl --head --silent $DRUPALRU_DEV_DUMP_SOURCE | head -n 1)

if [ -z "$PROJECT" ]; then
  sm "Drush alias \"$DRUSH_ALIAS\" or project directory \"$PROJECT\" is not found."
  exit 1;
fi

if echo "$DUMP_FILE_STATUS" | grep -q 404; then
  sm "Dump file is not found on Prod server"
  exit 1;
fi

if [ ! -d "$DUMP_DIR" ]; then
  exe "mkdir -p $DUMP_DIR"
  if [ ! -d "$DUMP_DIR" ]; then
    sm "Can not create folder \"$DUMP_DIR\""
    exit 1;
  fi
fi

if [ -z $(which zcat) ]; then
  sm "Zcat program need to be installed using \"apt-get install zcat\""
  exit 1;
fi

. ~/.profile
exe "cd $DUMP_DIR"
sm "Downloading dump from source..."
wget -O drupalru-dump.sql.gz $DRUPALRU_DEV_DUMP_SOURCE
exe "drush $DRUSH_ALIAS sql-drop -y"
sm "zcat \"$DUMP_DIR/drupalru-dump.sql.gz\" | drush $DRUSH_ALIAS sqlc"
zcat "$DUMP_DIR/drupalru-dump.sql.gz" | drush $DRUSH_ALIAS sqlc
exe "drush $DRUSH_ALIAS en devel_generate -y"
exe "drush $DRUSH_ALIAS genu 10"
exe "drush $DRUSH_ALIAS genc 20 5 --types=blog"
exe "drush $DRUSH_ALIAS dis devel_generate -y"
exe "drush $DRUSH_ALIAS pm-uninstall devel_generate -y"
exe "drush $DRUSH_ALIAS vset page_compression 0"
exe "drush $DRUSH_ALIAS vset preprocess_css 0"
exe "drush $DRUSH_ALIAS vset preprocess_js 0"
exe "drush $DRUSH_ALIAS vset site_mail dev@drupal.ru"
exe "drush $DRUSH_ALIAS cc all"

rm -rf "$DUMP_DIR"