#!/bin/sh

COMMIT=$(git log -n1 --abbrev-commit|grep commit|awk '{print $2}')
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
SCRIPTS="$ZENCI_DEPLOY_DIR/scripts"
VERSION_DIR="$HOME/github/environments/$ENVIRONMENT/$TIMESTAMP.$COMMIT"
SITEPATH="$HOME/domains/$ENVIRONMENT.drupal.ru"
PROFILES_PATH="$SITEPATH/profiles"
prompt()
{
    echo "";
    echo "\033[0;32m$@\033[0m"
}
prompt "Creating of version $TIMESTAMP.$COMMIT"
if [ ! -d "$VERSION_DIR" ]; then
  mkdir -p "$VERSION_DIR"
fi

prompt "Coping code"
rsync -am --stats "$ZENCI_DEPLOY_DIR/" "$VERSION_DIR" --exclude=".git"

cd "$PROFILES_PATH"
if [ -h "$PROFILE" ]; then
  echo "Deleting link to previous profile"
  rm "$PROFILE"
fi
prompt "Deploying new profile"
ln -s "$VERSION_DIR" "$PROFILE"

prompt "Updating database"
drush updb -y

prompt "Cleaning cache"
drush cc all

