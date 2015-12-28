#!/bin/sh

SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"

echo "Full site path: $SITEPATH"
cd $SITEPATH

#mini update #266 #267
drush -y language-import ru $GITLC_DEPLOY_DIR/modules/validate_api/antinoob_validate/translations/antinoob_validate.ru.po

echo "Clean cache"
drush cc all
