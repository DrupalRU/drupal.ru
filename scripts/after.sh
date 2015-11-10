#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#Quote module replacement. Issue #51
if [ -f "/tmp/quote.remove" ]; then
  ln -s $GITLC_DEPLOY_DIR/modules/quote $SITEPATH/sites/all/modules/local/quote
  drush -y en quote
  rm -f /tmp/quote.remove
fi

echo "Clean cache"
drush cc all
