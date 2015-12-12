#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Issue #95

echo "Activate module: user_filter"

ln -s $GITLC_DEPLOY_DIR/modules/user_filter $SITEPATH/sites/all/modules/local/

drush  en user_filter -y
drush  en user_filter_notify -y

echo "Clean cache"
drush cc all
