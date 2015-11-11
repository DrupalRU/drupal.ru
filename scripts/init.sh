#!/bin/sh
echo "INIT Drupal.ru"

CORE='drupal-7'
SITEPATH="$HOME/domains/$SETTINGS_DOMAIN"
CONTRIB="acl bbcode bueditor captcha  comment_notify diff-7.x-3.x-dev fasttoggle geshifilter google_plusone gravatar imageapi noindex_external_links pathauto privatemsg simplenews smtp spambot tagadelic taxonomy_manager jquery_ui jquery_update token rrssb ajax_comments fontawesome transliteration libraries views xmlsitemap bootstrap_lite xbbcode ban_user"

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

echo "Enable quote"
drush en -y quote

echo "Install Font awesome"
cd  $SITEPATH/sites/all/libraries
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

echo "Import META structure via module http://github.com/itpatrol/drupal_deploy."

echo "Import roles"
drush ddi roles --file=$GITLC_DEPLOY_DIR/data/roles.export

echo "Import filters"
drush ddi filters --file=$GITLC_DEPLOY_DIR/data/filters.export

echo "Import nodetypes"
drush ddi node_types --file=$GITLC_DEPLOY_DIR/data/blog.node_types.export

echo "Import taxonomy"
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/vocabulary_1.taxonomy.export
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/vocabulary_2.taxonomy.export
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/vocabulary_3.taxonomy.export
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/vocabulary_4.taxonomy.export
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/vocabulary_5.taxonomy.export
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/vocabulary_7.taxonomy.export
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/vocabulary_8.taxonomy.export
drush ddi taxonomy --file=$GITLC_DEPLOY_DIR/data/vocabulary_10.taxonomy.export

echo "Import forum"
drush ddi forum --file=$GITLC_DEPLOY_DIR/data/forum.export

echo "Import menu structure"
drush ddi menu --file=$GITLC_DEPLOY_DIR/data/main-menu.menu_links.export
drush ddi menu --file=$GITLC_DEPLOY_DIR/data/user-menu.menu_links.export

echo "Import theme settings"

drush ddi variables --file=$GITLC_DEPLOY_DIR/data/theme_bootstrap_lite_settings.variables.export

echo "Set default tmp"
drush vset filestore_tmp_dir /tmp

if [ "$SETTINGS_DEVEL" != "" ]; then
  cd $SITEPATH
  drudh dl devel
  drusn -y en devel
  drush generate-content 100
  drush generate-users 100
fi
echo "Please check http://$SETTINGS_DOMAIN"
