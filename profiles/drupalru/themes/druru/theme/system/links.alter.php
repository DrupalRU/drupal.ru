<?php


/**
 * Implements hook_menu_local_tasks_alter().
 * @todo Remove and test
 */
function druru_menu_local_tasks_alter(&$data, $router_item, $root_path) {
  $actions = array();
  if (isset($data['actions'])) {
    $actions = &$data['actions'];
  }

  if (isset($actions['output']) && is_array($actions['output'])) {
    foreach ($actions['output'] as &$item) {
      _druru_add_ajax_metadata_to_link($item['#link']);
    }
  }
}

/**
 * @param $link
 */
function _druru_add_ajax_metadata_to_link(&$link) {
  $callback = !empty($link['delivery_callback']) ? $link['delivery_callback']
    : NULL;
  if ('ajax_deliver' == $callback) {
    $id = drupal_html_class($link['page_callback']);
    $link['attributes']['class'][] = 'use-ajax';
    $link['attributes']['id'] = $id;
    $link['options']['attributes']['class'][] = 'use-ajax';
    $link['options']['attributes']['id'] = $id;
    $link['localized_options']['attributes']['class'][] = 'use-ajax';
    $link['localized_options']['attributes']['id'] = $id;
  }
}

/**
 * Implements hook_menu_contextual_links_alter().
 */
function druru_menu_contextual_links_alter(&$links, $router_item, $root_path) {
  foreach ($links as &$link) {
    _druru_add_ajax_metadata_to_link($link);
  }
}
