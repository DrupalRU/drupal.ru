<?php

/**
 * Theme function to theme the blocked user listing.
 */
function druru_pm_block_user_list($variables) {
  $form = drupal_render_children($variables['form']);
  $table = theme('table', array(
    'header' => $variables['form']['#header'],
    'rows'   => $variables['form']['#rows'],
  ));
  $pager = theme('pager');
  return '<div class="row">' . $form . ' </div>' . $table . $pager;
}
