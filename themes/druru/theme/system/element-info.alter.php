<?php

/**
 * Implements hook_element_info_alter().
 */
function druru_element_info_alter(&$elements) {
  foreach ($elements as &$element) {
    // Process all elements.
    $element['#process'][] = '_druru_process_element';
    // Process input elements.
    if (!empty($element['#input'])) {
      $element['#process'][] = '_druru_process_input';
    }
    // Process input elements.
    if (!empty($element['#type']) && $element['#type'] === 'checkboxes') {
      $element['#process'][] = '_druru_process_checkboxes';
    }
    // Process core's fieldset element.
    if (!empty($element['#type']) && $element['#type'] === 'fieldset') {
      $element['#theme_wrappers'] = array('bootstrap_panel');
    }
    if (!empty($element['#theme']) && $element['#theme'] === 'fieldset') {
      $element['#theme'] = 'bootstrap_panel';
    }
    // Replace #process function.
    if (!empty($element['#process']) && ($key = array_search('form_process_fieldset', $element['#process'])) !== FALSE) {
      $element['#process'][$key] = '_druru_process_fieldset';
    }
    // Replace #pre_render function.
    if (!empty($element['#pre_render']) && ($key = array_search('form_pre_render_fieldset', $element['#pre_render'])) !== FALSE) {
      $element['#pre_render'][$key] = '_druru_pre_render_fieldset';
    }
    // Replace #theme_wrappers function.
    if (!empty($element['#theme_wrappers']) && ($key = array_search('fieldset', $element['#theme_wrappers'])) !== FALSE) {
      $element['#theme_wrappers'][$key] = 'bootstrap_panel';
    }
  }
  $elements['text_format']['#process'][] = '_druru_process_format';
}
