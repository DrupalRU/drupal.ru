#!/bin/sh

SITEPATH="$GITLC_DOCROOT"

echo "Full site path: $SITEPATH"
cd $SITEPATH

echo "Migrating Menu structure"
mysql -u$SETTINGS_DATABASE_USER -p$SETTINGS_DATABASE_PASS $SETTINGS_DATABASE_NAME < $GITLC_DEPLOY_DIR/db/primary-links.sql

mysql -u$SETTINGS_DATABASE_USER -p$SETTINGS_DATABASE_PASS $SETTINGS_DATABASE_NAME < $GITLC_DEPLOY_DIR/db/secondary-links.sql


echo "Clean cache"
drush cc all
