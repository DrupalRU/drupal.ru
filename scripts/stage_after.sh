#!/bin/sh
export DATABASE_PASS=`cat /home/stage/.my.cnf |grep pass|awk -F= '{print$2}'`

# revert database
mysql -u $DATABASE_USER $DATABASE_PASS $DATABASE_NAME < $HOME/dump/drupal_main.sql
mysql -u $DATABASE_USER $DATABASE_PASS $DATABASE_NAME < $HOME/dump/drupal_main.sphinxmain.sql


echo "Process after.sh"
sh $ZENCI_DEPLOY_DIR/scripts/after.sh
