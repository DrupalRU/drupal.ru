<?php
/**
 * @file
 * Ban User - user forms.
 */

/**
 * Form constructor for the ban user form.
 *
 * Path: admin/people/ban-user
 *
 * @see ban_user_menu()
 * @see ban_user_list_form_submit()
 * @see ban_user_list_form_confirm()
 *
 * @ingroup forms
 */
function ban_user_list_form($form, &$form_state) {
  if (isset($form_state['storage']['confirm'])) {
    return ban_user_list_form_confirm($form_state, 'admin/people/ban-user');
  }

  $form = array();

  $header = array(
    'uid' => array('data' => t('User ID')),
    'name' => array('data' => t('Name'), 'field' => 'u.name', 'sort' => 'asc'),
    'deadline' => array('data' => t('Deadline'), 'field' => 'b.deadline', 'sort' => 'asc'),
    'changed' => array('data' => t('Last update'), 'field' => 'b.created', 'sort' => 'desc'),
  );

  $query = db_select('ban_user', 'b');
  $query->join('users', 'u', 'u.uid = b.uid');
  $banned = $query
    ->fields('b', array('uid', 'deadline', 'changed'))
    ->fields('u', array('name'))
    ->condition('b.deadline', REQUEST_TIME, '>')
    ->extend('TableSort')->orderByHeader($header)
    ->extend('PagerDefault')->limit(25)
    ->execute();

  $rows = array();
  foreach ($banned as $user) {
    $rows[$user->uid] = array(
      'uid' => check_plain($user->uid),
      'name' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => check_plain($user->name),
          '#href' => check_url('user/'. $user->uid),
          '#options' => array('title' => t('view user')),
        ),
      ),
      'deadline' => format_date($user->deadline, variable_get('ban_user_date_format', 'medium')),
      'changed' => format_date($user->changed, variable_get('ban_user_date_format', 'medium')),
    );
  }

  $form['banned'] = array(
    '#type' => 'tableselect',
    '#multiple' => TRUE,
    '#sticky' => TRUE,
    '#header' => $header,
    '#options' => $rows,
    '#attributes' => array('class' => array('ban-user-list')),
    '#empty' => t('No banned users are found.'),
  );

  $form['pager'] = array('#theme' => 'pager');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Pardon selected users'),
    '#submit' => array('ban_user_list_form_submit'),
  );

  return $form;
}

/**
 * Form submission handler for ban_user_list_form().
 */
function ban_user_list_form_submit($form, &$form_state) {
  if (!isset($form_state['storage']['confirm'])) {
    $form_state['storage']['confirm'] = $form_state['rebuild'] = TRUE;
    $form_state['storage']['banned'] = $form_state['values']['banned'];
  }
  else {
    $uids = array_filter($form_state['storage']['banned']);
    foreach ($uids as $uid) {
      db_delete('ban_user')
        ->condition('uid', $uid)
        ->execute();
    }
    drupal_set_message(format_plural(count($uids), 'Pardoned one user.', 'Pardoned @count users.'));
    if (isset($form_state['values']['destination'])) {
      $form_state['redirect'] = $form_state['values']['destination'];
    }
  }
}

/**
 * Form confirmation handler for ban_user_list_form().
 */
function ban_user_list_form_confirm(&$form_state, $destination) {
  $form = array();

  $form['destination'] = array(
    '#type' => 'hidden',
    '#value' => $destination,
  );

  return confirm_form($form,
    t('Are you sure you want to pardon the selected banned users?'),
    isset($_GET['destination']) ? $_GET['destination'] : $destination,
    NULL,
    t('Pardon')
  );
}
