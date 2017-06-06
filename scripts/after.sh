#!/bin/sh

SITEPATH="$HOME/domains/$DOMAIN"

# Please put your script into scripts/update/
# It will be called when deployed only once.


echo "Process new files"
sh $ZENCI_DEPLOY_DIR/scripts/update.sh

echo "Clean cache"
drush cc all
