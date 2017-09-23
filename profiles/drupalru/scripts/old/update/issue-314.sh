#!/bin/sh

echo "Activate module: dru_ticket & dru_claim"

ln -s $GITLC_DEPLOY_DIR/modules/dru_tickets $SITEPATH/sites/all/modules/local/

drush  en dru_tickets -y
drush  en dru_claim -y

#import translation
drush -y language-import ru sites/all/modules/local/dru_tickets/translations/dru_tickets.ru.po
drush -y language-import ru sites/all/modules/local/dru_tickets/dru_claim/translations/dru_claim.ru.po
