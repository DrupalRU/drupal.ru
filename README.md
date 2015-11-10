
# Модули и темы для Drupal.ru
###Страница нуждается в описании.

## Содержимое
- [modules](https://github.com/DrupalRu/d6/tree/master/modules) - не contrib модули что используются на сайте
- [themes](https://github.com/DrupalRu/d6/tree/master/themes) - темы
- [media](https://github.com/DrupalRu/d6/tree/master/media) - картинки для сайта не используеммые в теме

## Репозиторий для drupal.ru
Используется для разработки и деплоя инсталяции drupal.ru

#Структура репозитория

- master. Продакшн. НЕ возможно удалить. 
  - Только обновляет код (git pull) и выполняет before и after скрипты. 
  - PR на этот бранч создают отдельные сайты без полной базы
- stage. Версия ПЕРЕД продакшином. 
  - Если удалить и создать бранч заново - необходимо будет заливать базу с master вручную.
  - Только обновляет код (git pull) и выполняет before и after скрипты. 
  - PR на этот бранч создают отдельные сайты без полной базы

##master
принятия любого PR или commit в этот репо автоматически отображается на работе сайта drupal.ru

##stage
принятия любого PR или commit в этот репо автоматически отображается на работе сайта stage.drupal.ru

##branch
любой бранч чтоб был deploy работал требует создания секции в .gitlc.yml по примеру stage
```
  stage:
    server: "karma.vps-private.net"
    user: "stage"
    robin:
      domain: "{branch}.dev.drupal.ru"
      server: "karma.vps-private.net"
      user: "dev"
      password: ""
    type:
      name: "custom"
      settings:
        domain: "{branch}.dev.drupal.ru"
        account-name: "root"
        account-mail: "gor.martsen@gmail.com"
        account-pass: "d6testing"
        site-name: "Drupal.ru dev site"
        site-mail: "noreply@{branch}.dev.drupal.ru"
        database-user: "dev_{branch}"
        database-name: "dev_{branch}"
        database-pass: "BHjrb454"
        devel: TRUE
    dir: '{home}/github/{branch}'
    init: '{deploy_dir}/scripts/init.sh'
    before: '{deploy_dir}/scripts/before.sh'
    after: '{deploy_dir}/scripts/after.sh'    
```
- dev, название бранча
  - robin, секция настройки робина
    - domain, имя домена который через robin-panel будет добавлен
    - server, имя сервера
    - user, имя SSH и Robin-Panel пользователя. Необходим для доступа по SSH и аплоада кода
    - password, пароль от Robin_panel и SSH (не подходит для публичного бранча)
  - type, секция настройки переменных для deploy скриптов
    - name, тип темплейта. доступные drupal6, drupal7, custom. Для drupal6, drupal7 вызываются свои скрипты деплоя. Custom это полсностью свои настройки
    - settings, переменные что будут переданы в скрипты в виде имени SETTINGS_NAME. Все "-", в названии переменных будут переделаны в "_".
  - dir, путь куда делать git clone 
  - init,  скрипт что вызывается в первый раз, для иницилизации системы. Определяется по отсутствию данных в папке dir
  - before, скрипт вызывается каждый раз до git pull когда появляются новые данные в репозитории. Можно использовать для перевода сайта в режим тех-обслуживания через drush
  - after, скрипт вызывается каждый раз после git pull когда появляются новые данные в репозитории. Можно использовать для перевода сайта из режима тех-обслуживания через drush
   
   
   
#доступные переменные
{branch} - имя бранча

{home} - домашняя директория user

{domainname} - значение из robin:domain

{deploy_dir} - значение из dir:

{pr_number} - номер PullRequest

{username} - значение из robin:user

{server} - значение из robin:server

{repo_name} - имя репозитория

{repo_owner} - владелец репозитория
