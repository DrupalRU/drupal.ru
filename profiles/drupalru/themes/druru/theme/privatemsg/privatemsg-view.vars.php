<?php

/**
 * Implements hook_preprocess_privatemsg_view().
 */
function druru_preprocess_privatemsg_view(&$vars) {
  $message = $vars['message'];
  static $changed_author = 0, $author_place = NULL;
  $vars['is_current_user'] = $GLOBALS['user']->uid == $message->author->uid;
  $vars['show_picture'] = $changed_author != $message->author->uid;
  $changed_author = $message->author->uid;

  $vars['message_classes'][] = 'media';
  if ($vars['is_current_user']) {
    $vars['message_classes'][] = "current-user";
  }
  if ($vars['show_picture']) {
    $vars['message_classes'][] = "first-of-author-stack";
  }

  $vars['author_place'] = NULL;
  if ($vars['author_picture']) {
    if (!$vars['is_current_user']) {
      $vars['author_place'] = $author_place = 'left';
    }
    else {
      $vars['author_place'] = $author_place = 'right';
    }
  }
  if (!$vars['author_place']) {
    $vars['author_place'] = $author_place;
  }
}
