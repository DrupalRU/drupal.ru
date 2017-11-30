<?php

/**
 * @file
 * Contain list of helpers for the module.
 */

/**
 * Add one more record about user activity.
 *
 * @param int $uid
 * @param int $id
 * @param int $type
 *
 * @return bool|int
 */
function acal_collate($uid, $id, $type, $date = NULL) {
  $record = [
    'uid' => $uid,
    'id' => $id,
    'type' => $type,
    'date' => acal_date($date),
  ];

  return drupal_write_record('acal', $record);
}

/**
 * Generates current date of needed format.
 *
 * @return false|string
 *   Date like `20171121`.
 */
function acal_date($date = NULL) {
  return $date ? format_date($date, 'custom', 'Ymd') : date('Ymd');
}

/**
 * Convert date, saved in db and return in format like '2017-11-21'.
 *
 * @param string|int $date
 *
 * @return string
 */
function acal_convert_date($date) {
  return mb_substr($date, 0, 4) . '-' . mb_substr($date, 4, 2) . '-' . mb_substr($date, 6, 2);
}

/**
 * Returns id of passed entity.
 *
 * @param object $entity
 * @param string $type
 *
 * @return null|int
 *   Entity id.
 */
function acal_get_id($entity, $type) {
  $id = NULL;
  switch (TRUE) {

    // Comment.
    case $type == 'comment':
      $id = $entity->cid;
      break;

    // Node.
    case $type == 'node':
      $id = $entity->nid;
      break;
  }

  return $id;
}

/**
 * Returns converted entity.
 *
 * @param string|int $type
 *
 * @return bool|string|int
 */
function acal_type_convert($type) {
  $types = [
    'node' => ACAL_NODE,
    'comment' => ACAL_COMMENT,
    ACAL_NODE => 'node',
    ACAL_COMMENT => 'comment',
  ];

  return isset($types[$type]) ? $types[$type] : FALSE;
}

/**
 * Checks access to collect activity.
 *
 * @param object $entity
 * @param string $type
 *
 * @return bool
 *   TRUE if activity can be collected, FAlSE otherwise.
 */
function acal_access($entity, $type) {
  return acal_type_convert($type)
    && variable_get('acal_' . $type)
    && user_access('acal collect statistic')
    && variable_get('acal_node_type_' . acal_bundle($entity, $type));
}

/**
 * Returns node bundle of the entity, even if the entity is comment.
 *
 * @param object $entity
 * @param string $type
 *
 * @return string
 */
function acal_bundle($entity, $type) {
  return node_load($entity->nid)->type;
}

/**
 * Performs calculation activity by internal formula.
 *
 * @param $stat
 */
function _acal_calculate_activity(&$stat) {
  $stat->activity *= variable_get('acal_' . acal_type_convert($stat->type) . '_coeff');
}

/**
 * Returns sum of calculated activity aggregated by days.
 *
 * @param $stats
 */
function _acal_activity_sum(&$stats) {
  $new_stats = [];
  foreach ($stats as $stat) {
    $day = acal_convert_date($stat->date);
    if (!isset($new_stats[$day])) {
      $new_stats[$day] = [$day, 0];
    }
    $new_stats[$day][1] += $stat->activity;
  }
  $stats = $new_stats;
  unset($new_stats);
}
