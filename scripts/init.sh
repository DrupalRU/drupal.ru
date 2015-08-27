#!/bin/sh
echo "INIT DEVEL Drupal.ru Version"

CORE='drupal-6'
SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"
CONTRIB="acl ascii_art_captcha bbcode bueditor cacherouter captcha captcha_pack comment_notify comment_upload diff fasttoggle flag flag_abuse geshifilter google_plusone gravatar imageapi imagecache imagecache_profiles live_translation noindex_external_links pathauto pearwiki_filter privatemsg quote simplenews smtp spambot tagadelic taxonomy_manager token transliteration  views xmlsitemap "

echo "Full site path: $SITEPATH"
echo "Site core: $CORE"
echo "Deploy DIR: $GITLC_DEPLOY_DIR"

cd $SITEPATH
echo "Download DRUPAL."

drush dl $CORE --drupal-project-rename="drupal"

rsync -a $SITEPATH/drupal/ $SITEPATH
rm -rf tmp

echo "Install DRUPAL"

/usr/bin/drush site-install default -y --root=$SITEPATH --account-name=$SETTINGS_ACCOUNT_NAME --account-mail=$SETTINGS_ACCOUNT_MAIL --account-pass=$SETTINGS_ACCOUNT_PASS --uri=http://$SETTINGS_DOMAIN --site-name="$SETTINGS_SITE_NAME" --site-mail=$SETTINGS_SITE_MAIL --db-url=mysql://$SETTINGS_DATABASE_USER:$SETTINGS_DATABASE_PASS@localhost/$SETTINGS_DATABASE_NAME

echo "Install contrib modules"

mkdir -p $SITEPATH/sites/all/modules/contrib
drush dl $CONTRIB
drush -y en $CONTRIB

echo "Install drupal.ru modules"
mkdir $SITEPATH/sites/all/modules/local

ln -s $GITLC_DEPLOY_DIR/modules/* $SITEPATH/sites/all/modules/local/

echo "Install drupal.ru themes"
mkdir $SITEPATH/sites/all/themes/local

ln -s $GITLC_DEPLOY_DIR/themes/* $SITEPATH/sites/all/themes/local/


# Find all info files
modules_enable="";
for file in $(find $GITLC_DEPLOY_DIR -name \*.info -print)
do
  filename=$(basename "$file")
  modules_enable+="${filename%.*},"
done

echo "Enable modules and themes: $modules_enable"
drush -y en $modules_enable

echo "Set drupal.ru theme as default"
drush vset theme_default novosibirsk

if [-n "SETTINGS_DEVEL" ]; then
  drush dl devel
  drush en devel -y
  drush en devel_generate -y
  drush generate-content 200
  drush generate-users 100
fi  

echo "Please check http://$SETTINGS_DOMAIN"