<?php

/**
 * Callback dru_tnx_entity
 */
function dru_tnx_entity($entity_type, $ajax, $entity, $token) {

  if (!drupal_valid_token($token, 'dru_tnx')) {
    return;
  }

  global $user;

  $id  = $entity_type == 'node' ? $entity->nid : $entity->cid;
  $tnx = get_user_tnx($entity_type, $id, $user->uid);

  $vars = array(
    'type'   => $entity_type,
    'id'     => $id,
    'entity' => $entity,
    'tnx'    => $tnx,
    'user'   => $user,
  );

  return dru_tnx_callback($ajax, $vars);
}

/**
 * dru_tnx_callback
 */
function dru_tnx_callback($ajax, $vars) {

  dru_tnx_data($vars);

  if ($ajax == 'ajax') {

    $tnx = get_user_tnx($vars['type'], $vars['id'], $vars['user']->uid);

    $vars['entity']->tnx = $vars['entity']->tnx + ($tnx ? -1 : 1);

    $tnx_link = dru_tnx_link($vars['type'], $vars['id'], $tnx);

    $link = l($tnx_link['title'], $tnx_link['href'],
      array(
        'attributes' => $tnx_link['attributes'],
      ));

    $counter = dru_tnx_view($vars['entity'], $vars['type']);

    $animate = array('plus', 'minus');

    $counter['#attributes']['class'][] = $animate[$tnx];

    $link_selector = '.dru-tnx-';
    $link_selector .= $vars['type'];
    $link_selector .= '-';
    $link_selector .= $vars['id'];

    $counter_selector = $link_selector . '-counter';

    $commands   = array();
    $commands[] = ajax_command_replace($counter_selector, render($counter));
    $commands[] = ajax_command_replace($link_selector, $link);

    return array(
      '#type'     => 'ajax',
      '#commands' => $commands,
    );
  }
  else {
    drupal_set_message($vars['tnx'] == 'tnx' ? t('Your Thank Accepted') : t('Your Thank Deleted'));
    drupal_goto();
  }
}

/**
 * Callback dru_tnx_data
 */
function dru_tnx_data($vars) {

  // Counting tnx by entity
  $tnx_count = $vars['entity']->tnx;

  if ($vars['tnx'] == 'tnx') {
    $tnx_count++;

    // Add tnx
    db_insert('tnx')->fields(
      array(
        'entity_id'   => $vars['id'],
        'entity_type' => $vars['type'],
        'uid'         => $vars['user']->uid,
      )
    )->execute();

    // Count tnx
    db_merge('tnx_stat')
      ->key(array('uid' => $vars['entity']->uid))
      ->fields(array($vars['type'] => 1))
      ->expression($vars['type'], $vars['type'] . ' + :inc', array(':inc' => 1))
      ->execute();

    // Count user tnx
    db_merge('tnx_stat')
      ->key(array('uid' => $vars['user']->uid))
      ->fields(array('tnx_' . $vars['type'] => 1))
      ->expression('tnx_' . $vars['type'], 'tnx_' . $vars['type'] . ' + :inc',
        array(':inc' => 1))
      ->execute();
  }
  else {
    $tnx_count--;

    // Delete tnx
    db_delete('tnx')
      ->condition('entity_id', $vars['id'])
      ->condition('entity_type', $vars['type'])
      ->condition('uid', $vars['user']->uid)
      ->execute();

    if ($vars['entity']->tnx) {
      // Count tnx
      db_merge('tnx_stat')
        ->key(array('uid' => $vars['entity']->uid))
        ->fields(array($vars['type'] => 0))
        ->expression($vars['type'], $vars['type'] . ' - :inc',
          array(':inc' => 1))
        ->execute();
      // Count user tnx
      db_merge('tnx_stat')
        ->key(array('uid' => $vars['user']->uid))
        ->fields(array('tnx_' . $vars['type'] => 0))
        ->expression('tnx_' . $vars['type'], 'tnx_' . $vars['type'] . ' - :inc',
          array(':inc' => 1))
        ->execute();
    }
  }

  // Write tnx count to entity
  $query = db_update($vars['type']);
  $query->condition($vars['type'] == 'node' ? 'nid' : 'cid', $vars['id'])
        ->fields(array('tnx' => $tnx_count))
        ->execute();
}
