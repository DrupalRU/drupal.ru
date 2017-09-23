<?php
/**
 * @file
 * menu-local-action.func.php
 */

/**
 * Overrides theme_menu_local_action().
 */
function druru_menu_local_action($variables) {
  $link = $variables['element']['#link'];

  $options_key = 'localized_options';
  $options     = isset($link[$options_key]) ? $link[$options_key] : array();

  // If the title is not HTML, sanitize it.
  if (empty($options['html'])) {
    $link['title'] = check_plain($link['title']);
  }

  $icon = druru_get_icon_by_title($link['title']);

  // Format the action link.
  $output = '<li>';
  if (isset($link['href'])) {
    $options['attributes']['class'][] = 'btn';
    // Turn link into a mini-button and colorize based on title.
    if ($class = _druru_colorize_button($link['title'])) {
      if (!isset($options['attributes']['class'])) {
        $options['attributes']['class'] = array();
      }
      $string = is_string($options['attributes']['class']);
      if ($string) {
        $options['attributes']['class'] = explode(' ', $options['attributes']['class']);
      }
      $options['attributes']['class'][] = $class;
      if ($string) {
        $options['attributes']['class'] = implode(' ', $options['attributes']['class']);
      }
    }
    // Force HTML so we can add the icon rendering element.
    $options['html'] = TRUE;
    $output .= l($icon . $link['title'], $link['href'], $options);
  }
  else {
    $output .= $icon . $link['title'];
  }
  $output .= "</li>\n";

  return $output;
}
