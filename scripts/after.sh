#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Update alttracker now

cd $SITEPATH/sites/all/modules/github/alttracker/
git pull

#Issue #21 install live update
drush dl l10n_update
drush -y en l10n_update
drush -y l10n-update-refresh
drush -y l10n-update

#set auto update weekly
drush vset l10n_update_check_frequency 7




echo "Clean cache"
drush cc all
