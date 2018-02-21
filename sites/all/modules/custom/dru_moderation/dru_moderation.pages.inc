<?php

/**
 * Moderation Ban.
 */
function dru_moderation_ban($ajax, $node, $uid, $token) {
  if (!drupal_valid_token($token, 'dru_moderation')) {
    return;
  }

  $is_ajax = $ajax === 'ajax';
  $vars = array(
    'ticket' => $node,
    'buid' => $uid,
  );

  if ($is_ajax) {
    $form = drupal_get_form('dru_moderation_ban_form', $vars);
    $commands = array();
    $render_form  = '<div id="verdict-add-form">';
    $render_form .= '<div id="close-form"><i class="fa fa-times"></i></div>';
    $render_form .= '<h2>';
    $render_form .= t('Ban user!');
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
    return drupal_get_form('dru_moderation_ban_form', $vars);
  }
}

/**
 * dru_moderation_ban_form.
 */
function dru_moderation_ban_form($form, &$form_state, $vars) {
  $form['dru_moderation_ban_reason'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Reason'),
    '#disabled'      => TRUE,
    '#default_value' => $vars['ticket']->ticket->ticket_verdict,
  );

  $form['dru_moderation_ban_until'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Ban on ... days'),
    '#default_value' => '1',
  );

  $form['dru_moderation_ban_submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Ban!'),
    '#ajax'  => array(
      'callback' => 'dru_moderation_submit_js',
    ),
    '#submit' => array('dru_moderation_ban_form_submit'),
  );

  return $form;
}

/**
 * Callback dru_moderation_ban_form_validate.
 */
function dru_moderation_form_validate($form, &$form_state) {

}

/**
 * Callback dru_moderation_ban_submit_js.
 */
function dru_moderation_submit_js($form_id, $form_state) {
  $commands[] = ajax_command_remove('#verdict-add');
  return array(
    '#type'     => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Callback dru_moderation_ban_form_submit.
 */
function dru_moderation_ban_form_submit($form_id, &$form_state) {
dpm($form_state);
  /*$verdict = $form_state['values']['dru_verdict'];
  $ticket  = $form_state['dru_ticket']['ticket'];

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
  }*/
}

