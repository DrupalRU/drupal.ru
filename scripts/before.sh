#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"
cd $SITEPATH


drush cache-clear drush
