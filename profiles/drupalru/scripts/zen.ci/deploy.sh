#!/bin/sh

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

print_status() {
  echo "";
  echo "\033[0;32m$@\033[0m"
}

print_status "Статус репозитрия"
git status

print_status "Создание новой версии $VERSION"
if [ ! -d "$VERSION_DIR" ]; then
  mkdir -p "$VERSION_DIR"
  echo "Создание версии $VERSION выполнено"
fi

print_status "Деплоймент новой $ENVIRONMENT версии $VERSION"
rsync -am --stats --inplace "$ZENCI_DEPLOY_DIR/" "$VERSION_DIR" --exclude=".git"
print_status "Новый коммит \"$COMMIT\" задеплоен"
rm -rf "$VERSION_DIR/sites/default/files"
print_status "Папка default/files очищена"
rsync -am --stats "$ENVIRONMENT_DIR/etalon/" "$VERSION_DIR"
print_status "Эталон скопирован"
ln -sfn "$VERSION_DIR" "$SITE_DIR"
print_status "Ссылка на новую версию \"$VERSION\" создана"

print_status "Запуск обновлений"
drush @dru.${ENVIRONMENT} updb -y

print_status "Очистка кэша"
drush @dru.${ENVIRONMENT} cc all

cd $VERSIONS_DIR
if [ $(ls -l | grep -c ^d) -gt $STORE_VERSIONS ] ; then
    print_status "Удаление устаревших версий"
    while [ $(ls -l | grep -c ^d) -gt $STORE_VERSIONS ]
    do
        DEPRECATED_VERSION=$(ls -r | tail -n 1)
        chmod 755 -R ./$DEPRECATED_VERSION
        rm -rf ./$DEPRECATED_VERSION
        print_status "Версия \"$DEPRECATED_VERSION\" удалена"
    done
else
    print_status "Устаревших версий не найдено"
fi;
print_status "Деплоймент завершён"