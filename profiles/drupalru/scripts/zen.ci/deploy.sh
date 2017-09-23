#!/bin/sh

COMMIT=$(git log -n1 --abbrev-commit|grep commit|awk '{print $2}')
TIMESTAMP=$(date +%Y.%m.%d_%H:%M:%S)
SCRIPTS_PATH="$ZENCI_DEPLOY_DIR/scripts/zen.ci"
ENVIRONMENT=${ENVIRONMENT:-dev}
VERSIONS_PATH="$HOME/github/environments/$ENVIRONMENT"
VERSION_PATH="$VERSIONS_PATH/$TIMESTAMP.$COMMIT"
SITE_NAME=${SITE:-$ENVIRONMENT.drupal.ru}
SITE_PATH="$HOME/domains/$SITE_NAME"
PROFILES_PATH="$SITE_PATH/profiles"
header() {
  echo "";
  echo "\033[0;32m$@\033[0m"
}
header "Стягивание кода"
echo "Статус репозитрия"
echo ""
git status
git add .
git checkout -f HEAD
git pull
git status
echo "Статус репозитрия после стягивания"
echo ""

header "Создание новой версии $TIMESTAMP.$COMMIT"
if [ ! -d "$VERSION_PATH" ]; then
  mkdir -p "$VERSION_PATH"
  echo "Создание выполнено"
fi

header "Деплоймент новой $ENVIRONMENT версии $TIMESTAMP.$COMMIT"
rsync -am --stats "$ZENCI_DEPLOY_DIR/" "$VERSION_PATH" --exclude=".git"
cd "$PROFILES_PATH"
if [ -h "./$PROFILE" ]; then
  rm "./$PROFILE"
fi
ln -s "$VERSION_PATH" "$PROFILE"
echo "Деплоймент выполнен"

header "Запуск обновлений"
drush updb -y

header "Очистка кэша"
drush cc all

header "Удаление устаревших версий"
cd $VERSIONS_PATH
sh $SCRIPTS_PATH/clean.sh