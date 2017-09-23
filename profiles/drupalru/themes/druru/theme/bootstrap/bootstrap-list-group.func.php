<?php

/**
 * @file
 * bootstrap-lsit-group.func.php
 */

/**
 * Implements theme_bootstrap_list_group().
 */
function druru_bootstrap_list_group($variables) {
  $items = $variables['items'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];
  $attributes['class'][] = 'list-group';
  $links = $type == 'links';
  $tag = $links ? 'div' : 'ul';
  $output = '<' . $tag . drupal_attributes($attributes) . '>';
  if ($links) {
    $output .= _druru_bootstrap_list_group_links($items);
  }
  else {
    $output .= _druru_bootstrap_list_group_list($items);
  }
  $output .= '</' . $tag . '>';
  return $output;
}

/**
 * Makes list group from links (tag <a>).
 *
 * @param $items
 *
 * @return string
 */
function _druru_bootstrap_list_group_links($items) {
  $output = '';
  if (!empty($items)) {
    $num_items = count($items);
    $i = 0;
    foreach ($items as $item) {
      $i++;
      if(isset($item['options'])) {
        $options = $item['options'];
      }
      elseif(isset($item['#options'])) {
        $options = &$item['#options'];
      }
      else {
        $options = array();
      }

      $options['attributes']['class'][] = 'list-group-item';
      $options['attributes']['class'][] = 'clearfix';
      $options['attributes']['class'][] = $i == 1 ? 'first' : ($i == $num_items ? 'last' : '');

      if (isset($item['#theme'])) {
        $data = render($item);
      }
      else {
        $data = l($item['title'], $item['path'], $options);
      }

      $output .= $data;
    }
  }
  return $output;
}

function _druru_bootstrap_list_group_list($items) {
  $output = '';
  if (!empty($items)) {
    $num_items = count($items);
    $i = 0;
    foreach ($items as $item) {
      $attributes = array();
      $attributes['class'][] = 'list-group-item';
      $attributes['class'][] = 'clearfix';
      $attributes['class'][] = $i == 1 ? 'first' : ($i == $num_items ? 'last' : '');
      $data = '';
      $i++;

      $output .= '<li' . drupal_attributes($attributes) . '>' . $data . "</li>";
    }
  }
  return $output;
}
