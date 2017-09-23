<?php
/**
 * @file
 * element.inc
 * Provides hooks and functions for element related tasks.
 */

/**
 * Implements hook_element_info().
 */
function icon_element_info() {
  $types['icon_selector'] = array(
    '#input' => TRUE,
    '#tree' => TRUE,
    '#title' => t('Icon'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#default_bundle' => '',
    '#default_icon' => '',
    '#default_wrapper' => '',
    '#default_wrapper_class' => '',
    '#process' => array('form_process_fieldset', 'form_process_icon_selector'),
    '#pre_render' => array('form_pre_render_fieldset'),
    '#theme_wrappers' => array('fieldset'),
    '#attached' => array('library' => array(array('icon', 'icon_selector'))),
  );
  return $types;
}

/**
 * Generates a #states selector to be used.
 *
 * @param string $key
 *   The target element.
 * @param array $element
 *   The relevant element, if any.
 *
 * @return string
 *   A constructed #states selector.
 */
function _icon_states_selector($key, array $element = NULL) {
  if (isset($element) && ($keys = isset($element['#parents']) ? $element['#parents'] : array())) {
    $keys[] = $key;
    $first = array_shift($keys);
    return ':input[name="' . $first. '[' . implode('][', $keys) . ']' . '"]';
  }
  return ':input[name="' . $key . '"]';
}

/**
 * Processes an icon select list form element.
 */
function form_process_icon_selector($element) {
  // Get default values.
  $default_bundle = isset($element['#default_bundle']) ? $element['#default_bundle'] : icon_bundle_defaults();
  if (!is_array($default_bundle)) {
    $default_bundle = icon_bundle_load($default_bundle);
  }
  $default_icon = isset($element['#default_icon']) ? $element['#default_icon'] : '';
  $default_value = '';
  if ($default_bundle && !empty($default_bundle['name']) && !empty($default_icon)) {
    $default_value = $default_bundle['name'] . '|' . $default_icon;
  }
  // Build the options array for available icons per bundle.
  $options = array();
  foreach (icon_bundles() as $bundle_name => $bundle) {
    if (!$bundle['status']) {
      continue;
    }
    foreach ($bundle['icons'] as $icon_key => $icon_value) {
      $icon_name = is_string($icon_key) ? $icon_key : $icon_value;
      if (is_array($icon_value) && isset($icon_value['name'])) {
        $icon_name = $icon_value['name'];
      }
      $icon_title = is_string($icon_value) ? $icon_value : $icon_name;
      if (is_array($icon_value) && isset($icon_value['title'])) {
        $icon_title = $icon_value['title'];
      }
      $options[$bundle['title']][$bundle['name'] . '|' . $icon_name] = $icon_title;
    }
  }
  if (!count($options)) {
    $element['#description'] = t('There are no icons available to choose from. They either do not exist or are disabled. As a result, this icon selector has also been disable.');
    $element['#description'] .= '<br />' . l(t('Manage icon bundles'), ICON_ADMIN_PATH);
    $element['#disabled'] = TRUE;
    // Set the default value.
    if (!empty($default_value)) {
      $options = array($default_value => $default_icon);
    }
  }
  $element['icon'] = array(
    '#title' => t('Select Icon'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => isset($element['#default_value']) ? $element['#default_value'] : $default_value,
    '#empty_option' => '- No Icon -',
    '#required' => !empty($element['#required']),
    '#weight' => -10,
  );

  $default_wrapper = isset($element['#default_wrapper']) ? $element['#default_wrapper'] : '';
  $element['wrapper'] = array(
    '#type' => 'select',
    '#title' => t('Icon Wrapper'),
    '#description' => t('Choose an HTML element to wrap the icon with.'),
    '#options' => icon_wrapper_options(),
    '#default_value' => $default_wrapper,
    '#states' => array(
      'invisible' => array(
        _icon_states_selector('icon', $element) => array('value' => ''),
      ),
    ),
    '#weight' => -9,
  );

  $default_wrapper_class = isset($element['#default_wrapper_class']) ? $element['#default_wrapper_class'] : '';
  $element['wrapper_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon Wrapper Classes'),
    '#description' => t('A space separated list of CSS classes. Token replacement patterns are allowed.'),
    '#default_value' => $default_wrapper_class,
    '#size' => 60,
    '#maxlength' => 128,
    '#states' => array(
      'invisible' => array(
        array(_icon_states_selector('icon', $element) => array('value' => '')),
        array(_icon_states_selector('wrapper', $element) => array('value' => '')),
      ),
    ),
    '#weight' => -8,
  );

  $element['wrapper_class_token'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        array(_icon_states_selector('icon', $element) => array('value' => '')),
        array(_icon_states_selector('wrapper', $element) => array('value' => '')),
      ),
    ),
    '#weight' => -7,
  );

  if (module_exists('token')) {
    $element['wrapper_class_token']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => isset($element['#token_types']) ? $element['#token_types'] : 'all',
      '#global_types' => FALSE,
      '#dialog' => TRUE,
    );
  }
  else {
    $element['wrapper_class_token']['help'] = array(
      '#type' => 'item',
      '#markup' => t('Install the <a href="http://drupal.org/project/token" target="_blank" title="Token module project page">Token</a> module to view available replacement patterns.'),
    );
  }

  return $element;
}

/**
 * Implements form_type_ELEMENT_value().
 *
 * Value callback for the 'icon_selector' element.
 */
function form_type_icon_selector_value(&$element, $input = FALSE, &$form_state = array()) {
  if ($input !== FALSE && isset($input['icon']) && strpos($input['icon'], '|') !== FALSE) {
    list($bundle, $icon) = explode('|', $input['icon']);
    $wrapper = !empty($input['wrapper']) && in_array($input['wrapper'], array_keys(icon_wrapper_options())) ? $input['wrapper'] : '';
    $wrapper_class = !empty($input['wrapper_class']) ? filter_xss_admin($input['wrapper_class']) : '';
    drupal_array_set_nested_value($form_state['values'], array_merge($element['#parents'], array('bundle')), $bundle, TRUE);
    drupal_array_set_nested_value($form_state['values'], array_merge($element['#parents'], array('icon')), $icon, TRUE);
    drupal_array_set_nested_value($form_state['values'], array_merge($element['#parents'], array('wrapper')), $wrapper, TRUE);
    drupal_array_set_nested_value($form_state['values'], array_merge($element['#parents'], array('wrapper_class')), $wrapper_class, TRUE);
  }
}
