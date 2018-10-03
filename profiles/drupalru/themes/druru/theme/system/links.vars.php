<?php
/**
 * @file
 * links.vars.php
 */

/**
 * Implements hook_preprocess_links().
 */
function druru_preprocess_links(&$variables) {
  $attributes = array();

  if (isset($variables['attributes'])) {
    $attributes = &$variables['attributes'];
  }

  if (isset($attributes['class'])) {
    $string = is_string($attributes['class']);

    if ($string) {
      $attributes['class'] = explode(' ', $attributes['class']);

      if ($key = array_search('inline', $attributes['class'])) {
        $attributes['class'][$key] = 'list-inline';
      }

      $attributes['class'] = implode(' ', $attributes['class']);
    }
  }

  if (in_array('privatemsg-message-actions', $attributes['class'])) {
    if (!empty($variables['links']) && is_array($variables['links'])) {
      foreach ($variables['links'] as &$link) {
        switch ($link['title']) {
          case t('Delete'):
            $link['attributes']['title'] = $link['title'];
            $link['weight'] = 2;
            $link['title'] = druru_icon('times-circle');
            break;

          case t('Block'):
            $link['attributes']['title'] = $link['title'];
            $link['weight'] = 1;
            $link['title'] = druru_icon('ban');
            break;

          case t('Unblock'):
            $link['attributes']['title'] = $link['title'];
            $link['weight'] = 0;
            $link['title'] = druru_icon('check-circle');
            break;

          default:
            $link['weight'] = 0;
            break;
        }

        $link['html'] = TRUE;
      }

      uasort($variables['links'], 'drupal_sort_weight');
    }
  }
}
