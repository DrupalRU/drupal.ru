<?php

function dru_tickets_page() {
  
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'node')
                  ->propertyCondition('status', 1)
                  ->propertyCondition('type', 'ticket')
                  ->propertyOrderBy('nid', 'DESC')
                  ->pager(10)
                  ->execute();
                  
  if (!empty($result)) {
    $content = node_view_multiple(node_load_multiple(array_keys($result['node'])));
    $page    = array(
      'content' => $content,
      'pager'   => array(
        '#markup' => theme('pager'),
        '#weight' => 10,
      )
    );
  }
  else {
    $page = t('There are no tickets');
  }
  
  return $page;
  
}

function dru_ticket_accept($node, $token) {
  
  if (!drupal_valid_token($token, 'dru_tickets')) {
    return;
  }
  
  global $user;
  
  $claim = db_select('ticket_claim', 't')
        ->fields('t')
        ->condition('t.content_id', $node->nid, '=')
        ->execute()
        ->fetchObject();
        
  if ($claim->moderate_uid == 0) {
    $claim->moderate_uid = $user->uid;
    drupal_write_record('ticket_claim', $claim, array('id'));
    drupal_set_message(t('Ticket accepted')); 
  }
  elseif ($claim->moderate_uid == $user->uid) {
    drupal_set_message(t('Ticket already your'));
  }
  else {
    $moderator = user_load($claim->moderate_uid);
    drupal_set_message(t("Ticket moderated by @name", array('@name' => format_username($moderator))), 'warning');
  }
  
  drupal_goto('node/' . $node->nid);
}
