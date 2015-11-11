#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Fix view node revisions - issue #58
drush dis quote -y
rm -f $SITEPATH/sites/all/modules/local/quote
 
drush dl quote-7.x-1.x-dev -y
drush en -y quote

echo "Clean cache"
drush cc all
