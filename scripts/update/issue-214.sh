#!/bin/sh
# Issue 214. Marketplace module.

# import vocabulary organizations
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/organizations.taxonomy.export

# enable module
drush -y en marketplace

# update settings for node type organization
drush ddi node_types --file=$GITLC_DEPLOY_DIR/data/organization.node_types.export

# update block settings
drush ddi blocks --file=$GITLC_DEPLOY_DIR/data/alpha.blocks.export
