<?php

/**
 * Callback dru_tnx_node
 */
function dru_tnx_node($tnx, $ajax, $node, $token) {
  
  if (!drupal_valid_token($token, 'dru_tnx')) {
    return;
  }
  
  global $user;
  
  $vars = array(
    'type'   => 'node',
    'id'     => $node->nid,
    'entity' => $node,
    'tnx'    => $tnx,
    'user'   => $user,
  );
  
  return dru_tnx_callback($ajax, $vars);
  
}

/**
 * Callback dru_tnx_comment
 */
function dru_tnx_comment($tnx, $ajax, $comment, $token) {
  
  if (!drupal_valid_token($token, 'dru_tnx')) {
    return;
  }
  
  global $user;
  
  $vars = array(
    'type'   => 'comment',
    'id'     => $comment->cid,
    'entity' => $comment,
    'tnx'    => $tnx,
    'user'   => $user,
  );
  
  return dru_tnx_callback($ajax, $vars);
  
}

/**
 * dru_tnx_callback
 */
function dru_tnx_callback($ajax, $vars) {
  
  dru_tnx_data($vars);
  
  $is_ajax = $ajax === 'ajax';
  
  if ($is_ajax) {
    
    $tnx = get_user_tnx($vars['type'], $vars['id'], $vars['user']->uid);
    $tnx ? $new = -1 : $new = 1;
    $tnx_link = dru_tnx_link($vars['entity'], $vars['type'], $vars['id'], $tnx, $new);
    
    $link = l($tnx_link['title'], $tnx_link['href'], array('attributes' => $tnx_link['attributes']));
    
    $commands = array();
    
    $counter = '<div class="tnx-counter dru-tnx-' . $vars['type'] . '-' . $vars['id'] . '-counter">' . t('@count Thank you', array('@count' =>$tnx + $new)) . '</div>';
    
    $commands[] = ajax_command_replace('.dru-tnx-' . $vars['type'] . '-' .$vars['id'] . '-counter', $counter);

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
  
  // Counting tnx by entity
  $tnx_count = $vars['entity']->tnx;
  if ($vars['tnx'] == 'tnx') {
    $tnx_count++;
  
    // Add tnx
    db_insert('tnx')
      ->fields(array(
        'entity_id'   => $vars['id'],
        'entity_type' => $vars['type'],
        'uid'         => $vars['user']->uid,
      ))
      ->execute();
      
    // Count tnx
    db_merge('tnx_stat')
      ->key(array('uid' => $vars['entity']->uid))
      ->fields(array($vars['type'] => 1))
      ->expression($vars['type'], $vars['type'] . ' + :inc', array(':inc' => 1))
      ->execute();
      
    // Count user tnx
    db_merge('tnx_stat')
      ->key(array('uid' => $vars['user']->uid))
      ->fields(array('tnx_' . $vars['type'] => 1))
      ->expression('tnx_' . $vars['type'], 'tnx_' . $vars['type'] . ' + :inc', array(':inc' => 1))
      ->execute();
  }
  else {
    $tnx_count--;
  
    // Delete tnx
    db_delete('tnx')
      ->condition('entity_id', $vars['id'])
      ->condition('entity_type', $vars['type'])
      ->condition('uid', $vars['user']->uid)
      ->execute();
      
    // Count tnx
    db_merge('tnx_stat')
      ->key(array('uid' => $vars['entity']->uid))
      ->fields(array($vars['type'] => 1))
      ->expression($vars['type'], $vars['type'] . ' - :inc', array(':inc' => 1))
      ->execute();
      
    // Count user tnx
    db_merge('tnx_stat')
      ->key(array('uid' => $vars['user']->uid))
      ->fields(array('tnx_' . $vars['type'] => 1))
      ->expression('tnx_' . $vars['type'], 'tnx_' . $vars['type'] . ' - :inc', array(':inc' => 1))
      ->execute();
  }
  
  // Write tnx count to entity
  $query = db_update($vars['type']);
  $query->condition($vars['type'] == 'node' ? 'nid' : 'cid', $vars['id'])
        ->fields(array('tnx' => $tnx_count))
        ->execute();
}
