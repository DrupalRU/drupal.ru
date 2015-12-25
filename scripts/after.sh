#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#update 25 dec 2015
ln -s $GITLC_DEPLOY_DIR/modules/validate_api $SITEPATH/sites/all/modules/local/

# import translation
drush -y language-import ru $GITLC_DEPLOY_DIR/modules/user_filter/user_filter_notify/translations/user_filter_notify.ru.po
drush -y language-import ru $GITLC_DEPLOY_DIR/modules/validate_api/translations/validate_api.ru.po
drush -y language-import ru $GITLC_DEPLOY_DIR/modules/validate_api/antiswearing_validate/translations/antiswearing_validate.ru.po
drush -y language-import ru $GITLC_DEPLOY_DIR/modules/validate_api/antinoob_validate/translations/antinoob_validate.ru.po
drush -y language-import ru $GITLC_DEPLOY_DIR/modules/resolve/translations/resolve.ru.po

#enable module
drush -y en user_filter user_filter_notify validate_api antinoob_validate antiswearing_validate darkmatter resolve

#import darkmatter settings
drush en -y drupal_deploy
drush ddi variables --file=$GITLC_DEPLOY_DIR/data/darkmatter_notify.variables.export
drush ddi variables --file=$GITLC_DEPLOY_DIR/data/user_info_notify.variables.export
drush ddi variables --file=$GITLC_DEPLOY_DIR/data/validate_api.variables.export
drush ddi variables --file=$GITLC_DEPLOY_DIR/data/resolve_can.variables.export
drush ddi filters  --file=$GITLC_DEPLOY_DIR/data/filters.export


drush dis -y drupal_deploy


echo "Clean cache"
drush cc all
