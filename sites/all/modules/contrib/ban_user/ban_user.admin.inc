<?php
/**
 * @file
 * Ban User - administration forms.
 */

/**
 * Form constructor for the ban user settings form.
 *
 * Path: admin/config/people/ban-user
 *
 * @see ban_user_menu()
 *
 * @ingroup forms
 */
function ban_user_settings_form($form, &$form_state) {
  global $base_url;

  $form = array();

  $form['ban_user_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message for a banned user'),
    '#default_value' => variable_get('ban_user_message', t('This account has been temporarily suspended. You will be able to access the site again on [ban-deadline].')),
    '#description' => t('Avilable token is [ban-deadline].'),
    '#rows' => 5,
  );
  $date_formats = array();
  foreach (system_get_date_types() as $key => $type) {
    $date_formats[$key] = $type['title'];
  }
  $form['ban_user_date_format'] = array(
    '#type' => 'select',
    '#title' => t('Preferred date format'),
    '#description' => t('You can edit the formats in <a href="!date-format">Date and Time Settings.</a>', array(
      '!date-format' => url('admin/config/regional/date-time'),
      )),
    '#options' => $date_formats,
    '#default_value' => variable_get('ban_user_date_format', 'medium'),
  );

  return system_settings_form($form);
}
