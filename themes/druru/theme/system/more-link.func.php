<?php

/**
 * Returns HTML for a "more" link, like those used in blocks.
 *
 * @param $variables
 *   An associative array containing:
 *   - url: The URL of the main page.
 *   - title: A descriptive verb for the link, like 'Read more'.
 */
function druru_more_link($variables) {
  $icon = druru_icon('list', NULL, array('class' => array('text-accent')));
  $link = l($icon . t('More'), $variables['url'], array(
    'attributes' => array(
      'title' => $variables['title'],
    ),
    'html'       => TRUE,
  ));
  return '<div class="more-link">' . $link . '</div>';
}
