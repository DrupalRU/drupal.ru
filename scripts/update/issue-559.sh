#!/bin/sh

# Add new modules
drush -y dl admin_menu adminimal_admin_menu login_destination module_filter
drush -y en admin_menu adminimal_admin_menu login_destination module_filter