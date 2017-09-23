
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Features
 * Requirements
 * Installation & Upgrading
 * Using Birthdays
 * Maintainers


INTRODUCTION
------------

The Birthdays module allows users to add their birthday to their profile. In
their profile the date of birth can be shown, as well as their age and their
star sign. This is all configurable.

You can also list the birthdays on blocks and pages using Views. You can filter
by day, month and year, display only N upcoming birthdays and so on.

It is optional to send users an email or execute another action on their
birthday, and the administrator can receive periodic reminders of who are having
their birthday next day, week or month.


FEATURES
--------

 * Provides a field type that can be used on users, profiles or whatever entity
   you want.
 * Display a birthday input field on registration (or not).
 * A block displaying the next X birthdays, using Views.
 * A block listing birthdays for the next X days, using Views.
 * A page listing all the users and their birthdays/age/star sign, filtered by
   month and year, using Views.
 * Displaying the user's birthday/current age/star sign on the user's profile.
 * User option not to be mailed on their birthday.
 * User & admin option to hide the year and age of the user(s).
 * User options can be turned off by administrator.
 * Optionally send e-mails to administrator with upcoming birthdays for the
   next day, week or month.
 * Optionally sends an email to the user on their birthday. Other actions are
   also possible because Birthdays integrates with the Triggers module.
 * PGSQL support


REQUIREMENTS
------------

This module requires only the Field module that is included in Drupal core, but
it's functionality can be extended using Views, Triggers, Profile 2 and other
contrib modules.


INSTALLATION & UPGRADING
------------------------

See http://drupal.org/documentation/install/modules-themes/modules-7
for instrunctions on installing contrib modules.

Make sure you read UPGRADE.txt before upgrading from Drupal 6.


USING BIRTHDAYS
---------------

  The birthday field type
  -----------------------

  Birthdays module provides a field type for birthdays. You can use birthday
  fields for all entity types. Use the "Manage fields" page of your content
  type / entity type / bundle to add the field. You can also go there to change
  configuration options later.
  These field instance settings are available:

   * Display during registration (if on user entity)
   * Allow the user to hide the year of birth / always hide the year of birth /
     require a year of birth
   * Send regular emails reminding of upcoming birthdays
   * Allow the user to opt-out trigger integration

  Birthdays defaults
  ------------------

  Adds a birthday field to the user entity type, provides a default view and a
  default "Happy birthday mail" action.

  Triggers & Actions
  ------------------

  Triggers module allows you to execute actions on birthdays. Birthdays module
  has a tab on the Triggers configuration page, where you can assign actions to
  execute for each field instance.
  The assigned actions are fired during cron runs.
  Note that the birthday field type has also a setting, to allow the user to
  opt-out of triggers.

  Views
  -----

  Birthdays defaults provides a default page and block, but you can create more
  custom views.
  You can use birthday fields as fields, for sorting and for filtering. The
  field has clicksort support. You can sort by absolute timestamp, time to next
  birthday or day of the year. You can filter by absolute values or offsets in
  days. Also day, month and year column are available as seperate integer
  columns.


MAINTAINERS
-----------

 * David Gildeh (Drazzig) - http://drupal.org/user/26260
 * Maarten van Grootel (maartenvg) - http://drupal.org/user/109716
 * Niklas Fiekas - http://drupal.org/user/1089248 - niklas.fiekas@googlemail.com
