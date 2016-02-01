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
  if ($node->uid == 0) {
    $node->uid = $user->uid;
    node_save($node);
    drupal_set_message(t('Ticket accepted')); 
  }
  elseif ($node->uid == $user->uid) {
    drupal_set_message(t('Ticket already your'));
  }
  else {
    $moderator = user_load($node->uid);
    drupal_set_message(t("Ticket moderated by @name", array('@name' => format_username($moderator))), 'warning');
  }
  
  drupal_goto('node/' . $node->nid);
}
