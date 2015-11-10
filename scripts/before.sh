#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"
cd $SITEPATH

#Quote module replacement. Issue #51

if [ -d "$SITEPATH/sites/all/modules/contrib/quote" ]; then
  drush -y dis quote
  rm -rf $SITEPATH/sites/all/modules/contrib/quote
  touch /tmp/quote.remove
fi


drush cache-clear drush
