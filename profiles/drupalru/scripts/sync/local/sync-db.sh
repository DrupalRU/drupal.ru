#!/usr/bin/env sh

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

# Drush alias that given from command line like "@dru.dev"
DRUSH_ALIAS=$1
# Project web home dir
PROJECT=$(drush $DRUSH_ALIAS status | grep "Drupal root" | awk -F: '{ gsub(/ /, "", $2); print $2 }')
# Dump directory with specific date
DUMP_DIR=$PROJECT/sites/default/files/update/$(date +%Y%m%d%H%M%S)
# Web path to sql dump file.
DUMP_URL="https://drupal.ru/sites/default/files/drupalru-dump.sql.gz"
# File check for existence
DUMP_FILE_STATUS=$(curl --head --silent $DUMP_URL | head -n 1)
# Get user email from git config
GIT_EMAIL=$(git config --get user.email)


if [ -z "$PROJECT" ]; then
  sm "Drush alias \"$DRUSH_ALIAS\" or project directory \"$PROJECT\" is not found."
  exit 1;
fi

if echo "$DUMP_FILE_STATUS" | grep -q 404; then
  sm "Dump file is not found in $DUMP_URL"
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


exe "cd $DUMP_DIR"
exe "wget $DUMP_URL"
sm "zcat \"$DUMP_DIR/drupalru-dump.sql.gz\" | drush $DRUSH_ALIAS sqlc"
zcat "$DUMP_DIR/drupalru-dump.sql.gz" | drush $DRUSH_ALIAS sqlc
# Generate content
exe "drush $DRUSH_ALIAS en devel_generate -y"
exe "drush $DRUSH_ALIAS genu 10"
exe "drush $DRUSH_ALIAS genc 100 5 --types=blog"
exe "drush $DRUSH_ALIAS dis devel_generate -y"
exe "drush $DRUSH_ALIAS pm-uninstall devel_generate -y"
# Disable cache
exe "drush $DRUSH_ALIAS vset page_compression 0"
exe "drush $DRUSH_ALIAS vset preprocess_css 0"
exe "drush $DRUSH_ALIAS vset preprocess_js 0"

# Change site email.
if [ -z "$GIT_EMAIL" ]; then
  exe "drush $DRUSH_ALIAS vset site_mail $GIT_EMAIL"
fi

# Clear cache
exe "drush $DRUSH_ALIAS cc all"
# Reset password for root
exe "drush $DRUSH_ALIAS uli"
