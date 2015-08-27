#!/bin/sh

SITEPATH="$GITLC_DOCROOT"

echo "Full site path: $SITEPATH"
cd $SITEPATH
echo "Clean cache"
drush cc all
