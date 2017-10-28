<?php

/**
 * @file
 * User page callbacks for tracker.module.
 */

function alttracker_page_my(){
  global $user;
  return alttracker_page($user);
}

/**
 * Page callback: prints a listing of active nodes on the site.
 *
 * Queries the database for info, adds RDFa info if applicable, and generates
 * the render array that will be used to render the page.
 */
function alttracker_page($account = NULL, $set_title = FALSE) {
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

  if ($account) {
    $query = db_select('alttracker_user', 't')->extend('AltPager');
    $query->condition('t.uid', $account->uid);

    if ($set_title) {
      // When viewed from user/%user/track, display the name of the user
      // as page title -- the tab title remains Track so this needs to be done
      // here and not in the menu definition.
      drupal_set_title(format_username($account));
    }
  }
  else {
    $query = db_select('alttracker_node', 't', array('target' => 'slave'))->extend('AltPager');
  }

  if($types = variable_get('alttracker_node_type')){
    $in_types = array();

    foreach($types as $type => $val){
      if($val){
        $in_types[] = $type;
      }
    }
    if(!empty($in_types)){
      $query->leftJoin('node', 'n', 'n.nid = t.nid');
      $query->condition('n.type', $in_types, 'IN');
    }
  }
  $query
    ->addTag('alttracker_nodes_condition')
    ->fields('t', array('nid', 'changed'))
    ->condition('t.published', 1)
    ->orderBy('t.changed', 'DESC');

    $hide_dark_matter = (isset($user->darkmatter) ? $user->darkmatter == 1 : false);
    if (!$hide_dark_matter) {
      $query->condition('n.darkmatter', 0);
    }

  if($ignore_terms = variable_get('alttracker_ignore_terms')){
    $query->condition('n.nid', db_select('taxonomy_index', 't')
      -> fields('t', array('nid'))
      -> condition('tid', $ignore_terms , 'IN'), 'NOT IN');
  }
  $nids = $query->execute()->fetchCol();
  $max = variable_get('alttracker_max_count', 1000);
  $current = altpager_count_all_items();

  if($current > $max){
    altpager_count_all_items($max);
  }

  $page['first-pager'] = array(
    '#theme' => 'altpager',
    '#weight' => -10,
  );

  $nodes = array();
  if($nids){
	$nodes = alttracker_nodes_load($nids);

  foreach($nodes as $nid => $node){
    $history = _forum_user_last_visit($node->nid);
    $node->new_replies = comment_num_new($node->nid, $history);
    $node->new = $node->new_replies || (isset($node->last_comment_timestamp) && $node->last_comment_timestamp > $history);
    if($node->new){
      $node->url = url("node/$node->nid", array('query' => comment_new_page_count($node->comment_count, $node->new_replies, $node), 'fragment' => 'new'));
    }else{
      $node->url = url("node/$node->nid");
    }

    if(variable_get('alttracker_taxonomy')){
      $tids = db_select('taxonomy_index', 't', array('target' => 'slave'))
        ->fields('t', array('tid'))
        ->condition('t.nid', $node->nid)
        ->execute()
        ->fetchCol();
      $node->terms = taxonomy_term_load_multiple($tids, array());
    }
    $nodes[$nid] = $node;
  }
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

function alttracker_nodes_load($nids) {
  $query = db_select('node', 'n', array('target' => 'slave'));
  $query->addTag('alttracker_nodes_load');
  $query->join('alttracker_node', 't', 't.nid = n.nid');
  $query->join('node_comment_statistics', 'l', 'l.nid = n.nid');
  $query->join('users', 'u', 'n.uid = u.uid');
  $query->condition('n.nid', $nids, 'IN')
   ->fields('n')
   ->fields('t', array('changed'))
   ->fields('l', array('comment_count'))
   ->fields('u', array('name'))
   ->orderBy('t.changed', 'DESC');

  $result = $query->execute();
  return $result->fetchAll();
}
