#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Fix view node revisions - issue #58
drush up diff-7.x-3.x-dev -y

echo "Clean cache"
drush cc all
