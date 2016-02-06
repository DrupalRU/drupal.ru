#!/bin/sh
# Issue 25. Advanced sphinxsearch.

# Link module
ln -s $GITLC_DEPLOY_DIR/modules/advanced_sphinx $SITEPATH/sites/all/modules/local/

# enable module
drush -y en advanced_sphinx

#import translation
drush -y language-import ru sites/all/modules/local/advanced_sphinx/translations/ru.po

#import variables
drush ddi variables --file=$GITLC_DEPLOY_DIR/data/advanced_sphinx.variables.export

#import sphinx settings
mkdir ~/sphinx
cp  $GITLC_DEPLOY_DIR/data/sphinx.conf ~/sphinx/

sed -i "s|HOMEDIR|$HOME|g" ~/sphinx/sphinx.conf
sed -i "s|SQLUSER|$SETTINGS_DATABASE_USER|g" ~/sphinx/sphinx.conf
sed -i "s|SQLPASS|SETTINGS_DATABASE_PASS|g" ~/sphinx/sphinx.conf
sed -i "s|SQLDB|$SETTINGS_DATABASE_NAME|g" ~/sphinx/sphinx.conf

#index it:
/usr/bin/sphinx-indexer --config ~/sphinx/sphinx.conf --all

#start daemon:
/usr/sbin/sphinx-searchd --config ~/sphinx/sphinx.conf 

