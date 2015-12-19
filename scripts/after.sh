#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#update 25 dec 2015

ln -s $GITLC_DEPLOY_DIR/modules/validate_api $SITEPATH/sites/all/modules/local/
drush -y en user_filter user_filter_notify validate_api antinoob_validate antiswearing_validate


echo "Clean cache"
drush cc all
