#!/bin/sh

echo "Activate module: dru_ticket & dru_claim"

ln -s $GITLC_DEPLOY_DIR/modules/dru_tickets $SITEPATH/sites/all/modules/local/

drush  en dru_tickets -y
drush  en dru_claim -y
