#!/usr/bin/env bash

drush @dru.prod cc all
drush @dru.prod wd-del all -y
drush @dru.prod sql-sync default temp --sanitize -y --structure-tables-list=\
xmlsitemap,node,node_comment_statistics,alttracker_node,forum,pm_index,\
node_revision,field_revision_body,taxonomy_index,alttracker_user,profile_value,\
field_data_comment_body,comment,comment_notify,\
field_data_taxonomy_vocabulary_7,field_data_taxonomy_vocabulary_2,\
field_revision_taxonomy_vocabulary_7,field_revision_taxonomy_vocabulary_2
drush @dru.temp scr "profiles/drupalru/scripts/sync/sanitize.php"
drush @dru.temp ucrt admin --password=111
drush @dru.temp sql-dump --result-file="$HOME/domains/drupal.ru/sites/default/files/drupalru-dump.sql" --gzip
