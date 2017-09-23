<?php

/**
 * Overrides theme_resolved().
 *
 * For correct working of the override, need to allow tag "i" in title of page.
 * We will do it in preprocess_page().
 *
 * @param $variables
 *
 * @return string
 * @see druru_preprocess_page().
 * @see resolve_preprocess_page().
 */
function druru_resolved($variables) {
  $mark = '';
  if (function_exists('drurum_node_icon')) {
    if ($variables['resolved'] == RESOLVE_IS_RESOLVED) {
      $mark = drurum_node_icon($variables['context']);
    }
  }
  // fallback.
  else {
    $mark = theme_resolved($variables);
  }
  return $mark;
}
