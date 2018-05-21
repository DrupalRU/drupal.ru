<?php

/**
 * @file
 * Data Base functionality.
 */

/**
 * Implementation dru_tnx_get_user_statistic().
 *
 * Return user statistic object.
 */
function dru_tnx_get_user_statistic($uid) {
  $statistic = db_select('tnx_stat', 't')
    ->fields('t')
    ->condition('t.uid', $uid)
    ->execute()
    ->fetchObject();

  // If user don't have statistic.
  if (!$statistic) {
    $statistic = new stdClass();
    $statistic->uid = $uid;
    $statistic->tnx_node = 0;
    $statistic->tnx_comment = 0;
    $statistic->node = 0;
    $statistic->comment = 0;
  }

  return $statistic;
}

/**
 * Implementation dru_tnx_get_user_tnx().
 *
 * Return Boolean TRUE if user already have thanked.
 *
 * @param string $entity_type
 *   Entity type ("node" or "comment").
 * @param int $entity_id
 *   Entity ID (NID or CID).
 * @param int $uid
 *   Thanked user ID (UID).
 *
 * @return bool
 */
function dru_tnx_get_user_tnx($entity_type, $entity_id, $uid) {
  return !db_select('tnx', 't')
    ->condition('t.entity_type', $entity_type)
    ->condition('t.entity_id', $entity_id)
    ->condition('t.uid', $uid)
    ->countQuery()
    ->execute()
    ->fetchField();
}

/**
 * Insert Thanks.
 *
 * @param array $vars
 *
 * @return \DatabaseStatementInterface|int
 */
function dru_tnx_insert_tnx($vars) {
  return db_insert('tnx')
    ->fields([
      'entity_id'   => $vars['id'],
      'entity_type' => $vars['type'],
      'uid'         => $vars['user']->uid,
    ])->execute();
}

/**
 * Counting thanks statistic.
 *
 * @param array $vars
 * @param int $add
 *
 * @return int
 */
function dru_tnx_counting_tnx_statistic($vars, $add = 1) {
  $act = $add ? '+' : '-';
  return db_merge('tnx_stat')
    ->key(['uid' => $vars['entity']->uid])
    ->fields([$vars['type'] => $add])
    ->expression($vars['type'], "{$vars['type']} $act 1")
    ->execute();
}

/**
 * Counting user statistic.
 *
 * @param array $vars
 * @param int $add
 *
 * @return int
 */
function dru_tnx_counting_user_statistic($vars, $add = 1) {
  $act = $add ? '+' : '-';
  return db_merge('tnx_stat')
    ->key(['uid' => $vars['user']->uid])
    ->fields(['tnx_' . $vars['type'] => $add])
    ->expression('tnx_' . $vars['type'], "tnx_{$vars['type']} $act 1")
    ->execute();
}

/**
 * Delete Thanks.
 *
 * @param array $vars
 *
 * @return \DatabaseStatementInterface|int
 */
function dru_tnx_delete_tnx($vars) {
  return db_delete('tnx')
    ->condition('entity_id', $vars['id'])
    ->condition('entity_type', $vars['type'])
    ->condition('uid', $vars['user']->uid)
    ->execute();
}

/**
 * Callback dru_tnx_data.
 */
function dru_tnx_data($vars) {

  // Counting tnx by entity.
  $tnx_count = $vars['entity']->tnx;

  if ($vars['tnx'] == 'tnx') {
    $tnx_count++;

    // Add tnx.
    dru_tnx_insert_tnx($vars);
    // Count tnx.
    dru_tnx_counting_tnx_statistic($vars);
    // Count user tnx.
    dru_tnx_counting_user_statistic($vars);
  }
  else {
    $tnx_count--;

    // Delete tnx.
    dru_tnx_delete_tnx($vars);

    if ($vars['entity']->tnx) {
      // Count tnx.
      dru_tnx_counting_tnx_statistic($vars, 0);
      // Count user tnx.
      dru_tnx_counting_user_statistic($vars, 0);
    }
  }

  // Write tnx count to entity.
  $query = db_update($vars['type']);
  $query->condition($vars['id_type'], $vars['id'])
    ->fields(['tnx' => $tnx_count])
    ->execute();
}
