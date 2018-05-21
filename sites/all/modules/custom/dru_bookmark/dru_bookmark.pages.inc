<?php

/**
 * @file
 */

/**
 *
 */
function dru_bookmarks_page() {
  global $user;

  altpager_items_elements(array(
    60 => 60,
    100 => 100,
    200 => 200,
    400 => 400,
    600 => 600,
    800 => 800,
    1000 => 1000,
  ));

  $nids = db_select('dru_bookmarks', 'b')
    ->extend('AltPager')
    ->addTag('alttracker_nodes_condition')
    ->fields('b', array('nid'))
    ->condition('b.uid', $user->uid)
    ->execute()
    ->fetchCol();

  $max = variable_get('alttracker_max_count', 1000);
  $current = altpager_count_all_items();

  if ($current > $max) {
    altpager_count_all_items($max);
  }

  $page['first-pager'] = array(
    '#theme' => 'altpager',
    '#weight' => -10,
  );

  $nodes = node_load_multiple($nids);
  foreach ($nodes as $nid => $node) {
    $history = _forum_user_last_visit($node->nid);
    $node->new_replies = comment_num_new($node->nid, $history);
    $node->new = $node->new_replies || (isset($node->last_comment_timestamp) && $node->last_comment_timestamp > $history);
    if ($node->new) {
      $node->url = url("node/$node->nid", array('query' => comment_new_page_count($node->comment_count, $node->new_replies, $node), 'fragment' => 'new'));
    }
    else {
      $node->url = url("node/$node->nid");
    }
    $nodes[$nid] = $node;
  }

  $page['alttracker'] = array(
    '#theme' => 'alttracker',
    '#nodes' => $nodes,
  );

  $page['last-pager'] = array(
    '#theme' => 'altpager',
    '#weight' => 10,
  );

  $page['#sorted'] = TRUE;

  return $page;
}
