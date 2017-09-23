<?php

/**
 * @file
 * A sample implementation of Mail Editor hooks for User core module.
 */

/**
 * Implements hook_mailkeys().
 *
 * @return array
 */
function user_mailkeys() {
  return array(
    'register_no_approval_required'   => t('Welcome message when user self-registers'),
    'register_admin_created'          => t('Welcome message for user created by the admin'),
    'register_pending_approval'       => t('Welcome message, user pending admin approval'),
    'register_pending_approval_admin' => t('Message to admin, user is pending'),
    'password_reset'                  => t('Password recovery request'),
    'status_activated'                => t('Account activated'),
    'status_blocked'                  => t('Account blocked'),
    'cancel_confirm'                  => t('Request to confirm cancelation'),
    'status_canceled'                 => t('Account canceled'),
  );
}

/**
 * Implements hook_mail_edit_text().
 *
 * @param string $mailkey
 * @param object $language
 *
 * @return array
 */
function user_mail_edit_text($mailkey, $language) {
  $return = array();
  $return['subject'] = _user_mail_text($mailkey . '_subject', $language, array(), FALSE);
  $return['body'] = _user_mail_text($mailkey . '_body', $language, array(), FALSE);
  return $return;
}

/**
 * Implements hook_mail_edit_token_types().
 *
 * @param string $mailkey
 *
 * @return array
 */
function user_mail_edit_token_types($mailkey) {
  return array('user');
}
