#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

# implement #294 xbbcode
ln -s $GITLC_DEPLOY_DIR/modules/xbbcode_dru $SITEPATH/sites/all/modules/local/

drush -y en xbbcode_dru

echo "Clean cache"
drush cc all
