#!/bin/sh

echo "Update translations for modules: dru_ticket & dru_claim"

#import translation
drush -y language-import ru sites/all/modules/local/dru_tickets/translations/dru_tickets.ru.po
drush -y language-import ru sites/all/modules/local/dru_tickets/dru_claim/translations/dru_claim.ru.po
