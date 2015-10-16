#!/bin/sh
echo "INIT DEVEL Drupal.ru Version"

CORE='drupal-7'
SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"
CONTRIB="acl bbcode bueditor captcha  comment_notify diff fasttoggle flag flag_abuse geshifilter google_plusone gravatar imageapi live_translation noindex_external_links pathauto pearwiki_filter privatemsg quote simplenews smtp spambot tagadelic taxonomy_manager token transliteration  views xmlsitemap "

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

echo "Install contrib modules"

mkdir -p $SITEPATH/sites/all/modules/contrib
drush dl $CONTRIB
drush -y en $CONTRIB

echo "Install captcha_pack"
drush dl captcha_pack
drush -y en ascii_art_captcha css_captcha

echo "Install other modules"
drush -y en imageapi_imagemagick flag_actions geshinode pm_block_user pm_email_notify privatemsg_filter token_actions views_ui book forum geshinode php

echo "Install drupal.ru modules"
mkdir -p $SITEPATH/sites/all/modules/local

#ln -s $GITLC_DEPLOY_DIR/modules/* $SITEPATH/sites/all/modules/local/

echo "Install drupal.ru themes"
mkdir -p $SITEPATH/sites/all/themes/local

#ln -s $GITLC_DEPLOY_DIR/themes/* $SITEPATH/sites/all/themes/local/

echo "Please check http://$SETTINGS_DOMAIN"
