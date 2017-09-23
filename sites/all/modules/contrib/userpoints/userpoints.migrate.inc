<?php

/**
 * @file
 * Migration class for userpoint transactions.
 */

/**
 * Destination class for migrating userpoints.
 */
class MigrateDestinationUserpoints extends MigrateDestination {

  public function __toString() {
    return t('userpoints');
  }

  static public function getKeySchema() {
    return array(
      'txn_id' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
    );
  }

  /**
   * Import a single userpoints transaction.
   *
   * @param stdClass $userpoints
   * @param stdClass $row
   * @return array|bool
   */
  function import(stdClass $userpoints, stdClass $row) {
    // If updating previously-migrated content.
    if (isset($row->migrate_map_destid1)) {
      $updating = TRUE;
      if (isset($userpoints->txn_id)) {
        if ($userpoints->txn_id != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming id !id and map destination id !destid don't match.",
              array('!id' => $userpoints->txn_id, '!destid' => $row->migrate_map_destid1)
            ));
        }
      }
      else {
        $userpoints->txn_id = $row->migrate_map_destid1;
      }
    }
    else {
      $updating = FALSE;
      // Clear out the txn_id, just to be safe.
      unset($userpoints->txn_id);
    }

    // Build params.
    $params = array();
    $fields = $this->fields();
    foreach (array_keys($fields) as $key) {
      if (isset($userpoints->$key)) {
        $params[$key] = $userpoints->$key;
      }
    }

    // Call userpoints api to write the entry.
    $result = userpoints_userpointsapi($params);
    if ($result['status'] == TRUE && isset($result['transaction'])) {
      $txn_id = $result['transaction']['txn_id'];

      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }

      // Call any complete handlers.
      $this->complete($userpoints, $row);

      return array($txn_id);
    }

    if (isset($result['reason'])) {
      watchdog('userpoints_migrate', $result['reason'], array(), WATCHDOG_WARNING);
    }

    return FALSE;
  }

  /**
   * Delete the provided userpoint transaction.
   *
   * @param $id
   *  Primary key values.
   */
  public function rollback(array $id) {
    migrate_instrument_start('userpoints_txn rollback');

    $txn_id = reset($id);
    $txn = userpoints_transaction_load($txn_id);

    // We set txn to expired and status to declined and send it to the recache,
    // before we delete the entry.
    // This is necessary to decrease the user's total points.
    $del_txn = (array) $txn;
    $del_txn['expired'] = 1;
    $del_txn['status'] = USERPOINTS_TXN_STATUS_DECLINED;
    _userpoints_update_cache($del_txn, (array) $txn);

    // As there is no API function to delete, we must delete the row ourselves.
    db_delete('userpoints_txn')
      ->condition('txn_id', $txn_id)
      ->execute();

    migrate_instrument_stop('userpoints_txn rollback');
  }

  /**
   * Returns a list of fields available to be mapped/
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  function fields() {
    return array(
      'txn_id' => 'Transaction ID of points (if present an update is performed)',
      'points' => t('Number of points (int) (required)'),
      'moderate' => t('Moderation status. (TRUE, FALSE or empty)'),
      'uid' => 'User ID',
      'time_stamp' => 'unix time of the points assignment date',
      'operation' => t('Name for moderation operation (e.g. "published", "moderated", etc.)'),
      'tid' => t('Userpints category ID'),
      'expirydate' => t('Expiration date: timestamp or 0, 0 = non-expiring; NULL = site default'),
      'description' => t('Description'),
      'reference' => t('reserved for module specific use'),
      'display' => t('Whether or not to display "points awarded" message'),
      'entity_id' => 'ID of an entity in the Database. ex. $node->id or $user->uid',
      'entity_type' => 'string of the entity type. ex. "node" or "user"',
    );
  }

  /**
   * Give handlers a shot at modifying the object before saving it.
   *
   * @param $entity
   *  Entity object to build. Prefilled with any fields mapped in the Migration.
   * @param $source_row
   *  Raw source data object - passed through to prepare handlers.
   */
  public function prepare(stdClass $entity, stdClass $source_row) {
    $migration = Migration::currentMigration();
    $entity->migrate = array(
      'machineName' => $migration->getMachineName(),
    );

    // Call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($entity, $source_row);
    }
  }

  /**
   * Give handlers a shot at modifying the object (or taking additional action)
   * after saving it.
   *
   * @param $object
   *  Entity object to build. This is the complete object after saving.
   * @param $source_row
   *  Raw source data object - passed through to complete handlers.
   */
  public function complete(stdClass $entity, stdClass $source_row) {
    $migration = Migration::currentMigration();

    // Call any complete handler for this specific Migration.
    if (method_exists($migration, 'complete')) {
      $migration->complete($entity, $source_row);
    }
  }
}
