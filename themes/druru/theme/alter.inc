<?php
/**
 * @file
 * alter.inc
 *
 * Contains various implementations of hook_*_alter().
 *
 * Theme `*.alter.php` files are stored in the `theme` directory.
 * It is enabled by the processing done below. This type of files can
 * be grouped into sub-directories, also. It is recommended that they are
 * grouped by the modules they originate from. Theme specific
 * hooks should be grouped into a folder named after the theme itself.
 */

/**
 * Detect and include `alter` files.
 *
 * The function performs watching of `theme` folder and
 * search all files by template *.alter.php
 * By design we should to create files like `form.alter.inc`
 * in folder `theme/module` and containing all implementations
 * of hook_form_FORM_ID_alter() hook.
 *
 * Also, we can create files like `theme/module/some-form-id.alter.php`,
 * which contain only one hook like druru_form_some_form_id_alter().
 */
function druru_include_alters() {
  $path  = drupal_get_path('theme', 'druru') . '/theme';
  $regex = '/' . str_replace('.', '\.', '.alter.php') . '$/';
  // Third key should be set to 'uri' because,
  // files can have same names and in this case will be
  // included last of received file with same name.
  $files = drupal_system_listing($regex, $path, 'uri');
  // Detect all files with alters.
  if ($files && ($file = reset($files))) {
    do {
      require_once DRUPAL_ROOT . '/' . $file->uri;
    } while ($file = next($files));
  }
}

/**
 * Include #pre_render callbacks for elements.
 */
druru_include('druru', 'theme/pre-render.inc');

/**
 * Include #process callbacks for elements.
 */
druru_include('druru', 'theme/process.inc');

/**
 * Include alters.
 */
druru_include_alters();
