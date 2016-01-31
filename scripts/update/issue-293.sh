#!/bin/sh

echo "Activate module: dru_tnx"

ln -s $GITLC_DEPLOY_DIR/modules/dru_tnx $SITEPATH/sites/all/modules/local/

drush  en dru_tnx -y
