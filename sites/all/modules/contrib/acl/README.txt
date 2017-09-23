
README.txt for ACL 7.x-1.x



>>>> Please feel free to suggest improvements and additions to this file! <<<<




Overview
--------

ACL has no UI of its own and unless some other module uses it, it won't appear
to add anything to your site. Only bother with this module if some other module
tells you to.

For client modules that want to implement by-user node access in a robust and
compatible way, ACL provides the required functionality.
For a sample implementation see the Forum Access module:
http://drupal.org/project/forum_access




Acknowledgements
----------------

Originally written for Drupal 5 and maintained by merlinofchaos.
Ported to Drupal 6 and 7 and maintained by salvis.




Upgrading from Drupal 6
-----------------------

Update to the latest D6 release, then upgrade as outlined in the Drupal 7 docs.




Troubleshooting
---------------

Even though ACL does not do anything by its own, Core recognizes it as a node
access module, and it requires you to rebuild permissions upon installation.

The client module is fully responsible for the correct use of ACL. It is very
unlikely that ACL should cause errors. 

If there is a node access problem, or if you intend to implement a module that
uses ACL, we highly recommend to use the Devel Node Access module as outlined
in the step-by-step instructions in
http://drupal.org/node/add/project-issue/acl




Support/Customizations
----------------------

Support by volunteers is available on

   http://drupal.org/project/issues/acl?status=All&version=7.x

Please consider helping others as a way to give something back to the community
that provides Drupal and the contributed modules to you free of charge.


For paid support and customizations of this module, help with implementing an
ACL client module, or other Drupal work, contact the maintainer through his
contact form:

   http://drupal.org/user/82964

