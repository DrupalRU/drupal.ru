#!/bin/sh
echo "INIT Drupal.ru"

CORE='drupal-7'
SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"
CONTRIB="acl bbcode bueditor captcha  comment_notify diff fasttoggle geshifilter google_plusone gravatar imageapi noindex_external_links pathauto privatemsg quote simplenews smtp spambot tagadelic taxonomy_manager jquery_ui jquery_update token rrssb ajax_comments fontawesome transliteration libraries views xmlsitemap bootstrap_lite xbbcode ban_user"

echo "Full site path: $SITEPATH"
echo "Site core: $CORE"
echo "Deploy DIR: $GITLC_DEPLOY_DIR"

cd $SITEPATH
echo "Download DRUPAL."

drush dl $CORE --drupal-project-rename="drupal"

rsync -a $SITEPATH/drupal/ $SITEPATH
rm -rf drupal

echo "Install DRUPAL"

/usr/bin/drush site-install standard -y --root=$SITEPATH --account-name=$SETTINGS_ACCOUNT_NAME --account-mail=$SETTINGS_ACCOUNT_MAIL --account-pass=$SETTINGS_ACCOUNT_PASS --uri=http://$SETTINGS_DOMAIN --site-name="$SETTINGS_SITE_NAME" --site-mail=$SETTINGS_SITE_MAIL --db-url=mysql://$SETTINGS_DATABASE_USER:$SETTINGS_DATABASE_PASS@localhost/$SETTINGS_DATABASE_NAME

echo "make libraries dir"
mkdir $SITEPATH/sites/all/libraries

echo "Install contrib modules"

mkdir -p $SITEPATH/sites/all/modules/contrib
drush dl $CONTRIB
drush -y en $CONTRIB

echo "Install captcha_pack"
drush dl captcha_pack
drush -y en ascii_art_captcha css_captcha

echo "Install other modules"
drush -y en imageapi_imagemagick pm_block_user pm_email_notify privatemsg_filter  views_ui book forum

echo "Prepare github modules dir"
mkdir -p $SITEPATH/sites/all/modules/github

echo "Install inner poll"

cd $SITEPATH/sites/all/modules/github
git clone --branch master http://git.drupal.org/sandbox/andypost/1413472.git inner_poll
cd  inner_poll
git checkout 7.x-1.x

echo "Deploy module"

cd  $SITEPATH/sites/all/modules/github
git clone https://github.com/itpatrol/drupal_deploy.git
cd drupal_deploy
git checkout 7.x

echo "Altpager"
cd  $SITEPATH/sites/all/modules/github
git clone https://github.com/itpatrol/altpager

echo "Alttracker"
cd  $SITEPATH/sites/all/modules/github
git clone https://github.com/itpatrol/alttracker

cd $SITEPATH
drush -y en inner_poll altpager alttracker drupal_deploy

echo "Install drupal.ru modules"
mkdir -p $SITEPATH/sites/all/modules/local

ln -s $GITLC_DEPLOY_DIR/modules/* $SITEPATH/sites/all/modules/local/


echo "Install Font awesome"
cd  $SITEPATH/sites/all/modules/libraries
git clone https://github.com/FortAwesome/Font-Awesome.git fontawesome


echo "Install drupal.ru themes"
mkdir -p $SITEPATH/sites/all/themes/local

ln -s $GITLC_DEPLOY_DIR/themes/* $SITEPATH/sites/all/themes/local/

echo "Set default theme"
cd $SITEPATH

echo "Set default variables"
drush vset theme_default alpha
drush vset filestore_tmp_dir /tmp
drush vset admin_theme alpha

echo "Please check http://$SETTINGS_DOMAIN"
