Mail Editor
===========

This module provides the ability to customize e-mail templates for mail sent
out using drupal_mail(). On multi-language sites it supports all available
languages.

Users with the 'Administer mail templates' and 'Use the administration pages
and help' permissions may go to admin/config/system/mail-edit, where they find
list of all templates that can be customized. If you use a separate admin
theme, then the 'View the administration theme' permission may be needed, too.

To add Mail Editor support to a third-party module, refer to the
modules/mail_edit.user.inc file, which shows how Mail Editor support was
added to the User core module. Any module defining those three hooks
automatically appears in Mail Editor.
