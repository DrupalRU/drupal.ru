#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Issue #167 replace favicon

echo "Import theme settings"
drush -y en drupal_deploy

drush ddi variables --file=$GITLC_DEPLOY_DIR/data/theme_alpha_settings.variables.export

drush -y dis drupal_deploy



echo "Clean cache"
drush cc all
