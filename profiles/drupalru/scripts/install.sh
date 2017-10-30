#!/usr/bin/env bash

if [ -z $(drush --version | grep "7\|8\|9" ) ] ; then
  echo "Программа \"drush\" версий 7, 8 или 9 не установлена."
  exit 1;
fi

echo ""
echo "Получение переменных"
# Required fields
GH_NAME=""
DB_NAME=""
DB_PASS=""

# Get values
while [ -z "$GH_NAME" ] ; do
    read -p "Имя пользователя в GitHub: " GH_NAME
done
read -e -p "Имя вашего будущего локального сайта: " -i "dru.loc" SITE_NAME
read -e -p "Путь до дирректории, где будет создана папка с сайтом: " -i "/var/www" SITE_PARENT
while [ -z "$DB_NAME" ] ; do
    read -p "Имя базы данных: " DB_NAME
done
read -e -p "Имя пользователя базы данных: " -i "root" DB_USER
while [ -z "$DB_PASS" ] ; do
    read -sp "Пароль базы данных: " DB_PASS
done
echo ""
read -e -p "Drush алиас: " -i "dru" DRUSH_ALIAS

REPO="git@github.com:$GH_NAME/drupal.ru.git"
SITE="$SITE_PARENT/$SITE_NAME"

#---- Processing ----#

echo ""
echo "Загрузка репозитрия из \"$REPO\" в \"$SITE\""
git clone "$REPO" "$SITE"
if [ ! -d "$SITE" ] ; then
  echo "Репозиторий не склонирован"
  exit 1;
fi
git -c core.quotepath=false checkout -b dev "$GH_NAME/dev" --

echo ""
echo "Создание алиасов"
if [ ! -d "$HOME/.drush" ] ; then
  mkdir "$HOME/.drush"
  echo "Папка \"$HOME/.drush\" создана"
fi

if [ ! -f "$HOME/.drush/$DRUSH_ALIAS.aliases.drushrc.php" ]; then
  printf "<?php
\$aliases['loc'] = array(
  'root' => '$SITE',
  'uri' => '$SITE_NAME',
);
" >> "$HOME/.drush/$DRUSH_ALIAS.aliases.drushrc.php"
  echo "Файл алиасов \"$HOME/.drush/$DRUSH_ALIAS.aliases.drushrc.php\" создан"
else
    echo "Вы уже имеете локальный файл с алиасом \"$DRUSH_ALIAS\""
    exit 1;
fi

echo ""
echo "Настройка сайта"
mkdir "$SITE/sites/local"
echo "Папка с конфигурацией сайта \"$SITE/sites/local\" создана"

printf "<?php
\$databases = array(
  'default' => array(
    'default' => array(
      'database' => '$DB_NAME',
      'username' => '$DB_USER',
      'password' => '$DB_PASS',
      'host'     => 'localhost',
      'port'     => '',
      'driver'   => 'mysql',
      'prefix'   => '',
    ),
  ),
);
\$conf['temp_path'] = sys_get_temp_dir();
" >> "$SITE/sites/local/settings.php"
echo "Конфигурационный файл \"$SITE/sites/local/settings.php\" создан"

printf "<?php
\$sites['$SITE_NAME'] = 'local';
" >> "$SITE/sites/local.sites.php"
echo "Конфигурационный файл настроен на адрес сайта \"$SITE_NAME\""

