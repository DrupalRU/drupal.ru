<?php


/**
 * Implements hook_menu_local_tasks_alter().
 */
function druru_menu_local_tasks_alter(&$data, $router_item, $root_path) {
  $actions = array();
  if (isset($data['actions'])) {
    $actions = &$data['actions'];
  }

  if (isset($data['tabs'][0]['output'])) {
    foreach ($data['tabs'][0]['output'] as &$tab) {
      $link = &$tab['#link'];
      if (isset($link['path'])) {
        switch ($link['path']) {
          case 'node/%/view':
            _druru_iconize_pill($link, 'eye');
            break;
          case 'node/%/edit':
            _druru_iconize_pill($link, 'pencil');
            break;
          case 'node/%/outline':
            _druru_iconize_pill($link, 'sitemap');
            break;
          case 'node/%/revisions':
            _druru_iconize_pill($link, 'code-fork');
            break;
        }
      }
    }
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

function _druru_visible_on_phone($text) {
  return '<span class="visible-xs-inline-block">' . $text . '</span>';
}

function _druru_iconize_pill(&$link, $icon) {
  $link['localized_options']['attributes']['title'] = $link['title'];
  $link['title'] = druru_icon($icon) . _druru_visible_on_phone($link['title']);
  $link['html'] = TRUE;
  $link['localized_options']['html'] = TRUE;
}
