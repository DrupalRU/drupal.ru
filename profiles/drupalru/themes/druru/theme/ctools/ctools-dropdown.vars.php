<?php

/**
 * Implements hook_preprocess_HOOK() for ctools_dropdown().
 */
function druru_preprocess_ctools_dropdown(&$vars) {
  $vars['image'] = TRUE;
  $vars['title'] = '<span class="icon-bar"></span>';
  $vars['title'] .= '<span class="icon-bar"></span>';
  $vars['title'] .= '<span class="icon-bar"></span>';
}
