<?php

/**
 * Callback dru_claim_node
 */
function dru_claim_node($ajax, $node, $token) {
  
  if (!drupal_valid_token($token, 'dru_claim')) {
    return;
  }
  
  $vars = array(
    'type'   => 'node',
    'id'     => $node->nid,
    'entity' => $node,
  );
  
  return dru_claim_callback($ajax, $vars);
  
}

/**
 * Callback dru_claim_comment
 */
function dru_claim_comment($ajax, $comment, $token) {
  
  if (!drupal_valid_token($token, 'dru_claim')) {
    return;
  }
  
  $vars = array(
    'type'   => 'comment',
    'id'     => $comment->cid,
    'entity' => $comment,
  );
  
  return dru_claim_callback($ajax, $vars);
  
}

/**
 * dru_claim_callback
 */
function dru_claim_callback($ajax, $vars) {
  $is_ajax = $ajax === 'ajax';
  
  if ($is_ajax) {
    $form = drupal_get_form('dru_claim_form', $vars);
    
    $commands = array();
    
    $render_form  = '<div id="claim-add-form">';
    $render_form .= '<h2>';
    $render_form .= t('Add claim');
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
    return drupal_get_form('dru_claim_form', $vars);
  }
}

/**
 * dru_claim_form
 */
function dru_claim_form($form, &$form_state, $vars) {
  
  $options = array();
  $voc     = taxonomy_vocabulary_machine_name_load('claim_category');
  $terms   = taxonomy_get_tree($voc->vid);
  foreach ($terms as $term) {
    $options[$term->tid] = $term->name;
  }
  
  $form['dru_claim_category'] = array(
    '#type'        => 'radios',
    '#title'       => t('Select a category claim'),
    '#description' => t('Select a category claim'),
    '#options'     => $options,
    '#required'    => TRUE,
  );
  
  $form['dru_claim_description'] = array(
    '#type'        => 'textarea',
    '#title'       => t('Claim description'),
    '#description' => t('Claim description'),
    '#rows'        => 2,
  );
  
  $form['dru_claim_submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Send'),
    '#ajax'  => array(
      'callback' => 'dru_claim_submit_js',
      'method'   => 'remove',
      'effect'   => 'fade',
    ),
  );
  
  $form_state['dru_claim_entity'] = $vars;
  
  return $form;
}

/**
 * Callback dru_claim_form_validate
 */
function dru_claim_form_validate($form, &$form_state) {
  
}

/**
 * Callback dru_claim_submit_js
 */
function dru_claim_submit_js($form_id, $form_state) {
    
  $entity = $form_state['dru_claim_entity'];
  switch ($entity['type']) {
    case 'node':
        $body = 'field-name-body';
        break;
    case 'comment':
        $body = 'field-name-comment-body';
        break;
  }
  
  if (isset($body)) {
    $verdict  = '<div class="claim_message claim_message_processed">';
    $verdict .= '<i class="fa fa-cogs"></i>';
    $verdict .= t('Claim is under review');
    $verdict .= '</div>';
    
    $commands = array();
    $commands[] = ajax_command_remove('.claim-' . $entity['type'] . '-' . $entity['id']);
    $commands[] = ajax_command_before('.body-' . $entity['id'] . ' .field-item', $verdict);
    $commands[] = ajax_command_remove('#claim-add');
  
    return array(
      '#type'     => 'ajax',
      '#commands' => $commands,
    );
  }
  
}

/**
 * Callback dru_claim_form_submit
 */
function dru_claim_form_submit($form_id, &$form_state) {
  
  $entity = $form_state['dru_claim_entity']['entity'];
  global $user;
  
  // Create ticket
  $ticket = new stdClass();
  $ticket->type = 'ticket';
  node_object_prepare($ticket);
  $ticket->title = t('Claim !type=!id', array('!type' => $form_state['dru_claim_entity']['type'], '!id' => $form_state['dru_claim_entity']['id']));
  $ticket->language = LANGUAGE_NONE;
  $ticket->body[$ticket->language][0]['value'] = $form_state['values']['dru_claim_description'];
  $ticket->field_claim_category[$ticket->language][0]['tid'] = $form_state['values']['dru_claim_category'];
  
  $terms = taxonomy_get_term_by_name(t('Open'));
  $term = reset($terms);
  $ticket->field_ticket_status[$ticket->language][0]['tid'] = $term->tid;
  
  $ticket->uid = $user->uid;
  node_save($ticket);
  
  // Create claim
  db_insert('ticket_claim')
    ->fields(array(
      'ticket_id'    => $ticket->nid,
      'content_type' => $form_state['dru_claim_entity']['type'],
      'content_id'   => $form_state['dru_claim_entity']['id'],
      'init_uid'     => $user->uid,
    ))
    ->execute();
  
  // Update entity
  switch ($form_state['dru_claim_entity']['type']) {
    case 'node':
        $id_type = 'nid';
        break;
    case 'comment':
        $id_type = 'cid';
        break;
  }
  if (isset($id_type)) {
    $query = db_update($form_state['dru_claim_entity']['type']);
    $query->fields(array(
      'claim' => $ticket->nid,
    ));
    $query->condition($id_type, $form_state['dru_claim_entity']['id'], '=');
    $num_updated = $query->execute();
  }
}

