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

  if (function_exists($additional_preprocess)) {
    $additional_preprocess($variables);
  }
}

/**
 * Add icon "plus" for quickly adding node at the forum.
 */
function druru_preprocess_block__drurum__new(&$variables) {
  // No need to show the "Plus" icon at page of creating "Blog".
  $allowed_path = !preg_match('/node\/add\/blog(\/\d)?/', $_GET['q']);
  if ($allowed_path) {
    $variables['block']->subject .= l(druru_icon('plus'), 'node/add/blog', array(
      'html'       => TRUE,
      'attributes' => array(
        'class' => array('pull-right', drupal_is_front_page() ?: 'text-muted'),
      ),
    ));
  }
}

/**
 * Add icon "plus" for quickly adding node at the forum.
 */
function druru_preprocess_block__simple_events__upcoming_events(&$variables) {
  $elements = &$variables['elements'];
  if (!isset($elements['links'])) {
    return;
  }
  $elements['links']['#attributes']['class'][] = 'list-inline';
  if (!empty($elements['links']['#links'])) {
    $links = &$elements['links']['#links'];
    if (!empty($links['list']['title'])) {
      $title = druru_icon('list', FALSE, array(
        'class' => array('text-accent'),
      ));
      $links['list']['title'] = $title . t('More');
      $links['list']['html'] = TRUE;
    }
    // The link should be available always.
    if (!isset($links['add'])) {
      $links = array(
          'add' => array(
            'title' => t('Create'),
            'href'  => 'node/add/simple-event',
            'html'  => TRUE,
          ),
        ) + $links;
    }
    $title = druru_icon('plus', FALSE, array(
      'class' => array('text-accent'),
    ));
    $links['add']['title'] = $title . t('Create');
    $links['add']['html'] = TRUE;
  }
}

/**
 * Implements hook_process_block().
 */
function druru_process_block(&$variables) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $variables['title'] = $variables['block']->subject;
}
