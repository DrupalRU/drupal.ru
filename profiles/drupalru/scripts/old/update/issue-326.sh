#!/bin/sh

echo "Activate module: dru_solve_mark"

ln -s $GITLC_DEPLOY_DIR/modules/dru_solve_mark $SITEPATH/sites/all/modules/local/

drush  en dru_solve_mark -y
