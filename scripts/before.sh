#!/bin/sh

SITEPATH="$HOME/domains/$DOMAIN"
cd $SITEPATH


drush cache-clear drush
