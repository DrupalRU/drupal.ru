-----------------------------------------------------------
bloginfo.module for Drupal
  by Matt Farina
  contact: http://drupal.org/user/25701/contact
-----------------------------------------------------------

The bloginfo module adds a blog title and description to your blogs.  It is a simple module that adds 2 additional fields to the users account screen, for those who have permission, to have a blog title and blog description.  These are then put into a block that can be placed like any other block.  The title is the block title and the description is the block content.

The idea is to have a blog title and description like one via blogger.com.

The block will only display on a users blog list or blog post who have filled in the settings.

INSTALL
-------
1) Place the folder bloginfo into your drupal modules directory.
2) Go to admin/modules and activate the bloginfo module
3) Go to admin/access and set the access privileges to use this module.

UPGRADE
-------
1) Replace the bloginfo folder in the modules folder.
2) Run update.php to update the database.