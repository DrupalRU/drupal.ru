<?php

/**
 * @file
 */

/**
 * Callback dru_claim_node.
 */
function dru_claim_node($ajax, $node, $token) {
  return !drupal_valid_token($token, 'dru_claim') ?: dru_claim_callback($ajax, [
    'type' => 'node',
    'id' => $node->nid,
    'entity' => $node,
  ]);
}

/**
 * Callback dru_claim_comment.
 */
function dru_claim_comment($ajax, $comment, $token) {
  return !drupal_valid_token($token, 'dru_claim') ?: dru_claim_callback($ajax, [
    'type' => 'comment',
    'id' => $comment->cid,
    'entity' => $comment,
  ]);
}

/**
 * Dru_claim_callback.
 */
function dru_claim_callback($ajax, $vars) {
  $output = NULL;
  $is_ajax = $ajax === 'ajax';

  if ($is_ajax) {
    $form = drupal_get_form('dru_claim_report_form', $vars);
    $commands = [];

    $render_form = '<div id="verdict-add-form">';
    $render_form .= '<div id="close-form">×</div>';
    $render_form .= '<h2>';
    $render_form .= t('Add claim');
    $render_form .= '</h2>';
    $render_form .= render($form);
    $render_form .= '</div>';

    $commands[] = ajax_command_append('body', '<div id="verdict-add">' . $render_form . '</div>');
    $output = [
      '#type' => 'ajax',
      '#commands' => $commands,
    ];
  }
  else {
    $output = drupal_get_form('dru_claim_report_form', $vars);
  }
  return $output;
}

/**
 * Form builder for report a claim.
 */
function dru_claim_report_form($form, &$form_state, $vars) {
  $options = [];
  $voc = taxonomy_vocabulary_machine_name_load('claim_category');
  $terms = taxonomy_get_tree($voc->vid);

  foreach ($terms as $term) {
    $options[$term->tid] = $term->name;
  }

  $form['dru_claim_category'] = [
    '#type' => 'select',
    '#title' => t('Category of claim'),
    '#options' => $options,
    '#required' => TRUE,
  ];
  $form['dru_claim_description'] = [
    '#type' => 'textarea',
    '#title' => t('Claim description'),
    '#rows' => 4,
  ];

  $form['actions'] = [
    '#type' => 'actions',
  ];
  $form['actions']['dru_claim_submit'] = [
    '#type' => 'submit',
    '#value' => t('Send'),
    '#ajax' => [
      'callback' => 'dru_claim_submit_js',
      'method' => 'remove',
      'effect' => 'fade',
    ],
  ];

  $form_state['dru_claim_entity'] = $vars;

  return $form;
}

/**
 * Callback dru_claim_submit_js.
 */
function dru_claim_submit_js($form_id, $form_state) {
  $user = $GLOBALS['user'];
  $commands = [];

  // Create ticket.
  $ticket = new stdClass();
  $ticket->type = 'ticket';
  node_object_prepare($ticket);
  $ticket->title = t('Claim @type = @id', [
    '@type' => $form_state['dru_claim_entity']['type'],
    '@id' => $form_state['dru_claim_entity']['id'],
  ]);
  $ticket->language = LANGUAGE_NONE;
  $ticket->body[$ticket->language][0]['value'] = $form_state['values']['dru_claim_description'];
  $ticket->field_claim_category[$ticket->language][0]['tid'] = $form_state['values']['dru_claim_category'];

  $terms = taxonomy_get_term_by_name(t('Open'), 'ticket_status');
  $term = reset($terms);
  $ticket->field_ticket_status[$ticket->language][0]['tid'] = $term->tid;

  $ticket->uid = $user->uid;
  node_save($ticket);

  // Create claim.
  db_insert('ticket_claim')
    ->fields([
      'ticket_id' => $ticket->nid,
      'content_type' => $form_state['dru_claim_entity']['type'],
      'content_id' => $form_state['dru_claim_entity']['id'],
      'init_uid' => $user->uid,
    ])
    ->execute();

  // Update entity.
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
    $query->fields([
      'claim' => $ticket->nid,
    ]);
    $query->condition($id_type, $form_state['dru_claim_entity']['id'], '=');
    $query->execute();
  }

  $entity_link = $form_state['dru_claim_entity'];
  $type = $entity_link['type'];
  $id = $entity_link['id'];
  switch ($type) {
    case 'node':
      $body = 'field-name-body';
      break;

    case 'comment':
      $body = 'field-name-comment-body';
      break;
  }

  if (isset($body)) {
    switch ($type) {
      case 'node':
        $node = node_load($id, NULL, TRUE);
        $node_view = node_view($node);
        $commands[] = ajax_command_replace('.node', drupal_render($node_view));
        break;
      case 'comment':
        $comment = comment_load($id, TRUE);
        $node = node_load($comment->nid, NULL, TRUE);
        $comment_view = comment_view($comment, $node);
        $commands[] = ajax_command_replace('[data-' . $type . '-id="' . $id . '"]', drupal_render($comment_view));
        break;
    }
    $commands[] = ajax_command_remove('.claim-' . $type . '-' . $id);
    $commands[] = ajax_command_remove('#verdict-add');
  }

  return [
    '#type' => 'ajax',
    '#commands' => $commands,
  ];
}
