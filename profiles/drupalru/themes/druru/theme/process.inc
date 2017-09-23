<?php
/**
 * @file
 * process.inc
 *
 * Contains various implementations for #process callbacks on elements.
 */

/**
 * Process all elements.
 */
function _druru_process_element(&$element, &$form_state) {
  if (!empty($element['#attributes']['class']) && is_array($element['#attributes']['class'])) {
    if (in_array('container-inline', $element['#attributes']['class'])) {
      $element['#attributes']['class'][] = 'form-inline';
    }
    if (in_array('form-wrapper', $element['#attributes']['class'])) {
      $element['#attributes']['class'][] = 'form-group';
    }
  }
  // Stylize checkboxes select.
  if (!empty($element['#thumbnails'])) {
    $element['#attributes']['class'][] = 'thumbnails-select';
    foreach (element_children($element) as $index) {
      $element[$index]['#thumbnail'] = TRUE;
    }
  }
  return $element;
}

/**
 * Process input elements.
 */
function _druru_process_input(&$element, &$form_state) {
  // Only add the "form-control" class for specific element input types.
  $types = array(
    // Core.
    'password',
    'password_confirm',
    'select',
    'textarea',
    'textfield',
    // Elements module.
    'emailfield',
    'numberfield',
    'rangefield',
    'searchfield',
    'telfield',
    'urlfield',
  );
  if (!empty($element['#type']) && (in_array($element['#type'], $types) || ($element['#type'] === 'file' && empty($element['#managed_file'])))) {
    $element['#attributes']['class'][] = 'form-control';
  }
  return $element;
}

/**
 * Process fieldset element.
 */
function _druru_process_fieldset(&$element, &$form_state) {
  $parents = implode('][', $element['#parents']);

  // Each fieldset forms a new group. The #type 'vertical_tabs' basically only
  // injects a new fieldset.
  $form_state['groups'][$parents]['#group_exists'] = TRUE;
  $element['#groups'] = &$form_state['groups'];

  // Process vertical tabs group member fieldsets.
  if (isset($element['#group'])) {
    // Add this fieldset to the defined group (by reference).
    $element['#theme_wrappers'] = array('bootstrap_panel');
    $group = $element['#group'];
    $form_state['groups'][$group][] = &$element;
  }

  // Contains form element summary functionalities.
  $element['#attached']['library'][] = array('system', 'drupal.form');

  // The .form-wrapper class is required for #states to treat fieldsets like
  // containers.
  if (!isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = array();
  }

  return $element;
}

/**
 * Process checkboxes element.
 */
function _druru_process_checkboxes(&$element, &$form_state) {
  $thubnails = FALSE;
  $label_attributes = isset($element['#label_attributes']);

  if (isset($element['#label_attributes']) || $thubnails) {
    foreach (element_children($element) as $key) {
      if ($label_attributes) {
        $element[$key]['#label_attributes'] = $element['#label_attributes'];
      }
    }
  }
  return $element;
}

/**
 * Process format element.
 */
function _druru_process_format($element) {
  $value = $element['value'];
  $weight = isset($value['#weight']) ? $value['#weight'] : NULL;
  if (!isset($weight) && isset($element['format']['#weight'])) {
    $weight = $element['format']['#weight'];
  }
  if (!isset($weight) && isset($element['format']['format']['#weight'])) {
    $weight = $element['format']['format']['#weight'];
  }
  if (!isset($weight) && isset($element['format']['format']['#weight'])) {
    $weight = $element['format']['format']['#weight'];
  }
  $element['format']['#title'] = t('Filter info');
  $element['format']['#collapsible'] = TRUE;
  $element['format']['#collapsed'] = TRUE;
  $element['format_box'] = $element['format'];
  unset($element['format']);

  $element['format'] = $element['format_box']['format'];
  unset($element['format_box']['format']);

  $element['format']['#title_display'] = 'invisible';

  // Set weights.
  $element['value']['#weight'] = $weight++;
  $element['format']['#weight'] = $weight++;
  $element['format_box']['#weight'] = $weight++;
  return $element;
}
