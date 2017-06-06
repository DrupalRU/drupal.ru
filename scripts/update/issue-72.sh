#!/bin/sh

echo "Switching to use drupal.org theme"

rm -rf sites/all/themes/bootstrap_lite
drush -y dl bootstrap_lite
