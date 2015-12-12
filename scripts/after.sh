#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Issue #192

echo "Activate module: disable_login"

ln -s $GITLC_DEPLOY_DIR/modules/disable_login $SITEPATH/sites/all/modules/local/

drush  en disable_login -y

echo "Clean cache"
drush cc all
