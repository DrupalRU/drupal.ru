<?php
/**
 * @file
 * links.vars.php
 */

/**
 * Implements hook_preprocess_links().
 * @todo Refactor to use unified names for css classes
 * @todo Test if the first part of function's code is really required
 */
function druru_preprocess_links(&$vars) {
  $attributes = array();

  if (isset($vars['attributes'])) {
    $attributes = &$vars['attributes'];
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
    if (!empty($vars['links']) && is_array($vars['links'])) {
      foreach ($vars['links'] as &$link) {
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

      uasort($vars['links'], 'drupal_sort_weight');
    }
  }
}
