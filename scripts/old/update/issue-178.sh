#!/bin/sh

echo "Enable metatag"

drush -y dl metatag
drush -y en metatag

#default settings
drush sqlq "DELETE FROM metatag_config; INSERT INTO metatag_config VALUES (1,'global:frontpage','a:4:{s:5:\"title\";a:1:{s:5:\"value\";s:47:\"[site:name] | [current-page:pager][site:slogan]\";}s:11:\"description\";a:1:{s:5:\"value\";s:13:\"[site:slogan]\";}s:9:\"canonical\";a:1:{s:5:\"value\";s:10:\"[site:url]\";}s:9:\"shortlink\";a:1:{s:5:\"value\";s:10:\"[site:url]\";}}');"
