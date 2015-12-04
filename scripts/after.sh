#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Issue #44 update logo settings
drush en -y drupal_deploy

drush ddi variables --file=$GITLC_DEPLOY_DIR/data/theme_alpha_settings.variables.export

drush -y pm-disable drupal_deploy


echo "Clean cache"
drush cc all
