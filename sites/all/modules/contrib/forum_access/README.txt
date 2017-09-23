
README.txt for Forum Access 7.x-1.x



>>>> Please feel free to suggest improvements and additions to this file! <<<<




Overview
--------

Forum Access changes your forum administration pages to let you apply
role-based permissions to each forum, and to give each forum individual
moderators.

Moderators automatically get all privileges on all posts in that forum,
including edit and delete.




Requirements
------------

Forum Access requires the current versions of the following modules:
  - ACL                      http://drupal.org/project/acl
  - Chain Menu Access API    http://drupal.org/project/chain_menu_access




Acknowledgements
----------------

Originally written for Drupal 5 and maintained by merlinofchaos.
Ported to Drupal 6 and 7 and maintained by salvis.




Upgrading from Drupal 6
-----------------------

Update to the latest D6 release, then upgrade as outlined in the Drupal 7 docs.

Note: The D5 Legacy Mode has been removed (see http://drupal.org/node/1768330).




Permissions
-----------

Administering Forum Access requires Core's Administer Forums permission.
Detailed explanations of how Forum Access' grants work together with Core's
other permissions are available on the administration pages.




Configuration
-------------

Forum Access does not have its own administration page -- it adds its controls
to Core's forum administration pages at

   Administration | Structure | Forums | Edit/Add Forum/Container

   admin/structure/forum/edit/forum/1




Troubleshooting
---------------

Step-by-step troubleshooting instructions are provided on the administration
pages.

In case you have additional node access modules enabled, the administration
pages will provide additional information on how to make them work together,
and you should probably follow the troubleshooting instructions to install
DNA and learn about how your combination of node access modules works.




Support/Customizations
----------------------

Support by volunteers is available on

   http://drupal.org/project/issues/forum_access?status=All&version=7.x

Please consider helping others as a way to give something back to the community
that provides Drupal and the contributed modules to you free of charge.


For paid support and customizations of this module or other Drupal work
contact the maintainer through his contact form:

   http://drupal.org/user/82964

