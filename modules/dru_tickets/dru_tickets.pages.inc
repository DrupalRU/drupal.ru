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
  
  $ticket = db_select('ticket', 't')
    ->fields('t')
    ->condition('ticket_id', $node->nid)
    ->execute()
    ->fetchObject();
        
  if ($node->ticket->moderate_uid == 0) {
    $node->ticket->moderate_uid = $user->uid;
    node_save($node);
    drupal_set_message(t('Ticket accepted')); 
  }
  elseif ($node->ticket->moderate_uid == $user->uid) {
    drupal_set_message(t('Ticket already your'));
  }
  else {
    $moderator = user_load($node->ticket->moderate_uid);
    drupal_set_message(t("Ticket moderated by @name", array('@name' => format_username($moderator))), 'warning');
  }
  
  drupal_goto('node/' . $node->nid);
}

/**
 * Verdict
 */
function dru_ticket_verdict($ajax, $node, $token) {
  
  if (!drupal_valid_token($token, 'dru_tickets')) {
    return;
  }
  
  $is_ajax = $ajax === 'ajax';
  $vars = array(
    'ticket' => $node,
  );
  
  if ($is_ajax) {
    $form = drupal_get_form('dru_verdict_form', $vars);
    
    $commands = array();
    
    $render_form  = '<div id="claim-add-form">';
    $render_form .= '<h2>';
    $render_form .= t('Add Verdict');
    $render_form .= '</h2>';
    $render_form .= render($form);
    $render_form .= '</div>';
    
    $commands[] = ajax_command_append('body', '<div id="claim-add">' . $render_form . '</div>');

    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }
  else {
    return drupal_get_form('dru_verdict_form', $vars);
  }
}

/**
 * dru_verdict_form
 */
function dru_verdict_form($form, &$form_state, $vars) {
  
  $claim = db_select('ticket_claim', 't')
    ->fields('t', array('ticket_verdict'))
    ->condition('t.ticket_id', $vars['ticket']->nid, '=')
    ->execute()
    ->fetchObject();
    
  if (empty($claim->ticket_verdict)) {
    $verdict = '';
  }
  else {
    $verdict = $claim->ticket_verdict;
  }
  
  $form['dru_verdict'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Claim verdict'),
    '#rows'          => 2,
    '#default_value' => $verdict,
  );
  
  $form['dru_verdict_submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
    '#ajax'  => array(
      'callback' => 'dru_verdict_submit_js',
    ),
  );
  
  $form_state['dru_ticket'] = $vars;
  
  return $form;
}

/**
 * Callback dru_verdict_form_validate
 */
function dru_verdict_form_validate($form, &$form_state) {
  
}

/**
 * Callback dru_verdict_submit_js
 */
function dru_verdict_submit_js($form_id, $form_state) {
    
  $nid = $form_state['dru_ticket']['ticket']->nid;
  $commands = array();
  
  $commands[] = ajax_command_replace('.clame-verdict-' . $nid, '<div class="field-items clame-verdict-value clame-verdict-' . $nid . '">' . $form_state['values']['dru_verdict'] . '</div>');
  
  $commands[] = ajax_command_invoke('.clame-verdict-' . $form_state['dru_ticket']['ticket']->nid, 'addClass', array('tiket-changed'));
  
  $commands[] = ajax_command_remove('#claim-add');
  
  return array(
    '#type'     => 'ajax',
    '#commands' => $commands,
  );
  
}

/**
 * Callback dru_verdict_form_submit
 */
function dru_verdict_form_submit($form_id, &$form_state) {
  
  $verdict = $form_state['values']['dru_verdict'];
  $ticket  = $form_state['dru_ticket']['ticket'];
  
  if (!empty($verdict)) {
    $statuses = taxonomy_get_term_by_name(t('Closed'));
    foreach ($statuses as $tid => $status) {
      if ($status->vocabulary_machine_name == 'ticket_status') {
        $ticket->field_ticket_status[$ticket->language][0]['tid'] = $status->tid;
      }
    }
    node_save($ticket);
  }
  else {
    $statuses = taxonomy_get_term_by_name(t('Open'));
    foreach ($statuses as $tid => $status) {
      if ($status->vocabulary_machine_name == 'ticket_status') {
        $ticket->field_ticket_status[$ticket->language][0]['tid'] = $tid;
      }
    }
    node_save($ticket);
  }
  
  $query = db_update('ticket_claim');
  $query->fields(array(
    'ticket_verdict' => $verdict,
    ))
    ->condition('ticket_id', $ticket->nid)
    ->execute();
  
}
