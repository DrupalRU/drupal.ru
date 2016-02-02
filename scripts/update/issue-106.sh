#!/bin/sh

#remove acl module

drush -y dis acl
drush -y pm-uninstall acl
