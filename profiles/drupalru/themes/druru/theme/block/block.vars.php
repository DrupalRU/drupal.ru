<?php
/**
 * @file
 * block.vars.php
 */

/**
 * Implements hook_preprocess_block().
 */
function druru_preprocess_block(&$variables) {
  $block = &$variables['block'];
  // Use a bare template for the page's main content.
  if ($variables['block_html_id'] == 'block-system-main') {
    $variables['theme_hook_suggestions'][] = 'block__no_wrapper';
  }
  $variables['title_attributes_array']['class'][] = 'block-title';

  $additional_preprocess = 'druru_preprocess_block__';
  $additional_preprocess .= $block->module;
  $additional_preprocess .= '__';
  $additional_preprocess .= $block->delta;

  $additional_preprocess = str_replace('-', '_', $additional_preprocess);

  if (function_exists($additional_preprocess)) {
    $additional_preprocess($variables);
  }
}

/**
 * Add icon "plus" for quickly adding node at the forum.
 */
function druru_preprocess_block__drurum__new(&$variables) {
  $variables['block']->subject .= l(druru_icon('plus'), 'node/add/blog', array(
    'html'       => TRUE,
    'attributes' => array(
      'class' => array('inline-action'),
    ),
  ));
}

/**
 * Implements hook_process_block().
 */
function druru_process_block(&$variables) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $variables['title'] = $variables['block']->subject;
}

function druru_preprocess_block__views__events_upcoming_block_block(&$variables) {
  $variables['block_html_id'] = 'events--front';
  $variables['classes_array'] = ['events--front'];
}
