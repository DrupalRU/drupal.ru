#!/bin/sh
# Issue 214. Marketplace module.

# Link module
ln -s $GITLC_DEPLOY_DIR/modules/marketplace $SITEPATH/sites/all/modules/local/

# import vocabulary organizations
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/organizations.taxonomy.export

# enable module
drush -y en marketplace

# update settings for node type organization
drush ddi node_types --file=$GITLC_DEPLOY_DIR/data/organization.node_types.export


#import translation
drush -y language-import ru sites/all/modules/local/marketplace/translations/marketplace.ru.po
