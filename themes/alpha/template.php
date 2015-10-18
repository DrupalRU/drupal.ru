<?php

/**
 * @file
 * template.php
 */

/**
 * Process variables for user-picture.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $account: A user, node or comment object with 'name', 'uid' and 'picture'
 *   fields.
 *
 * @see user-picture.tpl.php
 */
function alpha_preprocess_user_picture(&$variables) {
  print_r($variables);
}
