<?php
/**
 * Implementation of theme_select
 */
function druru_select($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));

  // add classes
  $classes   = @$element['#attributes']['class'] ?: array();
  $classes[] = 'form-select';
  if (!in_array('no-picker', $element['#attributes']['class'])) {
    $classes[] = 'selectpicker';
  }
  _form_set_class($element, $classes);

  // render
  return '<select' . drupal_attributes($element['#attributes']) . '>' . form_select_options($element) . '</select>';
}