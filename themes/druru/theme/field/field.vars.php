<?php

/**
 * Implements hook_preprocess_field().
 */
function druru_preprocess_field(&$variables) {
  $elt   = &$variables['element'];
  $label = &$variables['label'];
  // wrap label to h3 tag if label has "above" position
  $is_above = isset($elt['#label_display']) && $elt['#label_display'] == 'above';
  $label    = $is_above ? '<h3>' . $label . '</h3>' : $label . ':';
}

