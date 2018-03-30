<?php

/**
 * Moderation Ban.
 */
function dru_moderation_ban($ajax, $node, $uid, $token) {
  if (!drupal_valid_token($token, 'dru_moderation')) {
    return;
  }

  $is_ajax = $ajax === 'ajax';
  $vars = [
    'ticket' => $node,
    'buid' => $uid,
  ];

  if ($is_ajax) {
    $form = drupal_get_form('dru_moderation_ban_form', $vars);
    $commands = [];
    $render_form  = '<div id="verdict-add-form">';
    $render_form .= '<div id="close-form"><i class="fa fa-times"></i></div>';
    $render_form .= '<h2>';
    $render_form .= t('Ban user!');
    $render_form .= '</h2>';
    $render_form .= render($form);
    $render_form .= '</div>';
    $commands[] = ajax_command_append('body', '<div id="verdict-add">' . $render_form . '</div>');
    return [
      '#type' => 'ajax',
      '#commands' => $commands,
    ];
  }
  else {
    return drupal_get_form('dru_moderation_ban_form', $vars);
  }
}

/**
 * dru_moderation_ban_form.
 */
function dru_moderation_ban_form($form, &$form_state, $vars) {
  $form['dru_moderation_ban_until'] = [
    '#type'          => 'textfield',
    '#title'         => t('Number of days to block user'),
    '#required'      => TRUE,
    '#element_validate' => array('element_validate_integer_positive'), 
    '#default_value' => '3',
  ];

  $form['dru_moderation_ban_submit'] = [
    '#type'  => 'submit',
    '#value' => t('Ban!'),
    '#ajax'  => [
      'callback' => 'dru_moderation_submit_js',
    ],
    '#submit' => ['dru_moderation_ban_form_submit'],
  ];

  $form_state['dru_moderation_ban_reason'] = $vars;

  return $form;
}

/**
 * Callback dru_moderation_ban_form_validate.
 */
/*function dru_moderation_ban_form_validate(&$elements, &$form_state, $form_id = NULL) {
  // nothing here
}*/

/**
 * Callback dru_moderation_ban_submit_js.
 */
function dru_moderation_submit_js($form, $form_state) {
  if (!form_get_errors()) {
    $commands[] = ajax_command_remove('#verdict-add');
    $commands[] = ajax_command_remove('.dru-moderation-ban');
  }
  else {
    $commands[] = ajax_command_prepend('#verdict-add-form', '<div class="alert alert-block alert-danger">' . t('Error submit form. Field "Number of days to block user" must be not empty and must be unsigned integer') . '</div>');
  }
  return [
    '#type'     => 'ajax',
    '#commands' => $commands,
  ];
}

/**
 * Callback dru_moderation_ban_form_submit.
 */
function dru_moderation_ban_form_submit($form_id, &$form_state) {
  if (!form_get_errors()) {
    // Pointer to ticket
    $ticket = $form_state['dru_moderation_ban_reason'];
    // Ban user UID
    $buid = $ticket['buid'];
    // Days from form
    $ban_days = $form_state['values']['dru_moderation_ban_until'];
    // Days to timestamp
    $ban_days_timestamp = $ban_days * 86400;
    // Final unban timestamp
    $ban_until = REQUEST_TIME + $ban_days_timestamp;
    // Moderator UID
    $moder_uid = $ticket['ticket']->ticket->moderate_uid;
    // Reason
    $reason = $ticket['ticket']->ticket->ticket_verdict;
    // Ticket ID
    $ticket_id = $ticket['ticket']->ticket->ticket_id;

    // Add record to Database
    db_insert('dru_ban')
      ->fields([
        'uid' => $buid,
        'moderator' => $moder_uid,
        'ticket_id' => $ticket_id,
        'start_date' => date("d.m.Y"),
        'ban_until' => $ban_until,
      ])
      ->execute();

    // Ban user
    db_update('users')
      ->fields(['status' => 0])
      ->condition('uid', $buid)
      ->execute();

    // Send mail to user and moderators
    $buser = user_load($buid);
    $moder = user_load($moder_uid);
    dru_moderation_sendmail($buser->name, $ban_days, $reason, 'ban');
  }
}

