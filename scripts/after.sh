#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#mini update #102
ln -s $GITLC_DEPLOY_DIR/modules/dru_forum $SITEPATH/sites/all/modules/local/

drush en -y dru_forum

echo "Clean cache"
drush cc all
