<?php


/**
 * Define default flags.
 */
function _drurum_flag_default_flags() {
  $flags = [];
  $flags[DRURUM_FORUM_TOPIC_SUBSCRIBE_FLAG] = [
    'entity_type' => 'node',
    'title' => 'Forum topic subscribe',
    'global' => FALSE,
    'types' => [
      1 => 'blog',
    ],
    'flag_short' => 'Subscribe this',
    'flag_long' => 'Add forum topic to your subscribe',
    'flag_message' => 'This forum has been added to your subscribe',
    'unflag_short' => 'Unsubscribe this',
    'unflag_long' => 'Remove this forum topic from your subscribes',
    'unflag_message' => 'This forum has been removed from your subscribes',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => [
      'full' => TRUE,
      'token' => FALSE,
    ],
    'show_as_field' => FALSE,
    'show_on_form' => FALSE,
    'access_author' => '',
    'show_contextual_link' => TRUE,
    'show_on_profile' => FALSE,
    'access_uid' => '',
    'api_version' => 3,
  ];
  return $flags;
}


/**
 * Private function to send the notifications.
 *
 * @param $comment
 *   The comment array as found in hook_comment $op = publish.
 */
function _drurum_comment_notify_mailalert($comment) {
  module_load_include('inc', 'comment_notify', 'comment_notify');

  $comment = (object) $comment;
  global $language;

  $nid = $comment->nid;
  $cid = $comment->cid;

  $uids = array_keys(flag_get_entity_flags('node', $nid, DRURUM_FORUM_TOPIC_SUBSCRIBE_FLAG));

  if ($comment->uid && isset($uids[$comment->uid])) {
    unset($uids[$comment->uid]);
  }

  $node = node_load($nid);


  foreach ($uids as $uid) {
    $recipient_user = user_load($uid);

    if (!node_access('view', $node, $recipient_user)) {
      continue;
    }

    $mail = $recipient_user->mail;

    $message = [];
    $user_language = user_preferred_language($recipient_user, $language);

    $raw_values = [
      'subject' => comment_notify_variable_registry_get('watcher_subject'),
      'body' => comment_notify_variable_registry_get('comment_notify_default_mailtext'),
    ];

    foreach ($raw_values as $k => $v) {
      $message[$k] = token_replace(t($v), [
        'comment' => $comment,

      ], ['sanitize' => FALSE]);
    }

    drupal_mail('comment_notify', 'comment_notify_mail', $mail, $user_language, $message);


    $user_mail = check_plain($mail);

    // Add an entry to the watchdog log.
    watchdog(
      'comment_notify',
      'Notified: @user_mail',
      ['@user_mail' => $user_mail],
      WATCHDOG_NOTICE,
      l(t('source comment'), 'node/' . $nid, [
        'fragment' => 'comment-' . $cid,
      ])
    );
  }
}