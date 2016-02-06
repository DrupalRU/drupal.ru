#!/bin/sh

echo "Import taxonomy for ticket status"
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/ticket_status.taxonomy.export

echo "Import taxonomy for claim status"
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/claim_category.taxonomy.export

echo "Update block settings"
drush ddi blocks --file=$GITLC_DEPLOY_DIR/data/alpha.blocks.export

echo "Delete old alias"
drush sqlq "DELETE from url_alias where alias='events';"

echo "Set organisations limits"
drush vset organizations_block_count '3'

echo "Update menu structure"
drush ddi menu --file=$GITLC_DEPLOY_DIR/data/main-menu.menu_links.export
drush ddi menu --file=$GITLC_DEPLOY_DIR/data/user-menu.menu_links.export

echo "Import nodetypes"
drush ddi node_types --file=$GITLC_DEPLOY_DIR/data/organization.node_types.export
drush ddi node_types --file=$GITLC_DEPLOY_DIR/data/simple_event.node_types.export
drush ddi node_types --file=$GITLC_DEPLOY_DIR/data/ticket.node_types.export
drush ddi node_types --file=$GITLC_DEPLOY_DIR/data/blog.node_types.export
