#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"

# update #319. Disable Enable is helping to enable all extra xbbcodes to be enabled. Simpler way.
cd $SITEPATH
drush -y dis xbbcode_dru
drush -y en xbbcode_dru

echo "Clean cache"
drush cc all
