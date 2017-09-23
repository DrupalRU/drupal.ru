<?php
/**
 * @file
 * html.vars.php
 *
 * @see html.tpl.php
 */

/**
 * Implements hook_preprocess_html().
 */
function druru_preprocess_html(&$variables) {
  switch (theme_get_setting('druru_navbar_position')) {
    case 'fixed-top':
      $variables['classes_array'][] = 'navbar-is-fixed-top';
      break;

    case 'fixed-bottom':
      $variables['classes_array'][] = 'navbar-is-fixed-bottom';
      break;

    case 'static-top':
      $variables['classes_array'][] = 'navbar-is-static-top';
      break;
  }

  // Stylize 404 and 403 pages.
  // Works only in case if in settings of the site not set 403 and 404 page.
  _druru_error_pages_preprocess('html', $vars);
}
