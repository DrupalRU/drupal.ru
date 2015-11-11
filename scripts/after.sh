#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Fix view node revisions - issue #58
 
drush dl quote-7.x-1.x-dev -y
drush en quote

echo "Clean cache"
drush cc all
