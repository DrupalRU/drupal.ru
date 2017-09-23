<?php

/**
 * @file
 * API documentation for Localize updater module.
 */

/**
 * Alter the list of project to be updated by Localization Update.
 *
 * L10n_update uses the same list of projects as update module. Using this hook
 * the list can be altered. This hook is typically used to alter the following
 * values from the .info file:
 *  - interface translation project
 *  - l10n path.
 *
 * @param array $projects
 *   Array of projects.
 */
function hook_l10n_update_projects_alter(array &$projects) {

  foreach (array_keys($projects) as $name) {
    // @todo These 'interface translation project' examples are good for system_projects_alter.
    // Make all custom_* modules use the 'custom_module' module translation
    // file.
    if (strpos($name, 'custom_') === 0) {
      $projects[$name]['info']['interface translation project'] = 'custom_module';
    }

    // Disable interface translation updates for all features.
    if (strpos($name, 'feature_') === 0) {
      $projects[$name]['info']['interface translation project'] = FALSE;
    }
  }

  // Set the path to the custom module translation files if not already set.
  if (isset($projects['custom_module']) && empty($projects['custom_module']['info']['l10n path'])) {
    $path = drupal_get_path('module', 'custom_module');
    $projects['custom_module']['info']['l10n path'] = $path . '/translations/%language.po';
  }

  // With this hook it is also possible to add a new project which does not
  // exist as a real module or theme project but is treated by the localization
  // update module as one. The below data is the minimum to be specified.
  // As in the previous example the 'l10n path' element is optional.
  $projects['new_example_project'] = array(
    'project_type'  => 'module',
    'name' => 'new_example_project',
    'info' => array(
      'name' => 'New example project',
      'version' => '7.x-1.5',
      'core' => '7.x',
      'l10n path' => 'http://example.com/files/translations/%core/%project/%project-%release.%language.po',
    ),
  );
}

/**
 * Alter the list of languages to be updated by Localization Update.
 *
 * @param array $langcodes
 *   Language codes of languages to be updated by Localization Update module.
 */
function hook_l10n_update_languages_alter(array &$langcodes) {
  // Use a different language code for Dutch translations.
  if (isset($langcodes['nl'])) {
    $langcodes['nl-NL'] = $langcodes['nl'];
    unset($langcodes['nl']);
  }
}
