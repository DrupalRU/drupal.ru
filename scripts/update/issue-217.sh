#!/bin/sh
# Issue 217. Front page.

# Update slogan.
drush vset site_slogan 'Сообщество разработчиков и пользователей Drupal в рунете'

# Update nodes limit on frontpage
drush vset site_frontpage 'frontpage'


echo "Activate module: dru_frontpage"

ln -s $GITLC_DEPLOY_DIR/modules/dru_frontpage $SITEPATH/sites/all/modules/local/

drush -y en dru_frontpage