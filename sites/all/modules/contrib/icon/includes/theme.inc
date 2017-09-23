<?php
/**
 * @file
 * theme.inc
 * Provides hooks and functions for theme related tasks.
 */

/**
 * Implements hook_theme().
 *
 * {@inheritdoc}
 */
function icon_theme($existing, $type, $theme, $path) {
  $include_path = drupal_get_path('module', 'icon') . '/includes';
  $hooks['icon_bundle_overview_form'] = array(
    'render element' => 'form',
    'file' => 'admin.inc',
    'path' => $include_path,
  );
  $hooks['icon'] = array(
    'file' => 'theme.inc',
    'path' => $include_path,
    'variables' => array(
      'attributes' => array(),
      'icon' => NULL,
      'bundle' => NULL,
      'token_data' => array(),
      'wrapper' => '',
      'wrapper_attributes' => array(),
      'wrapper_class' => '',
    ),
  );
  // Add theming hooks for render hooks so they can control their own output.
  // Due to a nasty bug in template suggestions and preprocessing, we can't do
  // something like icon_RENDER_HOOK__BUNDLE and have it be completely dynamic
  // as it probably should be.
  // @see: http://drupal.org/node/956520
  $render_hooks = array();
  foreach (icon_render_hooks() as $hook) {
    if (!isset($render_hooks[$hook['name']])) {
      $hooks['icon_' . $hook['name']] = array(
        'file' => $hook['file'],
        'path' => $hook['path'],
        'variables' => array(
          'attributes' => array(),
          'icon' => NULL,
          'bundle' => NULL,
          'token_data' => array(),
          'wrapper' => '',
          'wrapper_attributes' => array(),
          'wrapper_class' => '',
        ),
      );
    }
  }
  return $hooks;
}

/**
 * Implements hook_preprocess_icon().
 *
 * {@inheritdoc}
 */
function template_preprocess_icon(&$variables) {
  $bundle = &$variables['bundle'];
  $icon = &$variables['icon'];
  if (empty($bundle) || empty($icon)) {
    return;
  }
  if (is_string($bundle)) {
    $bundle = icon_bundle_load($bundle);
  }
  $render_hooks = array_keys(icon_render_hooks());

  // Check to see if the bundle is valid, the icon is a part of the bundle and
  // the bundle has a valid render hook assigned.
  if (!$bundle || !$bundle['status'] || !(isset($bundle['icons'][$icon]) || in_array($icon, $bundle['icons']) || !in_array($bundle['render'], $render_hooks))) {
    $bundle = FALSE;
    $icon = FALSE;
    return;
  }

  // Add classes to the icon wrapper_attributes variable.
  $wrapper_attributes = &$variables['wrapper_attributes'];
  if (!empty($variables['wrapper_class'])) {
    $classes = array();
    foreach (explode(' ', $variables['wrapper_class']) as $class) {
      if ($class = trim(token_replace($class, $variables['token_data'], array('clear' => TRUE)))) {
        $classes[] = drupal_html_class($class);
      }
    }
    if ($classes) {
      if (!isset($wrapper_attributes['class'])) {
        $wrapper_attributes['class'] = array();
      }
      $wrapper_attributes['class'] = array_unique(array_merge($wrapper_attributes['class'], $classes));
    }
  }

  // Add default icon class.
  $attributes = &$variables['attributes'];
  if (!isset($attributes['class'])) {
    $attributes['class'] = array();
  }
  $attributes['class'][] = 'icon';

  // Prevent screen readers from reading icons.
  $attributes['aria-hidden'] = 'true';

  // Load additional libraries, CSS, JavaScript an other custom
  // attached data associated with this bundle.
  icon_process_attached($bundle);
}

/**
 * Theming responsibility is always passed to the bundle provider.
 *
 * @param array $variables
 *   The variables array.
 *
 * WARNING: This theming hook should never be overridden.
 */
function theme_icon($variables) {
  $output = '';
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  if (!empty($bundle) && !empty($icon)) {
    // Render the icon.
    // @todo This should be a render array, but converting the variables to
    // render array properties is needed to do that and this is proxying them.
    $output = theme('icon_' . $bundle['render'], $variables);

    // Wrap icon, if necessary.
    if (($wrapper = $variables['wrapper']) && in_array($wrapper, array_keys(icon_wrapper_options()))) {
      $build = array(
        '#theme' => 'html_tag__icon_wrapper',
        '#tag' => $wrapper,
        '#attributes' => $variables['wrapper_attributes'],
        '#value' => $output,
      );
      $output = drupal_render($build);
    }
  }
  return $output;
}
