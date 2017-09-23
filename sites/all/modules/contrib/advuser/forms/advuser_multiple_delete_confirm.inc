<?php
// $Id$
/**
 * @file
 *
 * Confirm the delete of selected users.
 *
 */

/**
 * Implement the menu callback function for
 * admin/people/advuser/confirm/delete.
 */
function advuser_multiple_delete_confirm() {
  $advuser = &$_SESSION['advuser'];
  $accounts = &$advuser['accounts'];
  $selectall = &$advuser['selectall'];
  $deselected = &$advuser['deselected'];

  $form['accounts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Selected users'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE
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
    // array_filter returns only elements with TRUE values
    foreach (array_filter($accounts) as $uid => $value) {
      $user = db_select('users', 'u')
        ->fields('u', array('name'))
        ->condition('u.uid', $uid, '=')
        ->execute()
        ->fetchField();
      $form['accounts'][$uid] = array(
        '#prefix' => '<li>',
        '#markup' => check_plain($user),
        '#suffix' => '</li>',
      );
    }
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');

  return confirm_form(
    $form,
    t('Are you sure you want to delete these users?'),
    'admin/people/advuser', t('This action cannot be undone.'),
    t('Delete all'),
    t('Cancel')
  );
}

/**
 * Submit handler for the "Delete all" button.
 */
function advuser_multiple_delete_confirm_submit($form, &$form_state) {
  $advuser = &$_SESSION['advuser'];
  $accounts = &$advuser['accounts'];

  if ($form_state['values']['confirm']) {
    foreach ($accounts as $uid => $value) {
      user_cancel($form_state['values'], $uid, 'user_cancel_delete');
    }
    drupal_set_message(t('The users have been deleted.'));
  }
  $form_state['redirect'] = 'admin/people/advuser';
  advuser_reset_variables();
}

// vim:ft=php:sts=2:sw=2:ts=2:et:ai:sta:ff=unix
