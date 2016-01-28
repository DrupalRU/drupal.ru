#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"
STATUSFILE="$SITEPATH/.deploy.status"

UPDATEDIR="$GITLC_DEPLOY_DIR/scripts/update/"

touch $STATUSFILE

#update drupal_deploy
cd $SITEPATH/sites/all/modules/github/drupal_deploy
git pull

cd $SITEPATH

# enable drupal deploy
drush -y en drupal_deploy

for file in `ls $UPDATEDIR|grep sh$|grep -vf $STATUSFILE`;do
  echo "Processing $file"
  sh $file
  echo "$file" >> $STATUSFILE
done

# disable drupal deploy
drush -y dis drupal_deploy
