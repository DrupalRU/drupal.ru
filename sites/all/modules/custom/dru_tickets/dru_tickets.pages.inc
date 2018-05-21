<?php

/**
 * @file
 */

/**
 * Tickets page.
 */
function dru_tickets_page() {

  // Get ticket ststus terms.
  $status_voc = taxonomy_vocabulary_machine_name_load('ticket_status');
  $status_terms = taxonomy_get_tree($status_voc->vid);
  $status = array('any' => t('Select'));
  foreach ($status_terms as $status_term) {
    $status[$status_term->tid] = $status_term->name;
  }
  $default_status = array_keys($status);
  unset($default_status[0]);

  // Get claim category terms.
  $claim_category_voc = taxonomy_vocabulary_machine_name_load('claim_category');
  $claim_category_terms = taxonomy_get_tree($claim_category_voc->vid);
  $claim_category = array('any' => t('Select'));
  foreach ($claim_category_terms as $claim_category_term) {
    $claim_category[$claim_category_term->tid] = $claim_category_term->name;
  }
  $default_claim_category = array_keys($claim_category);
  unset($default_claim_category[0]);

  // Get query parametrs.
  $q = drupal_get_query_parameters();
  isset($q['status']) ? $default_status = array($q['status']) : $default_status;
  isset($q['claim_category']) ? $default_claim_category = array($q['claim_category']) : $default_claim_category;

  // Get filters form.
  $filter_vars = array(
    'status'         => array(
      'options' => $status,
      'default' => isset($q['status']) ? $q['status'] : 0,
    ),
    'claim_category' => array(
      'options' => $claim_category,
      'default' => isset($q['claim_category']) ? $q['claim_category'] : 0,
    ),
  );
  $filters = drupal_get_form('dru_tickets_page_filters', $filter_vars);

  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'node')
    ->propertyCondition('status', 1)
    ->propertyCondition('type', 'ticket')
    ->fieldCondition('field_ticket_status', 'tid', $default_status, 'IN')
    ->fieldCondition('field_claim_category', 'tid', $default_claim_category, 'in')
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
      ),
    );
  }
  else {
    $page = t('There are no tickets');
  }
  $output = '';
  $output .= render($filters);
  $output .= render($page);

  return $output;

}

/**
 * Ticket filters.
 */
function dru_tickets_page_filters($form, &$form_state, $filter_vars) {
  $form = array();

  $form['filter_status'] = array(
    '#type'          => 'select',
    '#title'         => t('Status'),
    '#options'       => $filter_vars['status']['options'],
    '#default_value' => $filter_vars['status']['default'],
  );

  $form['filter_claim_category'] = array(
    '#type'          => 'select',
    '#title'         => t('Claim category'),
    '#options'       => $filter_vars['claim_category']['options'],
    '#default_value' => $filter_vars['claim_category']['default'],
  );

  $form['filters_submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Send'),
  );

  $form['filters_clear'] = array(
    '#type'   => 'submit',
    '#value'  => t('Clear'),
    '#submit' => array('dru_tickets_page_filters_clear'),
  );

  return $form;
}

/**
 * Ticket filters submit.
 */
function dru_tickets_page_filters_clear($form, &$form_state) {
  drupal_goto(current_path(), array('query' => array()));
}

/**
 * Ticket filters submit.
 */
function dru_tickets_page_filters_submit($form, &$form_state) {
  $query = array();
  foreach ($form_state['values'] as $key => $filter) {
    if (
      substr($key, 0, 7) == 'filter_' &&
      !empty($filter) &&
      $filter !== 'any') {
      $query[substr($key, 7)] = $filter;
    }
  }
  drupal_goto(current_path(), array('query' => $query));
}

/**
 * Accept ticket.
 */
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
 * Verdict.
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

    $render_form = '<div id="verdict-add-form">';
    $render_form .= '<div id="close-form"><i class="fa fa-times"></i></div>';
    $render_form .= '<h2>';
    $render_form .= t('Add Verdict');
    $render_form .= '</h2>';
    $render_form .= render($form);
    $render_form .= '</div>';

    $commands[] = ajax_command_append('body', '<div id="verdict-add">' . $render_form . '</div>');

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
 * Dru_verdict_form.
 */
function dru_verdict_form($form, &$form_state, $vars) {
  $node = $vars['ticket'];

  if (empty($node->ticket->ticket_verdict)) {
    $verdict = '';
  }
  else {
    $verdict = $node->ticket->ticket_verdict;
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
 * Callback dru_verdict_form_validate.
 */
function dru_verdict_form_validate($form, &$form_state) {

}

/**
 * Callback dru_verdict_submit_js.
 */
function dru_verdict_submit_js($form_id, $form_state) {

  $nid = $form_state['dru_ticket']['ticket']->nid;
  $commands = array();

  $commands[] = ajax_command_replace('.verdict', '<div class="verdict">
        <div class="label">' . t('Verdict') . ':</div>
        <div class="value">' . check_plain($form_state['values']['dru_verdict']) . '</div>
      </div>');

  $commands[] = ajax_command_invoke('.verdict', 'addClass', array('tiket-changed'));

  $commands[] = ajax_command_remove('#verdict-add');

  $token = drupal_get_token('dru_tickets');
  $link = l(t('Change verdict'), 'ticket/' . $nid . '/verdict/nojs/' . $token, array('attributes' => array('class' => array('use-ajax', 'ticket-verdict-' . $nid))));
  $commands[] = ajax_command_replace('.ticket-verdict-' . $nid, $link);

  return array(
    '#type'     => 'ajax',
    '#commands' => $commands,
  );

}

/**
 * Callback dru_verdict_form_submit.
 */
function dru_verdict_form_submit($form_id, &$form_state) {

  $verdict = $form_state['values']['dru_verdict'];
  $ticket = $form_state['dru_ticket']['ticket'];

  $ticket->ticket->ticket_verdict = $verdict;

  if (!empty($verdict)) {
    $statuses = taxonomy_get_term_by_name(t('Closed'));
    foreach ($statuses as $tid => $status) {
      if ($status->vocabulary_machine_name == 'ticket_status') {
        $ticket->field_ticket_status[LANGUAGE_NONE][0]['tid'] = $status->tid;
      }
    }
    node_save($ticket);
  }
  else {
    $statuses = taxonomy_get_term_by_name(t('Open'));
    foreach ($statuses as $tid => $status) {
      if ($status->vocabulary_machine_name == 'ticket_status') {
        $ticket->field_ticket_status[LANGUAGE_NONE][0]['tid'] = $tid;
      }
    }
    node_save($ticket);
  }
}
