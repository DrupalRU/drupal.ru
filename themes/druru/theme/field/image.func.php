<?php

function druru_image_widget($variables) {
  $element = $variables['element'];
  $output = '';
  $output .= '<div class="image-widget form-managed-file input-group clearfix">';

  if (isset($element['preview'])) {
    $output .= '<div class="image-preview">';
    $output .= drupal_render($element['preview']);
    $output .= '</div>';
  }

  if ($element['fid']['#value'] != 0) {
    $element['filename']['#markup'] .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> ';
  }
  $element['upload_button']['#prefix'] = '<span class="input-group-btn">';
  $element['upload_button']['#suffix'] = '</span>';

  $output .= drupal_render_children($element);
  $output .= '</div>';

  return $output;
}
