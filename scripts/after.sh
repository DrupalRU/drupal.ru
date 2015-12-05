#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Issue #148 enable compression
drush vset preprocess_css 1
drush vset preprocess_js 1

#clean old directories
rm -rf $SITEPATH/files/js
rm -rf $SITEPATH/files/languages



echo "Clean cache"
drush cc all
