<?php

/**
 * Implements hook_preprocess_field().
 * @todo Refactor to use css classes
 */
function druru_preprocess_field(&$vars) {
  $element = &$vars['element'];
  $label   = &$vars['label'];
  // wrap label to h3 tag if label has "above" position
  // @todo Refactor to use css for styling
  $is_above = isset($element['#label_display']) && $element['#label_display'] == 'above';
  $label    = $is_above ? '<h>' . $label . '</h3>' : $label. ':';

  // Assign class name to body of nodes and comments
  if (in_array($element['#field_name'], ['body', 'comment_body'])) {
    $vars['classes_array'] = [];
    $vars['classes_array'][] = $element['#entity_type'] . '__body';
  }

  // Set single tpl for group of taxonomy fields
  if (in_array($element['#field_name'], ['taxonomy_vocabulary_7', 'taxonomy_vocabulary_8', 'taxonomy_vocabulary_10', 'taxonomy_forums'])) {
    $vars['theme_hook_suggestions'][] = 'field__group_taxonomy';
    $vars['theme_hook_suggestions'][] = 'field__group_taxonomy__' . $element['#entity_type'];
    $vars['theme_hook_suggestions'][] = 'field__group_taxonomy__' . $element['#entity_type'] . '__' . $element['#view_mode'];
  }
}
