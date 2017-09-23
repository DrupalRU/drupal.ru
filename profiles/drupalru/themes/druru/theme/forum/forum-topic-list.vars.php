<?php

function druru_preprocess_forum_topic_list(&$variables) {
  _druru_get_forum_sort_swicher($variables);
  foreach ($variables['topics'] as $key => $topic) {
    $variables['topics'][$key]->time = format_interval(REQUEST_TIME
      - $topic->last_comment_timestamp, 1);
    if (function_exists('drurum_node_icon')) {
      $variables['topics'][$key]->icon = drurum_node_icon($variables['topics'][$key]);
    }
  }
}

function _druru_forum_sort_get_dir($asc = TRUE) {
  return $asc ? 'asc' : 'desc';
}

function _druru_check_directions(&$headers, $current_order) {
  $default_dir = 'asc';
  $curr_field = $current_order['sql'];
  $curr_dir = $current_order['sort'];
  foreach ($headers as &$header) {
    // If header don't has no ability to order.
    if ($header) {
      // Set default sort rule if doesn't exists.
      if (!isset($header['sort'])) {
        $header['sort'] = $default_dir;
      }
      if ($curr_field == $header['field']) {
        $header['sort'] = _druru_forum_sort_get_dir($curr_dir == 'desc');
      }
    }
  }
}

function _druru_get_forum_sort_swicher(&$variables) {
  $order_items = array();
  $headers = $GLOBALS['forum_topic_list_header'];
  $current_order = tablesort_init($headers);
  _druru_check_directions($headers, $current_order);
  foreach ($headers as $header) {
    // If header don't has no ability to order.
    if ($header) {
      $order_items[] = _druru_get_forum_sort_swicher_link($header);
    }
  }
  if ($order_items) {
    $variables['sort_header'] = _druru_forum_sort_header_build($order_items, $current_order);
  }
}

function _druru_get_forum_sort_swicher_link($header) {
  return theme('link', array(
    'text'    => $header['data'],
    'path'    => current_path(),
    'options' => array(
      'attributes' => array(
        'title' => t('sort by @s', array('@s' => $header['data'])),
      ),
      'query'      => array(
        'sort'  => $header['sort'],
        'order' => $header['data'],
      ),
      'html'       => TRUE,
    ),
  ));
}

function _druru_forum_sort_header_build($items, $current_order) {
  $build = array();
  $build['sort_header'] = array(
    '#type'       => 'container',
    '#attributes' => array(
      'class' => array('btn-group', 'form-group', 'form-actions', 'clearfix'),
      'role'  => 'group',
    ),
  );

  // Make more one link near to dropdown, which provide ability to chage direction of order.
  $inverse_dir = _druru_forum_sort_get_dir($current_order['sort'] != 'asc');
  $build['sort_header']['link'] = array(
    '#type'    => 'link',
    '#suffix'  => '<span class="title">' . t('Sort') . '</span>',
    '#title'   => druru_icon('sort-amount-' . $inverse_dir),
    '#href'    => current_path(),
    '#options' => array(
      // Next block was added to exclude unneeded class 'active' from the link.
      'language'   => (object) array(
        'language' => 'fake',
      ),
      'attributes' => array(
        'title' => t('Change direction of sorting by @title', array(
          '@title' => $current_order['name'],
        )),
        'class' => array('btn', 'btn-default'),
      ),
      'query'      => array(
        'sort'  => $inverse_dir,
        'order' => $current_order['name'],
      ),
      'html'       => TRUE,
    ),
  );

  $build['sort_header']['group'] = array(
    '#type'       => 'container',
    '#attributes' => array(
      'class' => array('btn-group'),
      'role'  => 'group',
    ),
  );
  $build['sort_header']['group']['button'] = array(
    '#type'       => 'button',
    '#value'      => $current_order['name'] . ' <span class="caret"></span>',
    '#attributes' => array(
      'class'         => array('btn', 'btn-default', 'dropdown-toggle'),
      'data-toggle'   => 'dropdown',
      'aria-haspopup' => 'true',
      'aria-expanded' => 'false',
    ),
  );
  $build['sort_header']['group']['dropdown'] = array(
    '#theme'      => 'item_list',
    '#items'      => $items,
    '#attributes' => array(
      'class' => array('dropdown-menu', 'dropdown-menu-right'),
    ),
  );

  return $build;
}
