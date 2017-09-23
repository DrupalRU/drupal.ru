<?php

/**
 * @file
 * Hooks provided by the Birthdays module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Executed on birthdays.
 *
 * @param $entity
 *   The entity the birthday field is attached to. For example the user
 *   object.
 * @param $instance
 *   The field instance.
 */
function hook_birthdays($entity, $instance) {
  if (isset($entity->name)) {
    watchdog('birthdays', "It's %name's birthday.", array('%name' => $entity->name));
  }
}

/**
 * @} End of "addtogroup hooks".
 */
