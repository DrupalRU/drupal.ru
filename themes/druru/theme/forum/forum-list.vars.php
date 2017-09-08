<?php

/**
 * Preprocess for theme_forum_list().
 */
function druru_preprocess_forum_list(&$vars) {
  $forum_icons = array_map('trim', theme_get_setting('forum_icons'));
  if (isset($vars['forums']) && is_array($vars['forums']) && $forum_icons) {
    foreach ($vars['forums'] as &$forum) {
      $forum->icon = NULL;
      if (isset($forum_icons[$forum->link])) {
        $forum->icon = druru_icon($forum_icons[$forum->link]);
      }
      $forum_title = $forum->icon . $forum->name;
      $forum->linkable_title = l($forum_title, $forum->link, array(
        'html' => TRUE,
      ));

      if (isset($forum->last_post->created)) {
        $created = $forum->last_post->created;
        $forum->last_reply = format_interval(REQUEST_TIME - $created, 1);
      }
    }
  }
}
