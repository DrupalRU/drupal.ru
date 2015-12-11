#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Issue #188 enable HTML-mail modules

echo "Enable module: smtp"
drush  en smtp -y

echo "Enable module: htmlmail"
drush  en htmlmail -y

echo "Enable module: mimemail"
drush  en mimemail -y

echo "Clean cache"
drush cc all
