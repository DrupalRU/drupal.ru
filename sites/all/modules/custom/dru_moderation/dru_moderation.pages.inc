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

  $form_state['dru_moderation_ban_reason'] = $vars;

  return $form;
}

/**
 * Callback dru_moderation_ban_form_validate.
 */
/*function dru_moderation_form_validate($form, &$form_state) {

}*/

/**
 * Callback dru_moderation_ban_submit_js.
 */
function dru_moderation_submit_js($form_id, $form_state) {

  $commands[] = ajax_command_remove('#verdict-add');
  $commands[] = ajax_command_remove('.ban');

  return array(
    '#type'     => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Callback dru_moderation_ban_form_submit.
 */
function dru_moderation_ban_form_submit($form_id, &$form_state) {
  // Pointer to ticket
  $ticket = $form_state['dru_moderation_ban_reason'];
  // Ban user UID
  $buid = $ticket['buid'];
  // Days from form
  $ban_days = $form_state['values']['dru_moderation_ban_until'];
  // Days to timestamp
  $ban_days_timestamp = $ban_days * 86400;
  // Final unban timestamp
  $ban_until = time() + $ban_days_timestamp;
  // Moderator UID
  $moder_uid = $ticket['ticket']->ticket->moderate_uid;
  // Reason
  $reason = $ticket['ticket']->ticket->ticket_verdict;

  // Add record to Database
  db_insert('dru_ban')
    ->fields(array(
      'uid' => $buid,
      'moderator' => $moder_uid,
      'reason' => $reason,
      'start_date' => date("Y-m-d"),
      'ban_until' => $ban_until,
    ))
    ->execute();

  // Ban user
  db_update('users')
    ->fields(array('status' => 0))
    ->condition('uid', $buid)
    ->execute();

  // Send mail to user and moderators
  $buser = user_load($buid);
  $moder = user_load($moder_uid);
  dru_moderation_sendmail($buser->name, $moder->name, $ban_days, $reason, 'ban');
}

