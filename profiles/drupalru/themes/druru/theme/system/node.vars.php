<?php

/**
 * Implements hook_preprocess_node().
 */
function druru_preprocess_node(&$vars) {
  $content   = &$vars['content'];
  $view_mode = $vars['view_mode'];

  $vars['date'] = _druru_format_date_aging($vars['created']);

  // @todo Refactor to set node status to .node wrapper
  if (!$vars['status']) {
    $vars['submitted'] = ' <span class="unpublished-item">';
    $vars['submitted'] .= druru_get_icon_by_title(t('Unpublished'));
    $vars['submitted'] .= '</span>';
  }

  // @todo Refactor or remove if isn't required
  if (isset($vars['content']['datetime']['#markup']) && $view_mode == 'teaser') {
    $event_time = $vars['node']->event->time_from;
    $vars['content']['datetime']['#markup'] = format_date($event_time, 'medium');
    $vars['title_suffix'] = array(
      '#type'   => 'markup',
      '#markup' => format_date($event_time, 'short'),
      '#prefix' => '<small class="event-date">',
      '#suffix' => '</small>',
    );
  }

  // Remove author avatar and name in node teasers
  $vars['show_author'] = ($view_mode == 'teaser') ? FALSE : TRUE;

  $vars['attributes_array']['data-node-id'] = $vars['node']->nid;

  // Render blocks assigned to 'content_third' region
  // (between node and comments) for node full view
  if ($vars['page'] && $content_third = block_get_blocks_by_region('content_third')) {
    $vars['content_third'] = $content_third;
  }

  if (!empty($vars['resolved'])) {
    $vars['classes_array'][] = 'has-accepted-answer';
  }

  $vars['title_attributes_array']['class'] = 'node__title';
  $vars['content_attributes_array']['class'] = 'node__content';
  $vars['content']['links']['#attributes']['class'] = [];
  $vars['content']['links']['#attributes']['class'][] = 'node__menu';

  $vars['content']['links']['blog']['#links']['blog_usernames_blog']['title'] = t('Blog');
  $vars['content']['links']['blog']['#links']['blog_usernames_blog']['href'] = '/user/' . $vars['uid'] . '/blog';

  // @todo Reimplement 'claim' with module 'flag'
  _druru_wrap_claim($content, 'node', $vars['nid']);
  // @todo Refactor option 1. We need variable '$tnx' like other node variables.
  // @todo Refactor option 2. Migrate to module 'flag' (preferred).
  _druru_wrap_thanks($vars, 'node');
}
