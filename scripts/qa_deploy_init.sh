#!/bin/sh

#prepare database access
export DATABASE_NAME=`echo $DATABASE_NAME|md5sum|awk '{print$1}'`
export DATABASE_USER=`echo $DATABASE_NAME|md5sum|awk '{print$1}'|cut -c 1-10`
export DATABASE_PASS=`echo $DATABASE_NAME|md5sum|awk '{print$1}'`
export ACCOUNT_PASS=`echo $DATABASE_USER`

mysqladmin -uroot create $DATABASE_NAME
mysql -u root mysql -e "CREATE USER '"$DATABASE_USER"'@'localhost';"
mysql -u root mysql -e "GRANT ALL ON $DATABASE_NAME.* TO '"$DATABASE_USER"'@'localhost' IDENTIFIED BY '"$DATABASE_PASS"';"


#prepare DOCROOT
mkdir $DOCROOT

#prepare apache config and restart it.
cat $HOME/conf.d/template|sed 's/{$DOMAIN}/'$DOMAIN'/g' > $HOME/conf.d/$DOMAIN.conf
sudo apachectl restart

#install drupal
sh $ZENCI_DEPLOY_DIR/scripts/init.sh

echo "Website $DOMAIN installed: $ACCOUNT_NAME: $ACCOUNT_PASS"
