<?php

/**
 * Implements hook_process_hook().
 *
 * @param $variables
 */
function druru_process_tableselect(&$variables) {
  $variables['#attributes']['class'][] = 'tableselect';
  $variables['element']['#attributes']['class'][] = 'table-hover';

  foreach (element_children($variables['element']) as $key) {
    $variables['element'][$key]['#title_display'] = 'after';
  }
}
