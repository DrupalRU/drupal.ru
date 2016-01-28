#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

# Please put your script into scripts/update/
# It will be called when deployed only once.


echo "Process new files"
sh $GITLC_DEPLOY_DIR/update.sh

echo "Clean cache"
drush cc all
