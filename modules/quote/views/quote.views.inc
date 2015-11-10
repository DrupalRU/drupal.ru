<?php

/**
 * @file
 * Provide views data and handlers for the Quote module.
 */

/**
 * Implements hook_views_data_alter().
 */
function quote_views_data_alter(&$data) {
  // Link to quote to comment.
  $data['comment']['quote_comment'] = array(
    'group' => t('Comment'),
    'title' => t('Quote link'),
    'help' => t('Provide a simple link to quote to the comment.'),
    'field' => array(
      'handler' => 'quote_handler_field_comment_link_quote'
    )
  );
}

/**
 * Implements hook_views_handlers().
 */
function quote_views_handlers() {
  return array(
    'quote_handler_field_comment_link_quote' => array(
      'parent' => 'views_handler_field_comment_link'
    )
  );
}
