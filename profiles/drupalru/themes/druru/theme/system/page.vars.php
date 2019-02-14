<?php
/**
 * @file
 * page.vars.php
 */

/**
 * Implements hook_preprocess_page().
 *
 * @see page.tpl.php
 */
function druru_preprocess_page(&$variables) {
  // Generate correct grid classes of bootstrap.
  _druru_generate_columns_classes($variables);
  // Generate additional classes.
  _druru_generate_classes($variables);

  // Search and run custom preprocess functions.
  foreach ($variables['theme_hook_suggestions'] as $suggestion) {
    $theme = $GLOBALS['theme'];
    $preprocess_function = $theme . '_preprocess_' . $suggestion;
    if (function_exists($preprocess_function)) {
      $preprocess_function($variables);
    }
  }

  // Remove last item from breadcrumbs added by views in view 'tracker_my'
  $page = menu_get_item();
  if ($page['page_callback'] == 'views_page' && $page['page_arguments'][0] == 'tracker_my') {
    // Remove breadcrumbs from view arguments.
    // This assumes there's only one breadcrumb to take off the end
    // If there are more you could change the array_slice
    $breadcrumb = drupal_get_breadcrumb();
    $new_breadcrumb = array_slice($breadcrumb, 0, count($breadcrumb) - 1);
    drupal_set_breadcrumb($new_breadcrumb);
  }

  // Stylize 404 and 403 pages.
  // if it not override in settings on admin-side.
  _druru_error_pages_preprocess('page', $variables);

  _druru_struct_logo($variables);
}

/**
 * Generate custom classes for page elements.
 */
function _druru_generate_classes(&$variables) {
  // Primary nav.
  $variables['primary_nav'] = FALSE;
  if ($variables['main_menu']) {
    // Build links.
    $variables['primary_nav'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    // Provide default theme wrapper function.
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
  }

  // Secondary nav.
  $variables['secondary_nav'] = FALSE;
  if ($variables['secondary_menu']) {
    // Build links.
    $variables['secondary_nav'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    // Provide default theme wrapper function.
    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__secondary');
  }

  $variables['navbar_classes_array'] = array('navbar');

  if (theme_get_setting('druru_navbar_position') !== '') {
    $variables['navbar_classes_array'][] = 'navbar-'
      . theme_get_setting('druru_navbar_position');
  }
  else {
    $variables['navbar_classes_array'][] = 'container';
  }
  if (theme_get_setting('druru_navbar_inverse')) {
    $variables['navbar_classes_array'][] = 'navbar-inverse';
  }
  else {
    $variables['navbar_classes_array'][] = 'navbar-default';
  }
}

/**
 * Implements hook_process_page().
 *
 * @see page.tpl.php
 * @todo Test and remove if not required
 */
function druru_process_page(&$variables) {
  $variables['navbar_classes'] = implode(' ', $variables['navbar_classes_array']);
  if (!empty($variables['page_dropdown_menu'])) {
    druru_construct_contextual_menu($variables['page_dropdown_menu'], $variables);
  }
  if (isset($variables['node'])) {
    $title = &$variables['title'];
  }
}

/**
 * Adding a logo to variables.
 */
function _druru_struct_logo(&$variables) {
  $styles = array();
  $is_logo = FALSE;
  $site_name = variable_get('site_name');
  $site_slogan = variable_get('site_slogan');
  $is_svg_logo = theme_get_setting('svg_logo');
  $svg_logo_path = theme_get_setting('svg_logo_path');
  $logo_path = theme_get_setting('logo_path');
  $need_show_logo = theme_get_setting('toggle_logo');
  $logo = NULL;
  $brand = NULL;

  if ($need_show_logo) {
    // Svg logo.
    if ($is_svg_logo) {
      $logo_path = file_create_url($svg_logo_path);
      $styles[] = "background-image: url('$logo_path')";

      // Create styles for logo.
      if ($height = theme_get_setting('logo_height')) {
        $styles[] = "height: {$height}px";
      }
      if ($width = theme_get_setting('logo_width')) {
        $styles[] = "padding-left: {$width}px";
      }
      $styles[] = 'background-repeat: no-repeat';
      $is_logo = FALSE;
    }
    elseif ($logo_path) {
      $logo = theme('image', array(
        'path'       => $variables['logo'] ?: $logo_path,
        'alt'        => $site_name . ' logo',
        'attributes' => array(
          'id' => 'site-logo',
        ),
      ));
      $is_logo = FALSE;
    }
  }

  if ($site_name && theme_get_setting('toggle_name')) {
    $brand .= "<div class='site-name'>{$site_name}</div>";
  }
  if ($site_slogan && theme_get_setting('toggle_slogan')) {
    $brand .= "<div class='site-slogan'>{$site_slogan}</div>";
  }
  if ($brand || ($is_logo && theme_get_setting('toggle_logo'))) {
    $variables['logo'] = NULL;
    if ($logo) {
      $variables['logo'] = l($logo, $variables['front_page'], array(
        'attributes' => array(
          'alt'   => t('Home'),
          'class' => array('navbar-btn', 'frontpage-link'),
        ),
        'html'       => TRUE,
      ));
    }
    $brand = "<div class='name navbar-brand'>$brand</div>";
    $variables['logo'] .= l($brand, $variables['front_page'], array(
      'attributes' => array(
        'alt'   => t('Home'),
        'class' => array('navbar-btn', 'frontpage-link'),
      ),
      'html'       => TRUE,
    ));

    $logo_styles = '.navbar .navbar-brand { ' . implode(';', $styles) . '}';
    drupal_add_css($logo_styles, array(
      'type'  => 'inline',
      'group' => CSS_THEME,
    ));
  }
}

/**
 * Generate layout classes.
 * @todo Refactor to make code simpler and clearer
 */
function _druru_generate_columns_classes(&$variables) {
  // Detecting columns.
  $first_column_exists = !empty($variables['page']['sidebar_first']);
  $second_column_exists = !empty($variables['page']['sidebar_second']);

  // In case if these custom attributes defined somewhere.
  $variables['sidebar_first_attributes'] = @$variables['sidebar_first_attributes'] ?: array();
  $variables['sidebar_second_attributes'] = @$variables['sidebar_second_attributes'] ?: array();
  $variables['content_column_attributes'] = @$variables['content_column_attributes'] ?: array();

  // To short variables, which contains attributes.
  // ca meas "column attributes".
  $first_ca = &$variables['sidebar_first_attributes'];
  $second_ca = &$variables['sidebar_second_attributes'];
  $content_ca = &$variables['content_column_attributes'];

  $first_ca['role'] = 'complementary';
  $second_ca['role'] = 'complementary';

  // In case if classes defined in these custom attributes.
  $first_ca['class'] = @$first_ca['class'] ?: array();
  $second_ca['class'] = @$second_ca['class'] ?: array();
  $content_ca['class'] = @$content_ca['class'] ?: array();

  // To short variables.
  // cc means "columns classes".
  $first_cc = &$first_ca['class'];
  $second_cc = &$second_ca['class'];
  $content_cc = &$content_ca['class'];

  // Be sure that we have array of classes.
  if (is_scalar($first_cc)) {
    $first_cc = array($first_cc);
  }
  if (is_scalar($second_cc)) {
    $second_cc = array($second_cc);
  }
  if (is_scalar($content_cc)) {
    $content_cc = array($content_cc);
  }

  // Define default classes.
  $first_cc[] = 'first';
  $second_cc[] = 'second';
  $content_cc[] = 'main';

  switch (TRUE) {

    // One column.
    case $first_column_exists xor $second_column_exists:
      // When exists first column only.
      if ($first_column_exists) {
      }
      // When exists second column only.
      elseif ($second_column_exists) {
      }

      $first_cc[] = 'col-vsd-4';
      $first_cc[] = 'col-sd-3';
      $first_cc[] = 'col-lg-3';

      $second_cc[] = 'col-vsd-4';
      $second_cc[] = 'col-sd-3';
      $second_cc[] = 'col-lg-3';

      $content_cc[] = 'col-vsd-8';
      $content_cc[] = 'col-sd-9';
      $content_cc[] = 'col-lg-9';
      break;

    // Two columns.
    case $first_column_exists && $second_column_exists:
      $first_cc[] = 'col-vsd-4';
      $first_cc[] = 'col-sd-3';
      $first_cc[] = 'col-lg-2';

      $second_cc[] = 'col-vsd-4';
      $second_cc[] = 'col-sd-3';
      $second_cc[] = 'col-lg-2';
      $second_cc[] = 'col-sd-push-6';
      $second_cc[] = 'col-lg-push-8';

      $content_cc[] = 'col-vsd-8';
      $content_cc[] = 'col-sd-6';
      $content_cc[] = 'col-lg-8';
      $content_cc[] = 'col-sd-pull-3';
      $content_cc[] = 'col-lg-pull-2';
      break;

    // Without columns.
    default:
      $content_cc[] = 'col-xs-12';
      break;
  }
}

/**
 * Preprocess for messages page of Privatemsg module.
 */
function druru_preprocess_page__messages(&$variables) {
  // Exclude all child suggestions like 'page__messages__new' or 'page__messages__list'.
  $pattern = '/page\_\_messages\_\_\w/';
  if (preg_grep($pattern, $variables['theme_hook_suggestions'])) {
    return;
  }

  // Adds form for creating new message right in list of correspondence.
  // Will have incorrect behavior when module "Privatemeg filter" will enabled.
  if (isset($variables['page']['content']['system_main'])) {
    $default_page = $variables['page']['content']['system_main'];
    // Need to save current title, because in other case title
    // will overridden in form of privatemsg_new.
    $current_title = drupal_get_title();
    $new_message = drupal_get_form('privatemsg_new');
    drupal_set_title($current_title, PASS_THROUGH);
    $variables['page']['content']['system_main'] = array(
      'top'    => $new_message,
      'bottom' => $default_page,
    );
    $variables['page']['content']['system_main']['top']['#weight'] = 1;
    $variables['page']['content']['system_main']['bottom']['#weight'] = 2;
  }
}
