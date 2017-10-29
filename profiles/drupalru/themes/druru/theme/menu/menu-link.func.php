<?php
/**
 * @file
 * menu-link.func.php
 */

/**
 * Overrides theme_menu_link().
 */
function druru_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  _druru_menu_link_fill_icons($element);
  if ($element['#below']) {
    // Prevent dropdown functions from being added to management menu so it
    // does not affect the navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
      // Add our own wrapper.
      unset($element['#below']['#theme_wrappers']);
      $second_calss = '';
      if($element['#theme'] == 'menu_link__user_menu') {
        $second_calss = 'dropdown-menu-right';
      }
        $sub_menu = "<ul class=\"dropdown-menu $second_calss\">" . drupal_render($element['#below']) . '</ul>';
      // Generate as standard dropdown.
      $element['#title'] .= ' <span class="caret"></span>';
      $element['#attributes']['class'][] = 'dropdown';
      $element['#localized_options']['html'] = TRUE;

      // Set dropdown trigger element to # to prevent inadvertant page loading
      // when a submenu link is clicked.
      $element['#localized_options']['attributes']['data-target'] = '#';
      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
      $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';
    }
  }
  // On primary navigation menu, class 'active' is not set on active menu item.
  // @see https://drupal.org/node/1896674
  $is_front_page = ($element['#href'] == '<front>' && drupal_is_front_page());
  $is_current_link = ($element['#href'] == $_GET['q'] || $is_front_page);
  if ($is_current_link && empty($element['#localized_options']['language'])) {
    $element['#attributes']['class'][] = 'active';
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  $output = '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";

  // Delete last symbol "\n",
  // because we have css for 'li' tag: "display: inline-block",
  // but it symbol will be created space between items.
  return substr($output, 0, -1);
}

function _druru_menu_link_fill_icons(&$element) {
  if (empty($element['#localized_options']['attributes']['title'])) {
    $element['#localized_options']['attributes']['title'] = $element['#title'];
  }
  switch ($element['#href']) {
    case 'user':
      $url = entity_uri('user', $GLOBALS['user']);
      if ($url['path'] == current_path()) {
        $element['#localized_options']['attributes']['class'][] = 'active';
      }

      $element['#weight'] = 1000;
      $icon = _druru_user_menu_icon();
      $element['#title'] = $icon . ' ' . ucfirst($GLOBALS['user']->name);
      $element['#localized_options']['html'] = TRUE;
      break;

    case 'messages':
      $element['#title'] = t($element['#original_link']['link_title']);
      _druru_menu_link_fill_icon($element, 'envelope');

      // If we have enabled module "Privatemsg" Then we need to check
      // and show label if we have unread messages.
      if (_druru_count_unread_messages()) {
        $element['#title'] .= ' <sup class="label label-success">';
        $element['#title'] .= _druru_count_unread_messages();
        $element['#title'] .= '</sup>';
      }

      $element['#weight'] = -1000;
      break;

    case 'user/logout':
      _druru_menu_link_fill_icon($element, 'sign-out');
      break;

    case 'user/login':
      _druru_menu_link_fill_icon($element, 'sign-in');
      break;

    case 'user/register':
      _druru_menu_link_fill_icon($element, 'user-plus');
      break;

    case 'node/add':
      _druru_menu_link_fill_icon($element, 'plus');
      break;

    default:
      if ($element['#original_link']['menu_name'] == 'menu-social-links') {
        if ($icon = druru_search_icon_key($element['#title'])) {
          _druru_menu_link_fill_icon($element, $icon, 'delete');
        }
      }
      break;
  }

}

/**
 * Saturate link with icon.
 *
 * @param array $element
 *   Menu item
 * @param $icon
 *   Icon key
 * @param string $text
 *   Indicates what's need to do with text: "hide" from desktop devices
 *     or delete from all devices (any other string)
 */
function _druru_menu_link_fill_icon(&$element, $icon_key, $text = 'hide') {
  if ($icon = druru_icon($icon_key)) {
    $title = $element['#title'];
    $element['#title'] = $icon;
    if ($text == 'hide') {
      $element['#title'] .= ' ';
      $element['#title'] .= '<span class="visible-xs-inline-block">';
      $element['#title'] .= $title;
      $element['#title'] .= '</span>';
    }
    $element['#localized_options']['attributes']['class'][] = 'type-' . $icon_key;
    $element['#localized_options']['html'] = TRUE;
  }
}

/**
 * Returns avatar of the user or default image if the avatar is not set.
 * If the default avatar is not set also, then will returned icon of user.
 *
 * @return string
 */
function _druru_user_menu_icon() {
  if (module_exists('image')) {
    $account = $GLOBALS['user'];
    if (!empty($account->picture)) {
      if (is_numeric($account->picture)) {
        $account->picture = file_load($account->picture);
      }
      if (!empty($account->picture->uri)) {
        $filepath = $account->picture->uri;
      }
    }
  }

  if (!empty($filepath)) {
    $icon = theme('image_style', array(
      'style_name' => 'avatar_menu',
      'path' => $filepath,
      'alt' => 'user-icon',
    ));
  }
  else {
    $icon = druru_icon('user');
  }
  return $icon;
}