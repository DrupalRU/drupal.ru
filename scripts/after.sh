#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"
cd SITEPATH

echo "Full site path: $SITEPATH"
cd $SITEPATH

echo "Clean cache"
drush cc all
