<?php

/**
 * Menu callback
 *
 * @return form
 */
function birthdays_admin_settings() {

  $form['birthdays_show_starsign'] = array(
    '#type' => 'select',
    '#title' => t('Show star signs'),
    '#default_value' => variable_get('birthdays_show_starsign', 0),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes, with link to Yahoo Astrology'),
      2 => t('Yes, without link to Yahoo Astrology')
    ),
    '#description' => t('Select whether the star signs should be enabled.'),
  );

  // Get fields from the profile.module of the type 'date'.
  $options = _birthdays_get_date_fields();
  // If there aren't any 'date' fields, throw a warning
  if (empty($options)) {
    drupal_set_message(t('No profile fields of type \'date\' were found, please <a href="@profile-settings-page">create one here</a>.', array('@profile-settings-page' => url('admin/user/profile/add/date'))), 'error');
  }

  $form['birthdays_field_name'] = array(
    '#type' => 'select',
    '#title' => t('Profile field'),
    '#default_value' => variable_get('birthdays_field_name', ''),
    '#description' => t('Choose the profile field of type \'date\' you want to use as date of birth.'),
    '#options' => $options,
    '#required' => TRUE,
  );

  $form['birthdays_hide_year'] = array(
    '#type' => 'select',
    '#title' => t('Hide year and age'),
    '#default_value' => variable_get('birthdays_hide_year', 0),
    '#description' => t('Select whether the birth year and age should be shown.'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
      2 => t('User optional, \'No\' by default'),
    ),
  );

  $form['birthdays_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Birthdays page settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['birthdays_page']['birthdays_page_settings'] = array(
    '#type' => 'select',
    '#title' => t('Set birthdays page settings'),
    '#default_value' => variable_get('birthdays_page_settings', 0),
    '#description' => t('Select whether users that haven\'t entered their date of birth should be shown on the birthdays page, and whether the list should be ordered by birthday or by username.'),
    '#options' => array(
      0 => t('Filter users, sort by date'),
      1 => t('Filter users, sort by username'),
      2 => t('Do not filter users, sort by username'),
    ),
  );

  $form['birthdays_page']['birthdays_page_list_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Birthdays page size'),
    '#default_value' => variable_get('birthdays_page_list_number', 25),
    '#description' => t('How many users should be listed on each page on the Birthdays listing?'),
    '#required' => TRUE,
  );

  $form['birthdays_page']['birthdays_page_show_filters'] = array(
    '#type' => 'select',
    '#title' => t('Show filter options'),
    '#default_value' => variable_get('birthdays_page_show_filters', 1),
    '#description' => t('Show the "Filter by month and year" selections on the birthdays page. When "Hide year" has been set to "Yes", the year filter isn\'t shown.'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
  );

  $form['birthdays_remind'] = array(
    '#type' => 'radios',
    '#title' => t('Send upcoming birthdays to admin'),
    '#default_value' => variable_get('birthdays_remind', 0),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#description' => t('Do you want a daily e-mail containing all upcoming birthdays (for the next week)?')
  );

  // Fieldset for User E-mails
  $form['birthdays_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail user'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['birthdays_email']['birthdays_send_user'] = array(
    '#type' => 'select',
    '#title' => t('Send user e-mail on day of birth'),
    '#default_value' => variable_get('birthdays_send_user', 0),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
      2 => t('User optional, \'Yes\' by default'),
    ),
    '#description' => t('Should users that have their birthday today receive an e-mail?'),
  );

  $form['birthdays_email']['birthdays_send_user_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => variable_get('birthdays_send_user_subject', t('Happy Birthday!')),
  );

  $form['birthdays_email']['birthdays_send_user_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => variable_get('birthdays_send_user_message', t("Hey @name,
Happy birthday!
Hope you have a great day!")
    ),
    '#description' => t('@name will be replaced by the username.'),
  );

  $form['#submit'][] = 'birthdays_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Submit hook for admin settings form
 *
 * @param string $form_id
 * @param array $values
 */
function birthdays_admin_settings_submit($form, $form_state) {
  $field = _birthdays_get_field($form_state['values']['birthdays_field_name']);
  $op = isset($form_state['clicked_button']['#value']) ? $form_state['clicked_button']['#value'] : '';
  if ($op == t('Reset to defaults')) {
    variable_del('birthdays_field_id');
  }
  else {
    variable_set('birthdays_field_id', $field->fid);
  }
}


/**
 * Callback for birthdays synchronisation system.
 */
function birthdays_sync_page() {
  if (variable_get('birthdays_field_name', '')) {
    return drupal_get_form('birthdays_sync_form');
  }
  else {
    return t("You haven't set a Profile field yet in the birthdays settings, please do so first before synchronising data fields.");
  }
}


/**
 * Implementation of hook_form().
 */
function birthdays_sync_form() {

  $form['profile_fieldset'] = array('#type' => 'fieldset');

  $form['profile_fieldset']['description'] = array(
    '#type' => 'item',
    '#title' => 'Profile to Birthdays',
    '#value' => t('Fill the Birthdays module\'s table with data from the Profile module\'s table. This is needed when you (re-)installed the Birthdays module, but already collected birthdays with the Profile module (e.g. for age verification).'),
  );

  $form['profile_fieldset']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Copy Profile data to Birthdays module'),
    '#submit' => array('birthdays_sync_form_copy_submit')
  );

  $form['birthdays_fieldset'] = array('#type' => 'fieldset');

  $form['birthdays_fieldset']['description'] = array(
    '#type' => 'item',
    '#title' => 'Birthdays to Profile',
    '#value' => t('Fill the Profile module\'s table with data from the Birthdays module\'s table. You can use this to copy the old data collected by a previous version of the Birthdays module after an update. It is only necessary when updating from a version of the Birthdays module which didn\'t use the Profile module.'),
  );

  $form['birthdays_fieldset']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Copy Birthdays data to Profile module'),
  );
  return $form;
}

function birthdays_sync_form_copy_submit($form, &$form_state) {
  $field = variable_get('birthdays_field_name', '');
  $result = db_query('SELECT uid FROM {users}');
  while ($user = db_fetch_object($result)) {
    $account = user_load(array('uid' => $user->uid));
    if ($account->$field) {
      user_save($account, array($field => $account->$field));
    }
  }
  drupal_set_message(t('Modules have been synchronized.'));
}

/**
 * Implementation of hook_form_submit().
 */
function birthdays_sync_form_submit($form, &$form_state) {
  $field = variable_get('birthdays_field_name', '');

  switch ($GLOBALS['db_type']) {
    case 'mysql':
    case 'mysqli':
      $result = db_query('SELECT {dob}.uid, MONTH({dob}.birthday) AS month, YEAR({dob}.birthday) AS year, DAYOFMONTH({dob}.birthday) AS day FROM {dob}');
      break;
    case 'pgsql':
      $result = db_query("SELECT uid, date_part('month',{dob}.birthday) AS month, date_part('year',{dob}.birthday) AS year, date_part('day',{dob}.birthday) AS day FROM {dob}");
      break;
  }

  while ($birthday = db_fetch_object($result)) {
    $account = user_load(array('uid' => $birthday->uid));
    $dob = array(
      'day' => $birthday->day,
      'month' => $birthday->month,
      'year' => $birthday->year
    );
    user_save($account, array($field => $dob));
  }
  drupal_set_message(t('Modules have been synchronized.'));
}

