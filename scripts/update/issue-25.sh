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
mkdir ~/sphinx/index
mkdir ~/sphinx/log

cp  $GITLC_DEPLOY_DIR/data/sphinx.conf ~/sphinx/


DATABASE_PASS=`drush st --show-passwords|grep password|awk '{print$4}'`

sed -i "s|HOMEDIR|$HOME|g" ~/sphinx/sphinx.conf
sed -i "s|SQLUSER|$SETTINGS_DATABASE_USER|g" ~/sphinx/sphinx.conf
sed -i "s|SQLPASS|$DATABASE_PASS|g" ~/sphinx/sphinx.conf
sed -i "s|SQLDB|$SETTINGS_DATABASE_NAME|g" ~/sphinx/sphinx.conf

#index it:
/usr/bin/sphinx-indexer --config $HOME/sphinx/sphinx.conf --all

#start daemon:
/usr/sbin/sphinx-searchd --config $HOME/sphinx/sphinx.conf 

#import roles
drush ddi roles --file=$GITLC_DEPLOY_DIR/data/roles.export

echo "Update block settings"
drush ddi blocks --file=$GITLC_DEPLOY_DIR/data/alpha.blocks.export
