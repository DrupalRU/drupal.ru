<?php

function druru_preprocess_comment(&$vars) {
  $comment    = $vars['elements']['#comment'];
  $node       = $vars['elements']['#node'];
  $links      = array();
  $content    = &$vars['content'];
  $view_mode  = $vars['elements']['#view_mode'];

  $vars['changed'] = _druru_format_date_aging($comment->changed);

  $vars['attributes_array']['data-comment-id'] = $vars['comment']->cid;

  // Remove author avatar and name in comment teasers
  $vars['show_author'] = ($view_mode == 'teaser') ? FALSE : TRUE;

  $uri = entity_uri('comment', $comment);
  $uri['options'] += array(
    'html' => TRUE,
    'attributes' => array(
      'class' => array(
        'permalink',
      ),
      'rel' => 'bookmark',
    ),
  );

  $vars['title'] = l($node->title, $uri['path'], $uri['options']);
  $vars['permalink'] = l(druru_icon('anchor'), $uri['path'], $uri['options']);
  $vars['timeago']   = t('@time ago', array(
    '@time' => format_interval(time() - $vars['comment']->changed, 1),
  ));

  if ($comment->status == COMMENT_NOT_PUBLISHED) {
    $vars['classes_array'][] = 'is-unpublished';
  }

  if ($vars['new']) {
    $vars['classes_array'][] = 'is-new';
  }

  if (!$comment->uid) {
    $vars['classes_array'][] = 'by-anonymous';
  }
  else {
    if ($comment->uid == $vars['node']->uid) {
      $vars['classes_array'][] = 'by-node-author';
    }
    if ($comment->uid == $vars['user']->uid) {
      $vars['classes_array'][] = 'by-viewer';
    }
  }

  if (!empty($comment->best)) {
    $vars['classes_array'][] = 'is-accepted-answer';
  }

  $vars['classes_array'][] = 'comment--' . $view_mode;
  $vars['classes_array'][] = 'is-view-entity';

  $vars['content_attributes_array']['class'] = 'comment__content';
  $vars['content']['links']['#attributes']['class'] = [];
  $vars['content']['links']['#attributes']['class'][] = 'comment__menu';

  // @todo Reimplement 'claim' with module 'flag'
  _druru_wrap_claim($content, 'comment', $vars['id']);
  // @todo Refactor option 1. Create variable '$tnx' like other comment variables.
  // @todo Refactor option 2. Migrate to module 'flag' (preferred).
  _druru_wrap_thanks($vars, 'comment');
}

function _druru_links_access($content) {
  return (!empty($content['links']) && empty($content['links']['#printed']) && (
        !isset($content['links']['#access']) || $content['links']['#access'])
    ) && $GLOBALS['user']->uid;
}
