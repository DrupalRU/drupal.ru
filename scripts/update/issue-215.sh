#!/bin/sh
# Issue 215. Simple events module.

# Link module
ln -s $GITLC_DEPLOY_DIR/modules/simple_events $SITEPATH/sites/all/modules/local/

# import vocabulary event_type
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/event_types.taxonomy.export

# drush dl
drush -y dl date
drush -y en date_popup date_api

# enable module
drush -y en simple_events

# update settings for node type simple_event
drush ddi node_types --file=$GITLC_DEPLOY_DIR/data/simple_event.node_types.export

#import translation
drush -y language-import ru sites/all/modules/local/simple_events/translations/simple_events.ru.po
