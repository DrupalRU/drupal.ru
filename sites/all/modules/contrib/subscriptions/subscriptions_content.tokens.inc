<?php

/**
 * @file
 * Token callbacks for the subscriptions_content module.
 */

/**
 * Implements hook_tokens().
 *
 * @param string $type
 * @param array $tokens
 * @param array $data
 * @param array $options
 *
 * @return array
 */
function subscriptions_content_tokens($type, array $tokens, array $data = array(), array $options = array()) {
  //dpm($tokens, "subscriptions_content_tokens($type) - tokens");
  //dpm($data, "subscriptions_content_tokens($type) - data");
  //dpm($options, "subscriptions_content_tokens($type) - options");
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the node.
        case 'node-html':
          $replacements[$original] = $node->nid;
          break;
      }
    }
  }

  if ($type == 'subs' && !empty($data['comment'])) {
    $comment = $data['comment'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'is-new':
          $replacements[$original] = (int) !empty($comment->_subscriptions_is_new);
          break;
        case 'is-updated':
          $replacements[$original] = (int) empty($comment->_subscriptions_is_new);
          break;
        case 'is-old':
          $replacements[$original] = 0;
          break;
        case 'is-published':
          $replacements[$original] = (int) ($comment->status == COMMENT_PUBLISHED);
          break;
        case 'has-distinct-summary':
          $replacements[$original] = 0;
          break;
        case 'view':
          $element = comment_view($comment, $data['node'], 'subscriptions', $options['language']->language);
          $replacements[$original] = drupal_render($element);
          break;
      }
    }
  }
  elseif ($type == 'subs' && !empty($data['node'])) {
    $node = $data['node'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'is-new':
          $replacements[$original] = (int) !empty($node->_subscriptions_is_new);
          break;
        case 'is-updated':
          $replacements[$original] = (int) !empty($node->_subscriptions_is_updated);
          break;
        case 'is-old':
          $replacements[$original] = (int) (empty($node->_subscriptions_is_new) && empty($node->_subscriptions_is_updated));
          break;
        case 'is-published':
          $replacements[$original] = $node->status;
          break;
        case 'has-distinct-summary':
          $repls = token_generate('node', array('summary' => '[node:summary]', 'body' => '[node:body]'), $data, $options);
          $replacements[$original] = (int) (!empty($repls['[node:summary]']) && $repls['[node:summary]'] != $repls['[node:body]']);
          break;
        case 'view':
          $element = node_view($node, 'subscriptions', $options['language']->language);
          $replacements[$original] = drupal_render($element);
          break;
      }
    }

    if (($comments_tokens = token_find_with_prefix($tokens, 'comments')) && !empty($node->_subscriptions_comments)) {
      if (empty($node->_subscriptions_comments_rendered)) {
        foreach ($node->_subscriptions_comments as $comment) {
          $node->_subscriptions_comments_rendered[] = mail_edit_format($data['template']['subscriptions_comment_body'], array('comment' => $comment, 'object_type' => 'comment') + $data, $options);
        }
      }
      $replacements += token_generate('array', $comments_tokens, array(
        'array' => (isset($node->_subscriptions_comments_rendered) ? $node->_subscriptions_comments_rendered : array()),
      ), array('sanitize' => FALSE) + $options);
    }
  }

  return $replacements;
}

