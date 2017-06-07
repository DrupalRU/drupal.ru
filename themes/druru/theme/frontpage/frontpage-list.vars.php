<?php

/**
 *
 */
function druru_preprocess_frontpage_list(&$variables) {
  if (!empty($variables['content']['links'])) {
    $links = &$variables['content']['links'];
    foreach ($links['#links'] as &$link) {
      $new_link['#theme'] = 'link';
      $new_link['#text'] = $link['title'];
      $new_link['#path'] = $link['href'];
      $new_link['#options']['attributes']['class'][] = 'btn';
      $new_link['#options']['attributes']['class'][] = 'btn-link';
      $new_link['#options']['html'] = TRUE;
      $link = $new_link;
    }
    $links += $links['#links'];
    unset($new_link, $link, $links['#theme'], $links['#links']);

    $icon = druru_icon('list', FALSE, array('class' => array('text-accent')));
    $links['list']['#text'] = $icon . ' ' . t('More');
  }
}
