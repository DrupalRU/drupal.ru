
Localization Update
-------------------
  Automatically download and update your translations by fetching them from
  https://localize.drupal.org or any other Localization server.

  The l10n update module helps to keep the translation of your drupal core and
  contributed modules up to date with the central Drupal translation repository
  at https://localize.drupal.org. Alternatively locally stored translation files
  can be used as translation source too.

  By choice updates are performed automatically or manually. Locally altered
  translations can either be respected or ignored.

  The l10n update module is developed for:
   * Distributions which include their own translations in .po files.
   * Site admins who want to update the translation with each new module
     revision.
   * Site builders who want an easy tool to download translations for a site.
   * Multi-sites that share one translation source.

  Project page:  https://www.drupal.org/project/l10n_update
  Support queue: https://www.drupal.org/project/issues/l10n_update

Installation
------------
  Download, unpack the module the usual way.
  Enable this module and the Locale module (core).

  You need at least one language (besides the default English).
  On Administration > Configuration > Regional and language > Languages:
    Click "Add language".
    Select a language from the select list "Language name".
    Then click the "Add language" button.

  Drupal is now importing interface translations. This can take a few minutes.
  When it's finished, you'll get a confirmation with a summary of the
  translations that have been imported.

  If required, enable the new language as default language.
  Administration > Configuration > Regional and language > Languages:
    Select your new default language.

Update interface translations
-----------------------------
  You want to import translations regularly using cron. You can enable this
  on Administration > Configuration > Regional and language > Languages:
   * Choose the "Translation updates" tab.
   * Change "Check for updates" to "Daily" or "Weekly" instead of the default
     "Never".
  From now on cron will check for updated translations, and import them is
  required.

  The status of the translations is reported on the "Status report" page at
  Administration > Reports.

  To check the translation status and execute updates manually, go to
    Administration > Configuration > Regional and language > Translate inteface
    Choose the "Update" tab.
  You see a list of all modules and their translation status.
  On the bottom of the page, you can manually update using "Update
  translations".

Use Drush
---------
  You can also use drush to update your translations:
    drush l10n-update           # Update translations.
    drush l10n-update-refresh   # Refresh available information.
    drush l10n-update-status    # Show translation status of available project


Summary of administrative pages
-------------------------------
  Translations status overview can be found at
    Administration > Configuration > Regional and language > Languages
    > Translation updates

  Update configuration settings can be found at
    Administration > Configuration > Regional and language > Translate interface
    > Update

Translating Drupal core, modules and themes
-------------------------------------------
  When Drupal core or contributed modules or themes get installed, Drupal core
  checks if .po translation files are present and updates the translations with
  the strings found in these files. After this, the localization update module
  checks the localization server for more recent translations, and updates
  the site translations if a more recent version was found.
  Note that the translations contained in the project packages may become
  obsolete in future releases.

  Changes to translations made locally using the site's build in translation
  interface (Administer > Site building > Translate interface > Search) and
  changes made using the localization client module are marked. Using the
  'Update mode' setting 'Edited translations are kept...', locally edited
  strings will not be overwritten by translation updates.
  NOTE: Only manual changes made AFTER installing Localization Update module
  are preserved. To preserve manual changes made prior to installation of
  Localization Update module, use the option 'All existing translations are
  kept...'.

po files, multi site and distributions
--------------------------------------
  Multi sites and other installations that share the file system can share
  downloaded translation files. The Localization Update module can save these
  translations to disk. Other installations can use these saved translations
  as their translation source.

  All installations that share the same translation files must be configured
  with the same 'Store downloaded files' file path e.g.
  'sites/all/translations'.
  Set the 'Update source' of one installation to 'Local files and remote server'
  or 'Remote server only', all other installations are set to
  'Local files only' or 'Local files and remote server'.

  Translation files are saved with the following file name syntax:

    <module name>-<release>.<language code>.po

  For example:
    masquerade-6.x-1.5.de.po
    tac_lite-7.x-1.0-beta1.nl.po

  Po files included in distributions should match this syntax too.

Missing translations
--------------------

  If you see "Missing translations for ..." on the Translate interface update
  page this means that Localization Update module was not able to find a
  translation file for one or more of your modules or themes. This is usually
  the case with features, but can also occur with custom modules.

  In case of custom modules, remove the line "project = ..." from the .info
  file. Only use "project = ..." for modules that are available at drupal.org or
  custom modules for which an alternative source of translation is provided (see
  below). In case of a feature add the feature machine name to 'Project' at
  admin/config/regional/language/update > Disable update. This will prevent
  Localization Update module from checking for updates for this feature.

  If "missing translations" lists all your enabled modules the webserver has no
  access to ftp.drupal.org. Contact your hosting to allow access.

  If only a few of your contributed modules are in the list, first verify that
  the translation is actually missing by visiting the listed URL of the .po file
  (for example "https://ftp.drupal.org/files/translations/7.x/views/views.hu.po")
  When the file is not found, you either try again later or contact a
  translation administrator of your language at https://localize.drupal.org/.

Alternative source of translation
---------------------------------

  The download path pattern is normally defined in the above defined xml file.
  You may override the download path of the po file on a project by project
  basis by adding this definition in the .info file:

    l10n path = http://example.com/files/translations/%core/%project/%project-%release.%language.po

  Modules can force Locale to load the translation of an other project by
  defining 'interface translation project' in their .info file. This can be
  useful for custom modules to use for example a common translation file

    interface translation project = my_project

  This can be used in combination with an alternative path to the translation
  file. For example:

    l10n path = sites/all/modules/custom/%project/%project.%language.po

Exclude a project from translation checks and updates
-----------------------------------------------------

  Individual modules can be excluded from translation checks and updates. For
  example custom modules or features. Add the following line to the .info file
  to exclude a module from translation checks and updates:

  interface translation project = FALSE

API
---
  Using hook_l10n_update_projects_alter modules can alter or specify the
  translation repositories on a per module basis.

  See l10n_update.api.php for more information.

Using a Proxy
-------------

  Use the cURL HTTP Request module (https://www.drupal.org/project/chr) if your
  website is behind a proxy. If you want to use an alternative http request
  function, set the 'drupal_http_request_function', the same way as you would
  set it to override Drupal core's http_request function:

    $conf['drupal_http_request_function'] = 'my_custom_http_request';

Maintainers
-----------
  Erik Stielstra
  Gábor Hojtsy
  Jose Reyero
