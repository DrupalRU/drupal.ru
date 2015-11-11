#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"
cd $SITEPATH

drush dis quote -y

rm -f $SITEPATH/sites/all/modules/local/quote

drush cache-clear drush
