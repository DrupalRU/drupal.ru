<?php

/**
 * Implements template_preprocess_theme() for theme_marketplace_random_block().
 */
function druru_preprocess_marketplace_random_block(&$variables) {
  if (!isset($variables['links'])) {
    return;
  }
  $variables['links']['#attributes']['class'][] = 'list-inline';
  if (!empty($variables['links']['#links'])) {
    $links = &$variables['links']['#links'];
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
            'href'  => 'node/add/organization',
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
