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
      $sub_menu = '<ul class="dropdown-menu">' . drupal_render($element['#below']) . '</ul>';
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
      $element['#title'] = druru_icon('user') . ucfirst($GLOBALS['user']->name);
      $element['#localized_options']['html'] = TRUE;
      // For notifications in the future.
//      if ($has_notifications) {
//        $element['#title'] .= ' <sup>';
//        $element['#title'] .= druru_icon('asterisk', NULL, array(
//          'class' => array('text-accent'),
//        ));
//        $element['#title'] .= '</sup> ';
//      }
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
  }
}

function _druru_menu_link_fill_icon(&$element, $icon) {
  if ($icon = druru_icon($icon)) {
    $title = $element['#title'];
    $element['#title'] = $icon;
    $element['#title'] .= ' ';
    $element['#title'] .= '<span class="visible-xs-inline-block">';
    $element['#title'] .= $title;
    $element['#title'] .= '</span>';
    $element['#localized_options']['html'] = TRUE;
  }
}
