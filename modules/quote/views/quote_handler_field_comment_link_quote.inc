<?php

/**
 * @file
 * Provides a 'quote' link for Views.
 */

/**
 * Field handler to present a link to delete a node.
 */
class quote_handler_field_comment_link_quote extends views_handler_field_comment_link {
  function access() {
    // Check for permission to reply to comments.
    return user_access('post comments');
  }

  function render_link($data, $values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('quote');
    $nid =  $this->get_value($values, 'nid');
    $cid =  $this->get_value($values, 'cid');

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "comment/reply/" . $nid . '/' . $cid . '?quote=1#comment-form';

    return $text;
  }
}
