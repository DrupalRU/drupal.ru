<?php
/**
 * @file
 */


/**
 * User info page.
 */
function user_filter_popup_info($name) {
  $user = user_load_by_name($name);
  return user_view($user, 'user_info');
}
