<?php

function druru_preprocess_comment(&$vars) {
  $comment    = $vars['elements']['#comment'];
  $node       = $vars['elements']['#node'];
  $links      = array();
  $link_attrs = array();
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

  // Check published status
  if ($vars['status'] == 'comment-unpublished') {
    $vars['unpublished'] = ' <span class="unpublished-item">';
    $vars['unpublished'] .= druru_get_icon_by_title(t('Unpublished')) . t('Unpublished');
    $vars['unpublished'] .= '</span>';
  }

  // Delete class 'inline" and
  // "list-inline" (will added automatically) from links.
  // @todo Refactor to use unified names for css classes
  if (!empty($link_attrs['class'])) {
    $key = array_search('inline', $link_attrs['class']);
    if ($key !== FALSE) {
      unset($link_attrs['class'][$key]);
    }
  }

  $vars['classes_array'][] = 'comment--' . $view_mode;
  $vars['classes_array'][] = 'is-view-entity';

  $vars['content_attributes_array']['class'] = 'comment__content';
  $vars['content']['links']['#attributes']['class'] = [];
  $vars['content']['links']['#attributes']['class'][] = 'comment__menu';

  // @todo Reimplement 'claim' with module 'flag'
  _druru_wrap_claim($content, 'comment', $vars['id']);
  // @todo Refactor option 1. We need variable '$tnx' like other comment variables.
  // @todo Refactor option 2. Migrate to module 'flag' (preferred).
  _druru_wrap_thanks($vars, 'comment');
}

function _druru_links_access($content) {
  return (!empty($content['links']) && empty($content['links']['#printed']) && (
        !isset($content['links']['#access']) || $content['links']['#access'])
    ) && $GLOBALS['user']->uid;
}
