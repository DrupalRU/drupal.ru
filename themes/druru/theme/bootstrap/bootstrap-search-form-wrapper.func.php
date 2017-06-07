<?php
/**
 * @file
 * bootstrap-search-form-wrapper.func.php
 */

/**
 * Theme function implementation for bootstrap_search_form_wrapper.
 */
function druru_bootstrap_search_form_wrapper($variables) {
  $output = '<div class="input-group">';
  $output .= $variables['element']['#children'];
//  $output .= '<span class="input-group-btn">';
  $output .= '<button type="submit" class="btn btn-primary">';
  // We can be sure that the font icons exist in CDN.
  if (theme_get_setting('druru_iconize')) {
    $output .= druru_icon('search');
  }
  else {
    $output .= t('Search');
  }
  $output .= '</button>';
//  $output .= '</span>';
  $output .= theme('form_element_label', $variables['element']);
  $output .= '</div>';
  return $output;
}
