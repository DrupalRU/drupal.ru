#!/bin/sh

set -e
CI_COLOR='\033[1;32m'
NO_COLOR='\033[0m'
sm() {
  echo "";
  echo "${CI_COLOR}$@${NO_COLOR}";
}

exe() {
  sm "$@";
  $@;
}

COMMIT=$(git log -n1 --abbrev-commit|grep commit|awk '{print $2}')
TIMESTAMP=$(date +%Y.%m.%d_%H:%M:%S)
ENVIRONMENT=${ENVIRONMENT:-dev}
SITE_NAME=${SITE:-$ENVIRONMENT.drupal.ru}
SITE_DIR="$HOME/domains/$SITE_NAME"
SCRIPTS_DIR="$SITE_DIR/profiles/drupalru/scripts"
ENVIRONMENT_DIR="$HOME/envs/$ENVIRONMENT"
VERSION="$TIMESTAMP.$COMMIT"
VERSIONS_DIR="$ENVIRONMENT_DIR/versions"
VERSION_DIR="$VERSIONS_DIR/$VERSION"
PREVIOUS_VERSION=$(ls | tail -n 1)

sm "Статус репозитрия"
git status

sm "Создание новой версии $VERSION"
if [ ! -d "$VERSION_DIR" ]; then
  mkdir -p "$VERSION_DIR"
  echo "Создание версии $VERSION выполнено"
fi

sm "Деплоймент новой $ENVIRONMENT версии $VERSION"
rsync -am --stats --inplace "$ZENCI_DEPLOY_DIR/" "$VERSION_DIR" --exclude=".git"
sm "Новый коммит \"$COMMIT\" задеплоен"
rm -rf "$VERSION_DIR/sites/default/files"
sm "Папка default/files очищена"
rsync -am --stats "$ENVIRONMENT_DIR/etalon/" "$VERSION_DIR"
sm "Эталон скопирован"
ln -sfn "$VERSION_DIR" "$SITE_DIR"
sm "Ссылка на новую версию \"$VERSION\" создана"

sm "Запуск обновлений"
drush @dru.${ENVIRONMENT} updb -y

sm "Очистка кэша"
drush @dru.${ENVIRONMENT} cc all

# Building of dumps.
sm "Scripts dir: $SCRIPTS_DIR"
chmod +x "$SCRIPTS_DIR/sync/build-dumps.sh"
sh "$SCRIPTS_DIR/sync/build-dumps.sh"
chmod -x "$SCRIPTS_DIR/sync/build-dumps.sh"

cd $VERSIONS_DIR
if [ $(ls -l | grep -c ^d) -gt $STORE_VERSIONS ] ; then
    sm "Удаление устаревших версий"
    while [ $(ls -l | grep -c ^d) -gt $STORE_VERSIONS ]
    do
        DEPRECATED_VERSION=$(ls -r | tail -n 1)
        chmod 755 -R ./$DEPRECATED_VERSION
        rm -rf ./$DEPRECATED_VERSION
        sm "Версия \"$DEPRECATED_VERSION\" удалена"
    done
else
    sm "Устаревших версий не найдено"
fi

sm "Деплоймент завершён"