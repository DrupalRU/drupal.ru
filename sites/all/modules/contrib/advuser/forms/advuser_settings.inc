<?php
// $Id$
/**
 * @file
 *
 * The admin/settings/advuser form.
 *

/**
 * The callback function for admin/settings/advuser.
 */
function advuser_settings() {
  // We need to clear the warning because of the advuser_notify_uid1 field and
  // the profile fields selection.
  drupal_get_messages('advuser_warning');

  // Provide a warning that no notifications will be sent.
  if (!count(users_by_access_('receive email advuser'))) {
    drupal_set_message(t('No notifications of account activity will be sent due to the settings here or the roles, permissions and assigned users to the roles.'), 'advuser_warning', FALSE);
  }

  // Controls for the page listings.
  $form['advuser_list'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controls for the page listings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Maximum rows in dataset to display?
  $form['advuser_list']['advuser_listno'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(50, 100, 500, 1000, 'unlimited')),
    '#title' => t('Number of users in listing'),
    '#description' => t('Sets how many users to display in table view'),
    '#default_value' => variable_get('advuser_listno', 50),
  );

  // Display the administrative user?
  $form['advuser_list']['advuser_allow_list_uid1'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow the site administrator (uid 1) to be listed'),
    '#description' => t('It is advisable for you to leave this unchecked but if you feel comfortable with the site administrator being displayed for editing in mass lists you may check this active.'),
    '#default_value' => variable_get('advuser_allow_list_uid1', FALSE),
  );

  if (variable_get('advuser_allow_list_uid1', FALSE)) {
    drupal_set_message(t('Site administrator is set to be editable in mass lists.'), 'advuser_warning', FALSE);
  }

  // Controls for the user activity notifications.
  $form['advuser_mail'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controls for mail notifications on user account activity'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Log the notifications to the watchdog table?
  $form['advuser_mail']['advuser_log_notifications'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log the notifications sent?'),
    '#description' => t('Output a message to the watchdog giving the specifics of the email notification sent.  This can be useful if you do not want notification mail sent but want to log the actions instead.'),
    '#default_value' => variable_get('advuser_log_notifications', FALSE),
  );

  // Notify administrative user of user activity?
  $uid1 = user_load(1);
  $form['advuser_mail']['advuser_notify_uid1'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send account activity mail to %user?', array('%user' => $uid1->name)),
    '#description' => t('Notify the site administrator of account activity if notifications are being used?'),
    '#default_value' => variable_get('advuser_notify_uid1', FALSE),
  );

  // New account notifications.
  $form['advuser_mail']['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration notification controls'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Inform of allowed tokens.
  $form['advuser_mail']['registration']['variables'] = array(
    '#prefix' => '<div class="advuser-inset-panel">',
    '#markup' => t(
      ADVUSER_SUBSTITUTION_TEXT,
      array('!tokens' => _advuser_get_tokens_list())
    ),
    '#suffix' => '</div>',
  );

  // Notify for new account registration?
  $form['advuser_mail']['registration']['advuser_new_notify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send notifications on new user registration'),
    '#description' => t('Notify selected roles when new users register.'),
    '#default_value' => variable_get('advuser_new_notify', FALSE),
  );

  // The subject of the mail.
  $form['advuser_mail']['registration']['advuser_new_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail subject'),
    '#description' => t('The subject of the mail that is going to be sent to the user.  You may insert substitution variables within this item.'),
    '#default_value' => variable_get('advuser_new_subject', t(ADVUSER_DEFAULT_NEW_SUBJECT)),
  );

  // The body of the mail.
  $form['advuser_mail']['registration']['advuser_new_mail'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail body'),
    '#description' => t('The mail that is going to be sent to the selected roles.  You may insert substitution variables within this item.'),
    '#default_value' => variable_get('advuser_new_mail', t(ADVUSER_DEFAULT_NEW_MAIL)),
  );

  // Account change notifications.
  $form['advuser_mail']['update'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account update notification controls'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Inform of allowed tokens.
  $form['advuser_mail']['update']['variables'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="advuser-inset-panel">',
    '#markup' => t(
      ADVUSER_SUBSTITUTION_TEXT,
      array('!tokens' => _advuser_get_tokens_list())
    ),
    '#suffix' => '</div>',
  );

  // Notify for updated account activity?
  $form['advuser_mail']['update']['advuser_modify_notify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send notifications on user profile updates'),
    '#description' => t('Notify selected roles when users update their profiles.'),
    '#default_value' => variable_get('advuser_modify_notify', FALSE),
  );

  // The subject of the mail.
  $form['advuser_mail']['update']['advuser_modify_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail subject'),
    '#description' => t('The subject of the mail that is going to be sent when a user modifies their profiles.  You may insert substitution variables within this item.'),
    '#default_value' => variable_get('advuser_modify_subject', t(ADVUSER_DEFAULT_MODIFY_SUBJECT)),
  );

  // The body of the mail.
  $form['advuser_mail']['update']['advuser_modify_mail'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail body'),
    '#description' => t('The mail that is going to be sent to the selected roles when a user modifies their account.  You may insert substitution variables within this item.'),
    '#default_value' => variable_get('advuser_modify_mail', t(ADVUSER_DEFAULT_MODIFY_MAIL)),
  );

  // Provide checkboxes for available profile fields.
  if (module_exists('profile')) {
    $form['advuser_profile'] = array(
      '#type' => 'fieldset',
      '#title' => t('Controls for profile fields'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $fields = array();
    $result = db_select('profile_field', 'pf')
      ->fields('pf')
      ->orderBy('category')
      ->orderBy('weight')
      ->execute();
    foreach ($result as $row) {
      $fields[$row->fid] = $row->title;
    }

    $values = array();
    $options = variable_get('advuser_profile_fields', NULL);
    foreach ((array)$options as $opt => $v) {
      if ($v > 0) {
        $values[] = $v;
      }
    }

    if (!count($values)) {
      drupal_set_message(t('Profile fields have not been selected for use.'), 'advuser_warning', FALSE);
    }

    $form['advuser_profile']['advuser_profile_fields'] = array(
        '#type' => 'checkboxes',
        '#description' => t('Profile fields to be used as filters for the users.'),
        '#title' => t('Profile fields'),
        '#options' => $fields,
        '#default_value' => $values,
    );
  }

  // Controls for miscellaneous options.
  $form['advuser_misc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controls for miscellaneous options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Reset account access column for administrative changes?
  $form['advuser_misc']['advuser_reset_never_access'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reset the accessed to never on administrative update'),
    '#description' => t('Because of a bug caused by !link, watch for administrative updates that change the accessed column from 0 (never) and reset it.', array('!link' => l(t('this issue'), 'http://drupal.org/node/171117'))),
    '#default_value' => variable_get('advuser_reset_never_access', FALSE),
  );

  // Reset account access column for administrative registrations?
  $form['advuser_misc']['advuser_set_never_access'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set accessed to never on administrative create'),
    '#description' => t('Because of a bug caused by !link, watch for administrative creates and set the accessed column to 0 (never).', array('!link' => l(t('this issue'), 'http://drupal.org/node/171117'))),
    '#default_value' => variable_get('advuser_set_never_access', FALSE),
  );

  // Set From address to authenticated users mail address?
  $form['advuser_misc']['advuser_senders_from_address'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the authenticated users mail address'),
    '#description' => t('When sending mail to users use the senders mail as the from address?'),
    '#default_value' => variable_get('advuser_senders_from_address', FALSE),
  );

  // Set From address to nobody@$_SERVER[SERVER_NAME]?
  $form['advuser_misc']['advuser_nobody_from_address'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use "nobody" as the from address'),
    '#description' => t('When sending mail to users use %nobody as the from address?', array('%nobody' => "nobody@$_SERVER[SERVER_NAME]")),
    '#default_value' => variable_get('advuser_nobody_from_address', FALSE),
  );

  // Create warnings based on selections.
  if (variable_get('advuser_senders_from_address', FALSE) && variable_get('advuser_nobody_from_address', FALSE)) {
    drupal_set_message(t('You have selected to use a from address of both from sender and from nobody, the from nobody will be used.'), 'advuser_warning', FALSE);
  }
  elseif (variable_get('advuser_senders_from_address', FALSE)) {
    drupal_set_message(t('Caution, you need to make sure that your users have authority to send mail based on your environment.'), 'advuser_warning');
  }
  elseif (variable_get('advuser_nobody_from_address', FALSE)) {
    drupal_set_message(t('Caution, you need to make sure that %nobody has authority to send mail based on your environment.', array('%nobody' => "nobody@$_SERVER[SERVER_NAME]")), 'advuser_warning');
  }

  return system_settings_form($form);
}

/**
 * Returns the HTML representation of the available tokens list.
 *
 * @return string The HTML representation of the list of usable tokens and their
 * descriptions, organized by token type.
 */
function _advuser_get_tokens_list() {
  static $tokens_list = '';

  if (!$tokens_list) {
    $tokens_list = theme('token_tree', array('token_types' => array('user', 'site'), 'click_insert' => FALSE, 'show_restricted' => FALSE, 'global_types' => FALSE));
  }
  return $tokens_list;
}

// vim:ft=php:sts=2:sw=2:ts=2:et:ai:sta:ff=unix
