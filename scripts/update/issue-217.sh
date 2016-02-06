#!/bin/sh
# Issue 217. Front page.

# Update slogan.
drush vset site_slogan 'Сообщество разработчиков и пользователей Drupal в рунете'

# Update nodes limit on frontpage
drush vset site_frontpage 'frontpage'


echo "Activate module: dru_frontpage"

ln -s $GITLC_DEPLOY_DIR/modules/dru_frontpage $SITEPATH/sites/all/modules/local/
drush -y en dru_frontpage

echo "Import settings"
drush ddi variables --file=$GITLC_DEPLOY_DIR/data/dru_frontpage.variables.export

echo "Set organisations limits"
drush vset organizations_block_count '3'

echo "Set event limits"
drush vset upcoming_events_block_count '5'

# update block settings
drush ddi blocks --file=$GITLC_DEPLOY_DIR/data/alpha.blocks.export