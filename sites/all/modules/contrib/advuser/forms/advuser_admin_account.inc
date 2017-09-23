<?php
// $Id$
/**
 * @file
 *
 * This is the part of the form that provides the actions and the list based
 * on the selected filters.
 *
 */

/**
 * Provide the list of filtered users.
 */
function advuser_admin_account($form, &$form_state) {
  $advuser_accounts = &$_SESSION['advuser']['accounts'];
  $selectall = &$_SESSION['advuser']['selectall'];

  // Clear the warnings.
  drupal_get_messages('warning');
  if (empty($advuser_accounts) && !$selectall) {
    drupal_set_message(t('No selections have been applied.'), 'warning', FALSE);
  }
  // Get the filters for the user data.
  $u_sel = db_select('users', 'u');
  advuser_build_filter_query($u_sel);

  // Get the list of user roles to be displayed.
  $roles = advuser_user_roles();

  // Array used for user profile title and value display in list.
  $ff = array();

  foreach (advuser_profile_fields() as $field) {
    if (isset($field->name)) {
      $ff[$field->name] = array('data' => t($field->title), 'field' => "$field->name.value");
    }
  }

  $header = array(
    'name' => array('data' => t('User name'), 'field' => 'u.name'),
    'mail' => array('data' => t('Mail'), 'field' => 'u.mail'),
    'status' => array('data' => t('Status'), 'field' => 'u.status'),
  );

  if (count($roles)) {
    $header['roles'] = array('data' => t('Roles'));
  }

  $header = array_merge($header, array(
    'member_for' => array('data' => t('Member for'), 'field' => 'u.created', 'sort' => 'desc'),
    'last_access' => array('data' => t('Last access'), 'field' => 'u.access'),
  ));

  $header = array_merge($header, $ff);

  $header['operations'] = array('data' => t('Operations'));

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Action options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#collapsible' => FALSE,
  );
  $options = array();

  // We don't want to call the user_operations here because we can't control
  // the functioning of the callbacks to the saved accounts in the session.
  // The callbacks only operate on the current screenfull of data and know
  // nothing about the Select All Users mode.
  $operations = module_invoke_all('advuser_operations');
  $operations += module_invoke_all('user_operations');

  foreach ($operations as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => 'saveselect',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply action'),
  );

  $destination = drupal_get_destination();

  $go = urldecode($destination['destination']);
  $form['destination'] = array('#type' => 'value', '#value' => $go);

  $status = array(t('blocked'), t('active'));

  $sql = advuser_build_query();
  $sql = $sql->extend('TableSort');

  $limit = variable_get('advuser_listno', 50);
  $include_uid1 = variable_get('advuser_allow_list_uid1', FALSE);

  $query_count = NULL;
  if ($limit !== 'unlimited') {
    $sql = $sql->extend('PagerDefault');

    $query_count = advuser_build_query('count');
    $sql->setCountQuery($query_count);

    // Correct LIMIT if advuser_allow_list_uid1 per foreach check below.
    if (empty($include_uid1)) {
      $limit++;
    }

    $sql->limit($limit);
  }

  $sql->orderByHeader($header);
  $result = $sql->execute();

  $accounts = array();
  $inputs = array();
  foreach ($result as $account) {
    if ($account->uid && ($account->uid != 1 || $include_uid1)) {
      $users_roles = array();
      $roles_result = db_select('users_roles', 'ur')
        ->fields('ur', array('rid'))
        ->condition('ur.uid', $account->uid, '=')
        ->execute();
      foreach ($roles_result as $user_role) {
        $users_roles[] = $roles[$user_role->rid];
      }
      asort($users_roles);

      $accounts[$account->uid] = array(
        'name' => theme('username', array('account' => $account)),
        'mail' => $account->mail,
        'status' => $status[$account->status],
        'roles' => theme('item_list', array('items' => $users_roles)),
        'member_for' => format_interval(REQUEST_TIME - $account->created),
        'last_access' => $account->access ? t('@time ago', array('@time' => format_interval(REQUEST_TIME - $account->access))) : t('never'),
      );
      if (!empty($advuser_accounts[$account->uid]) || $selectall) {
        $inputs[$account->uid] = $account->uid;
      }
      if (module_exists('profile')) {
        profile_user_load(array($account->uid => $account));
      }
      foreach (advuser_profile_fields() as $field) {
        $accounts[$account->uid][$field->name] = profile_view_field($account, $field);
      }
      if ($account->uid) {
        $accounts[$account->uid]['operations'] = array(
          'data' => array(
            'edit' => array(
              '#type' => 'link',
              '#title' => t('edit'),
              '#href' => "user/$account->uid/edit",
              '#options' => array('query' => $destination),
            ),
            array(
              '#markup' => ' | ',
            ),
            'delete' => array(
              '#type' => 'link',
              '#title' => t('delete'),
              '#href' => "user/$account->uid/cancel",
              '#options' => array('query' => $destination),
            ),
          ),
        );
      }
    }
  }

  if (!empty($query_count)) {
    $query_count = $query_count->execute()->fetchField();
    $query_count = t('Filtered %count users.', array(
      '%count' => $query_count,
    ));
  }

  $form['accounts'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $accounts,
    '#empty' => t('No people available.'),
    '#value' => $inputs,
    '#prefix' => (!empty($query_count) ? '<p>' . $query_count . '</p>' : ''),
  );
  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

/**
 * Submit the user administration update form.
 */
function advuser_admin_account_submit($form, &$form_state) {
  $operations = module_invoke_all('advuser_operations', $form_state);
  $operations += module_invoke_all('user_operations', $form, $form_state);
  $operation = $operations[$form_state['values']['operation']];

  $destination = $form_state['values']['destination'];
  // Filter out unchecked accounts.
  $accounts_selected = array_filter($form_state['values']['accounts']);
  $form_accounts = $form['accounts']['#options'];

  $advuser = &$_SESSION['advuser'];
  $accounts = &$advuser['accounts'];
  $selectall = &$advuser['selectall'];
  $deselected = &$advuser['deselected'];

  switch ($form_state['values']['operation']) {
    case 'saveselect': {
      $completeForm =& $form_state['complete form']['accounts'];
      foreach ($form_accounts as $form_user_id => $value) {
        $selected = isset($completeForm[$form_user_id]['#value'])
          ? $completeForm[$form_user_id]['#value']
          : 0;
        if ($selected) {
          $accounts[$form_user_id] = $form_user_id;
          unset($deselected[$form_user_id]);
        }
        else {
          $deselected[$form_user_id] = $form_user_id;
          unset($accounts[$form_user_id]);
        }
      }
      $selectall = FALSE;
    } break;

    case 'deselectall': {
      $selectall = FALSE;
      $accounts = array();
      $deselected = array();
      drupal_set_message(t('All selections have been reset.'));
    } break;

    case 'selectall': {
      $selectall = TRUE;
      $accounts = array();
      $deselected = array();
      drupal_set_message(t('All filtered users have been selected.'));
    } break;

    case 'email': {
      // Menu callback is provided for mass emailing.
      $completeForm =& $form_state['complete form']['accounts'];
      foreach ($form_accounts as $form_user_id => $value) {
        $selected = isset($completeForm[$form_user_id]['#value'])
        ? $completeForm[$form_user_id]['#value']
        : 0;
        if ($selected) {
          $accounts[$form_user_id] = $form_user_id;
        }
      }

      drupal_goto('admin/people/advuser/confirm/email');
    } break;

    case 'delete': {
      // Menu callback is provided for mass deleting.
      $completeForm =& $form_state['complete form']['accounts'];
      foreach ($form_accounts as $form_user_id => $value) {
        $selected = isset($completeForm[$form_user_id]['#value'])
          ? $completeForm[$form_user_id]['#value']
          : 0;
        if ($selected) {
          $accounts[$form_user_id] = $form_user_id;
        }
      }

      drupal_goto('admin/people/advuser/confirm/delete');
    } break;
  }





  if (!empty($operation['callback']) && $function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($accounts_selected), $operation['callback arguments']);
    }
    else {
      $args = array($accounts_selected);
    }
    call_user_func_array($function, $args);

    drupal_set_message(t('The update has been performed.'));
  }

}

// vim:ft=php:sts=2:sw=2:ts=2:et:ai:sta:ff=unix
