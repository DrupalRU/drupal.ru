<?php

/**
 * @file
 * Hooks provided by the friendly_register module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allows you to provide your own validation routines for email addresses
 * beyond the valid_email_address() call.
 *
 * @param string $address
 *
 * @return bool
 */
function hook_validate_email_address($address) {
  return TRUE;
}

/**
 * Allows you to provide your own validation routines for the user name.
 *
 * @param string $username
 *
 * @return bool
 */
function hook_validate_user_name($username) {
  return TRUE;
}

/**
 * @} End of "addtogroup hooks".
 */
