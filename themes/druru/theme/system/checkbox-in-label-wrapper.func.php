<?php

function druru_checkbox_in_label_wrapper($variables) {
  $element = &$variables['element'];
  $attributes = isset($element['#attributes']) ? $element['#attributes'] : array();
  if(isset($element['#label_attributes'])) {
    $attributes = array_merge($attributes, $element['#label_attributes']);
  }

  // Add element #id for #type 'item'.
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }


  // Check for errors and set correct error class.
  if (isset($element['#parents']) && form_get_error($element)) {
    $attributes['class'][] = 'error';
  }

  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(
          ' ' => '-',
          '_' => '-',
          '[' => '-',
          ']' => '',
        ));
  }
  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }

  $output = '<label' . drupal_attributes($attributes) . '>' . "\n";

  $prefix = '';
  $suffix = '';
  if (isset($element['#field_prefix']) || isset($element['#field_suffix'])) {
    // Determine if "#input_group" was specified.
    if (!empty($element['#input_group'])) {
      $prefix .= '<div class="input-group">';
      $prefix .= isset($element['#field_prefix']) ? '<span class="input-group-addon">'
        . $element['#field_prefix']
        . '</span>' : '';
      $suffix .= isset($element['#field_suffix']) ? '<span class="input-group-addon">'
        . $element['#field_suffix']
        . '</span>' : '';
      $suffix .= '</div>';
    }
    else {
      $prefix .= isset($element['#field_prefix']) ? $element['#field_prefix'] : '';
      $suffix .= isset($element['#field_suffix']) ? $element['#field_suffix'] : '';
    }
  }

  switch ($element['#title_display']) {
    case 'before':
    case 'invisible':
      $output .= $element['#title'];
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;

    case 'after':
      $output .= ' ' . $prefix . $element['#children'] . $suffix;
      $output .= $element['#title'];
      break;

  }
  $output .= "</label>\n";

  return $output;
}
