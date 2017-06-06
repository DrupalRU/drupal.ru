#!/bin/sh

STATUSFILE="$DOCROOT/.deploy.status"


UPDATEDIR="$ZENCI_DEPLOY_DIR/scripts/update/"

touch $STATUSFILE

#update drupal_deploy
cd $DOCROOT/sites/all/modules/github/drupal_deploy
git pull

cd $DOCROOT


# enable drupal deploy
drush -y en drupal_deploy

for file in `ls $UPDATEDIR|grep sh$|grep -vf $STATUSFILE`;do
  echo "Processing $file"
  sh $UPDATEDIR/$file
  echo "$file" >> $STATUSFILE
done

# disable drupal deploy
drush -y dis drupal_deploy
