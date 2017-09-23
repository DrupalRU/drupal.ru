<?php
// $Id$
/**
 * @file
 *
 * Confirm the emailing of selected users.
 *
 */

require_once 'advuser_settings.inc';

/**
 * The menu callback function for
 * admin/people/advuser/confirm/email
 */
function advuser_multiple_email_confirm($form, &$form_state) {
  global $user;
  $advuser = &$_SESSION['advuser'];
  $accounts = &$advuser['accounts'];
  $selectall = &$advuser['selectall'];
  $deselected = &$advuser['deselected'];

  $form['accounts'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter returns only elements with TRUE values
  $form['accounts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Selected accounts'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE,
  );
  if ($selectall) {
    $sql = advuser_build_query('name');
    $filter = advuser_build_filter_query();
    $result = db_query($sql, $filter['args']);
    foreach ($result as $account) {
      if (!isset($deselected[$account->uid])) {
        $accounts[$account->uid] = $account->uid;
        $form['accounts'][$account->uid] = array(
          '#prefix' => '<li>',
          '#markup' => check_plain($account->name),
          '#suffix' => '</li>',
        );
      }
    }
  }
  else {
    foreach (array_filter($accounts) as $uid => $value) {
      $username = db_select('users', 'u')
        ->fields('u', array('name'))
        ->condition('u.uid', $uid, '=')
        ->execute()
        ->fetchField();
      $form['accounts'][$uid] = array(
        '#prefix' => '<li>',
        '#markup' => check_plain($username),
        '#suffix' => '</li>',
      );
    }
  }
  $form['operation'] = array(
    '#type' => 'hidden',
    '#value' => 'email'
  );

  $form['variables'] = array(
    '#prefix' => '<div class="advuser-inset-panel">',
    '#markup' => t(
      ADVUSER_SUBSTITUTION_TEXT,
      array('!tokens' => _advuser_get_tokens_list())
    ),
    '#suffix' => '</div>' 
  );

  $from = variable_get("site_mail", "nobody@$_SERVER[SERVER_NAME]");
  if (variable_get('advuser_senders_from_address', FALSE)) {
    $from = $user->mail;
  }
  if (variable_get('advuser_nobody_from_address', FALSE)) {
    $from = "nobody@$_SERVER[SERVER_NAME]";
  }

  $form['mailfrom'] = array(
    '#type' => 'textfield',
    '#title' => t('From'),
    '#value' => $from,
    '#default_value' => $from,
    '#disabled' => TRUE,
  );

  $form['mailsubject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
  );

  $form['mailbody'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail body'),
    '#required' => TRUE,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to email these users?'),
    'admin/people/advuser',
    t('This action cannot be undone.'),
    t('Email'),
    t('Cancel')
  );
}

/**
 * The submit handler for the Email button.
 */
function advuser_multiple_email_confirm_submit($form, &$form_state) {
  $advuser = &$_SESSION['advuser'];
  $accounts = &$advuser['accounts'];

  if ($form_state['values']['confirm']) {
    foreach ($accounts as $uid => $value) {
      $account = user_load((int)$uid);
      $from = $form_state['values']['mailfrom'];
      $types = array('user' => $account);
      $mail_subject = token_replace(
        $form_state['values']['mailsubject'],
        $types
      );
      $mail_body = token_replace(
        $form_state['values']['mailbody'],
        $types
      );
      drupal_mail('advuser', 'advuser-mail', $account->mail, user_preferred_language($account), array('subject' => $mail_subject, 'body' => array($mail_body)), $from, TRUE);
    }
    drupal_set_message(t('The users have been mailed.'));
  }
  $form_state['redirect'] = 'admin/people/advuser';
  advuser_reset_variables();
}

// vim:ft=php:sts=2:sw=2:ts=2:et:ai:sta:ff=unix
