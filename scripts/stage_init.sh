#!/bin/sh

#install drupal
sh $ZENCI_DEPLOY_DIR/scripts/init.sh

export DATABASE_PASS=`cat /home/stage/.my.cnf |grep pass|awk -F= '{print$2}'`

# revert database and files
mysql --user=$DATABASE_USER --password=$DATABASE_PASS $DATABASE_NAME < $HOME/dump/drupal_main.sql
mysql --user=$DATABASE_USER --password=$DATABASE_PASS $DATABASE_NAME < $HOME/dump/drupal_main.sphinxmain.sql

cd $DOCROOT
rm -rf files
ln -s ~/files ./files
