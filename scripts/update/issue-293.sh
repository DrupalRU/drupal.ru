#!/bin/sh

echo "Activate module: dru_tnx"

ln -s $GITLC_DEPLOY_DIR/modules/dru_tnx $SITEPATH/sites/all/modules/local/

drush  en dru_tnx -y

#import translation

echo "Import Translation for dru_tnx"

drush -y language-import ru sites/all/modules/local/dru_tnx/translations/dru_tnx.ru.po
