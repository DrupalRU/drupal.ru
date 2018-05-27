#!/bin/sh

set -e
trap 'echo "exit due to $(!!)"' EXIT

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
SCRIPTS_DIR="$ZENCI_DEPLOY_DIR/profiles/drupalru/scripts/zen.ci"
ENVIRONMENT_DIR="$HOME/envs/$ENVIRONMENT"
VERSION="$TIMESTAMP.$COMMIT"
VERSIONS_DIR="$ENVIRONMENT_DIR/versions"
VERSION_DIR="$VERSIONS_DIR/$VERSION"
PREVIOUS_VERSION=$(ls | tail -n 1)

exe "Статус репозитрия"
git status

exe "Создание новой версии $VERSION"
if [ ! -d "$VERSION_DIR" ]; then
  mkdir -p "$VERSION_DIR"
  echo "Создание версии $VERSION выполнено"
fi

exe "Деплоймент новой $ENVIRONMENT версии $VERSION"
rsync -am --stats --inplace "$ZENCI_DEPLOY_DIR/" "$VERSION_DIR" --exclude=".git"
exe "Новый коммит \"$COMMIT\" задеплоен"
rm -rf "$VERSION_DIR/sites/default/files"
exe "Папка default/files очищена"
rsync -am --stats "$ENVIRONMENT_DIR/etalon/" "$VERSION_DIR"
exe "Эталон скопирован"
ln -sfn "$VERSION_DIR" "$SITE_DIR"
exe "Ссылка на новую версию \"$VERSION\" создана"

exe "Запуск обновлений"
drush @dru.${ENVIRONMENT} updb -y

exe "Очистка кэша"
drush @dru.${ENVIRONMENT} cc all

cd $VERSIONS_DIR
if [ $(ls -l | grep -c ^d) -gt $STORE_VERSIONS ] ; then
    exe "Удаление устаревших версий"
    while [ $(ls -l | grep -c ^d) -gt $STORE_VERSIONS ]
    do
        DEPRECATED_VERSION=$(ls -r | tail -n 1)
        chmod 755 -R ./$DEPRECATED_VERSION
        rm -rf ./$DEPRECATED_VERSION
        exe "Версия \"$DEPRECATED_VERSION\" удалена"
    done
else
    exe "Устаревших версий не найдено"
fi;
exe "Деплоймент завершён"