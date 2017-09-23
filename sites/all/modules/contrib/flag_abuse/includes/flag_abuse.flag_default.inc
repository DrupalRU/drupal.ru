<?php

function _flag_abuse_abuse_node_flags(&$flags) {
  // Exported flag: "Node Abuse".
  $flags['abuse_node'] = array(
    flag_abuse_get_api_type() => 'node',
    'title' => 'Node Abuse',
    'global' => 0,
    'types' => array(
    ),
    'flag_short' => 'Flag as offensive',
    'flag_long' => 'Flag this content as offensive.',
    'flag_message' => '',
    'unflag_short' => 'Flag as non-offensive',
    'unflag_long' => 'Flag this content as non-offensive.',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'confirm',
    'roles' => array(
      'flag' => array(
        0 => '2',
      ),
      'unflag' => array(
        0 => '2',
      ),
    ),
    'weight' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_page' => 1,
    'show_on_teaser' => 0,
    'show_contextual_link' => FALSE,
    'i18n' => 0,
    'flag_confirmation' => 'Are you sure you want to flag this content as offensive?',
    'unflag_confirmation' => 'Are you sure you want to flag this content as non-offensive?',
    'module' => 'flag_abuse',
    'locked' => array(
      'name' => 'name',
      'global' => 'global',
    ),
    'api_version' => FLAG_API_VERSION,
  );
  // Exported flag: "Whitelist Node".
  $flags['abuse_whitelist_node'] = array(
    flag_abuse_get_api_type() => 'node',
    'title' => 'Node Whitelist',
    'global' => 0,
    'types' => array(
    ),
    'flag_short' => 'Whitelist this content.',
    'flag_long' => 'Remove abuse flags from this node, whitelist.',
    'flag_message' => '',
    'unflag_short' => 'Un-whitelist this content.',
    'unflag_long' => 'Allow abuse reports on this node, un-whitelist',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'confirm',
    'roles' => array(
      'flag' => array(
        0 => '3',
      ),
      'unflag' => array(
        0 => '3',
      ),
    ),
    'weight' => 1,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_page' => 1,
    'show_on_teaser' => 0,
    'show_contextual_link' => FALSE,
    'i18n' => 0,
    'flag_confirmation' => 'Are you sure you want to whitelist this content?',
    'unflag_confirmation' => 'Are you sure you want to un-whitelist this content?',
    'module' => 'flag_abuse',
    'locked' => array(
      'name' => 'name',
      'global' => 'global',
    ),
    'api_version' => FLAG_API_VERSION,
  );
  return $flags;
}

function _flag_abuse_abuse_comment_flags(&$flags) {
  // Exported flag: "Comment Abuse".
  $flags['abuse_comment'] = array(
    flag_abuse_get_api_type() => 'comment',
    'title' => 'Comment Abuse',
    'global' => 0,
    'types' => array(
    ),
    'flag_short' => 'Flag as offensive',
    'flag_long' => 'Flag this content as offensive.',
    'flag_message' => '',
    'unflag_short' => 'Flag as non-offensive',
    'unflag_long' => 'Flag this content as non-offensive.',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'confirm',
    'roles' => array(
      'flag' => array(
        0 => '2',
      ),
      'unflag' => array(
        0 => '2',
      ),
    ),
    'weight' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_comment' => 1,
    'flag_confirmation' => 'Are you sure you want to flag this content as offensive?',
    'unflag_confirmation' => 'Are you sure you want to flag this content as non-offensive?',
    'module' => 'flag_abuse',
    'locked' => array(
      'name' => 'name',
      'global' => 'global',
    ),
    'api_version' => FLAG_API_VERSION,
  );
  // Add whitelist flag
  $flags['abuse_whitelist_comment'] = array(
    flag_abuse_get_api_type() => 'comment',
    'title' => 'Comment Whitelist',
    'global' => 0,
    'types' => array(
    ),
    'flag_short' => 'Whitelist this comment.',
    'flag_long' => 'Add this content to a whitelist, remove abuse reports..',
    'flag_message' => '',
    'unflag_short' => 'Un-whitelist this comment.',
    'unflag_long' => 'Un-whitelist, allow users to mark as offensive..',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'confirm',
    'roles' => array(
      'flag' => array(
        0 => '3',
      ),
      'unflag' => array(
        0 => '3',
      ),
    ),
    'weight' => 1,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_comment' => 1,
    'flag_confirmation' => 'Are you sure you want to whitelist this content?',
    'unflag_confirmation' => 'Are you sure you want to un-whitelist this content?',
    'module' => 'flag_abuse',
    'locked' => array(
      'name' => 'name',
      'global' => 'global',
    ),
    'api_version' => FLAG_API_VERSION,
  );

return $flags;

}

function _flag_abuse_abuse_user_flags(&$flags) {
  // Exported flag: "User Abuse".
  $flags['abuse_user'] = array(
    flag_abuse_get_api_type() => 'user',
    'title' => 'User Abuse',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'Report User',
    'flag_long' => 'Report this user as offensive.',
    'flag_message' => '',
    'unflag_short' => 'Remove Report',
    'unflag_long' => 'Remove your report about this user.',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'confirm',
    'roles' => array(
      'flag' => array(
        0 => '2',
      ),
      'unflag' => array(
        0 => '2',
      ),
    ),
    'weight' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_profile' => 1,
    'access_uid' => '',
    'flag_confirmation' => 'Are you sure you want to report this user as offensive?',
    'unflag_confirmation' => 'Are you sure you want to remove your report of this user?',
    'module' => 'flag_abuse',
    'locked' => array(
      'name' => 'name',
      'global' => 'global',
    ),
    'api_version' => FLAG_API_VERSION,
  );
  // Add whitelisting flag for users
  $flags['abuse_whitelist_user'] = array(
    flag_abuse_get_api_type() => 'user',
    'title' => 'User Whitelist',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'Whitelist User',
    'flag_long' => 'Whitelist this user and remove abuse reporting.',
    'flag_message' => '',
    'unflag_short' => 'Un-whitelist User',
    'unflag_long' => 'Remove whitelist from this user, allow abuse reports about them.',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'confirm',
    'roles' => array(
      'flag' => array(
        0 => '3',
      ),
      'unflag' => array(
        0 => '3',
      ),
    ),
    'weight' => 1,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_profile' => 1,
    'access_uid' => '',
    'flag_confirmation' => 'Are you sure you want to whitelist this user?',
    'unflag_confirmation' => 'Are you sure you want to un-whitelist this user?',
    'module' => 'flag_abuse',
    'locked' => array(
      'name' => 'name',
      'global' => 'global',
    ),
    'api_version' => FLAG_API_VERSION,
  );
  return $flags;
}
