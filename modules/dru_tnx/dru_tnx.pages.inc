<?php

/**
 * Callback dru_tnx_node
 */
function dru_tnx_node($tnx, $ajax, $node) {
  $vars = array(
    'type'   => 'node',
    'id'     => $node->nid,
    'entity' => $node,
    'tnx'    => $tnx,
  );
  
  return dru_tnx_callback($ajax, $vars);
  
}

/**
 * Callback dru_tnx_comment
 */
function dru_tnx_comment($tnx, $ajax, $comment) {
  $vars = array(
    'type'   => 'comment',
    'id'     => $comment->cid,
    'entity' => $comment,
    'tnx'    => $tnx,
  );
  
  return dru_tnx_callback($ajax, $vars);
  
}

/**
 * dru_tnx_callback
 */
function dru_tnx_callback($ajax, $vars) {
  
  dru_tnx_data($vars);
  
  global $user;
  
  $is_ajax = $ajax === 'ajax';
  
  if ($is_ajax) {
    
    $tnx = get_user_tnx($vars['type'], $vars['id'], $user->uid);
    $tnx_link = dru_tnx_link($vars['entity'], $vars['type'], $vars['id'], $tnx);
    
    $link = l($tnx_link['title'], $tnx_link['href'], array('attributes' => $tnx_link['attributes']));
    
    $commands = array();
    
    $commands[] = ajax_command_replace('.dru-tnx-' . $vars['type'] . '-' .$vars['id'], $link);

    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }
  else {
    $message = $vars['tnx'] == 'tnx' ? t('Your Thank Accepted') : t('Your Thank Deleted');
    drupal_set_message($message);
    drupal_goto();
  }
}

/**
 * Callback dru_tnx_data
 */
function dru_tnx_data($vars) {
  
  global $user;
  
  // Update entity
  switch ($vars['type']) {
    case 'node':
        $id = 'nid';
        $entity = node_load($vars['id']);
        break;
    case 'comment':
        $id = 'cid';
        $entity = comment_load($vars['id']);
        break;
  }
  
  $tnx_count = $entity->tnx;
  if ($vars['tnx'] == 'tnx') {
    $tnx_count++;
  
    // Create tnx
    db_insert('tnx')
      ->fields(array(
        'entity_id'   => $vars['id'],
        'entity_type' => $vars['type'],
        'uid'         => $user->uid,
      ))
      ->execute();
  }
  else {
    $tnx_count--;
  
    // Delete tnx
    db_delete('tnx')
      ->condition('entity_id', $vars['id'])
      ->condition('entity_type', $vars['type'])
      ->condition('uid', $user->uid)
      ->execute();
  }
  
  if (isset($id)) {
    $query = db_update($vars['type']);
    $query->condition($id, $vars['id'])
          ->fields(array('tnx' => $tnx_count))
          ->execute();
  }
}
