#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Issue #167 replace favicon

echo "Activate module: dru_comment_quote"

ln -s $GITLC_DEPLOY_DIR/modules/dru_comment_quote $SITEPATH/sites/all/modules/local/

drush  en dru_comment_quote -y
drush  en smtp -y
drush  en htmlmail -y
drush  en mimemail -y

echo "Clean cache"
drush cc all
