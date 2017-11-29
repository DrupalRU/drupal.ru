<?php
/**
 * @file
 * common.inc
 *
 * Contains common functionality for the entire base theme.
 */

/**
 * Auto-rebuild the theme registry during theme development.
 */
if (theme_get_setting('druru_rebuild_registry') && !defined('MAINTENANCE_MODE')) {
  // Rebuild .info data.
  system_rebuild_theme_data();
  // Rebuild theme registry.
  drupal_theme_rebuild();
}

/**
 * Return information from the .info file of a theme (and possible base themes).
 *
 * @param string $theme_key
 *   The machine name of the theme.
 * @param string $key
 *   The key name of the item to return from the .info file. This value can
 *   include "][" to automatically attempt to traverse any arrays.
 * @param bool $base_themes
 *   Recursively search base themes, defaults to TRUE.
 *
 * @return string|array|false
 *   A string or array depending on the type of value and if a base theme also
 *   contains the same $key, FALSE if no $key is found.
 */
function druru_get_theme_info($theme_key = NULL, $key = NULL, $base_themes = TRUE) {
  // If no $theme_key is given, use the current theme if we can determine it.
  if (!isset($theme_key)) {
    $theme_key = !empty($GLOBALS['theme_key']) ? $GLOBALS['theme_key'] : FALSE;
  }
  if ($theme_key) {
    $themes = list_themes();
    if (!empty($themes[$theme_key])) {
      $theme = $themes[$theme_key];
      // If a key name was specified, return just that array.
      if ($key) {
        $value = FALSE;
        // Recursively add base theme values.
        if ($base_themes && isset($theme->base_themes)) {
          foreach (array_keys($theme->base_themes) as $base_theme) {
            $value = druru_get_theme_info($base_theme, $key);
          }
        }
        if (!empty($themes[$theme_key])) {
          $info = $themes[$theme_key]->info;
          // Allow array traversal.
          $keys = explode('][', $key);
          foreach ($keys as $parent) {
            if (isset($info[$parent])) {
              $info = $info[$parent];
            }
            else {
              $info = FALSE;
            }
          }
          if (is_array($value)) {
            if (!empty($info)) {
              if (!is_array($info)) {
                $info = array($info);
              }
              $value = drupal_array_merge_deep($value, $info);
            }
          }
          else {
            if (!empty($info)) {
              if (empty($value)) {
                $value = $info;
              }
              else {
                if (!is_array($value)) {
                  $value = array($value);
                }
                if (!is_array($info)) {
                  $info = array($info);
                }
                $value = drupal_array_merge_deep($value, $info);
              }
            }
          }
        }
        return $value;
      }
      // If no info $key was specified, just return the entire info array.
      return $theme->info;
    }
  }
  return FALSE;
}

/**
 * Helper function for including theme files.
 *
 * @param string $theme
 *   Name of the theme to use for base path.
 * @param string $path
 *   Path relative to $theme.
 */
function druru_include($theme, $path) {
  static $themes = array();
  if (!isset($themes[$theme])) {
    $themes[$theme] = drupal_get_path('theme', $theme);
  }
  if ($themes[$theme] && ($file = DRUPAL_ROOT . '/' . $themes[$theme] . '/' . $path) && file_exists($file)) {
    include_once $file;
  }
}

/**
 * Performs move tabs from Drupal tabs and actions links to the dropdown menu.
 */
function _druru_move_page_tabs(&$vars) {
  $links   = array();
  $menu    = array();
  $links[] = &$vars['tabs']['#primary'];
  $links[] = &$vars['tabs']['#secondary'];
  $links[] = &$vars['action_links'];

  foreach ($links as &$group) {
    _druru_fetch_links($group, $menu);
  }

  if (!isset($vars['page_dropdown_menu'])) {
    $vars['page_dropdown_menu'] = array();
  }
  $vars['page_dropdown_menu'] += $menu;
}

/**
 * Function-helper, which performs fetching titles and paths.
 *
 * Pulled out from the links into any menu (in array passed by reference
 * in the $menu variable).
 *
 * @param array $links
 *   List of links, that need to pull out.
 * @param array $menu
 *   Menu, which should contain the fetched links.
 *
 * @see _druru_move_page_tabs()
 */
function _druru_fetch_links(&$links, &$menu) {
  if (!empty($links) && is_array($links)) {
    foreach ($links as $key => &$link) {
      if (isset($link['#link'])) {
        $link = $link['#link'];
        if(!empty($link['path'])) {
          $class        = drupal_html_class($link['path']);
          $menu[$class] = $link;
          $menu[$class] += $link['localized_options'];
          unset($links[$key]);
        }
      }
    }
  }
}

/**
 * Stylize error pages.
 *
 * Users can override error pages in admin panel,
 * else we should stylize these pages.
 *
 * @param $type
 * @param $variables
 */
function _druru_error_pages_preprocess($type, &$variables) {
  // Fetch page status code
  // Drupal required add header for error pages.
  // @see drupal_deliver_html_page()
  $code = substr(drupal_get_http_header('Status'), 0, 3);
  $body = NULL;
  $default_error_text = NULL;
  if (isset($variables['page']['content']['system_main']['main']['#markup'])) {
    $body = $variables['page']['content']['system_main']['main']['#markup'];
  }

  // Check that we don't have overridden error page.
  if (403 == $code) {
    $default_error_text = t('You are not authorized to access this page.');
  }
  elseif (404 == $code) {
    $default_error_text = t('The requested page "@path" could not be found.', array('@path' => request_uri()));
  }
  $error_page_is_not_overridden = $body == $default_error_text;

  if ($code && $error_page_is_not_overridden) {
    switch ($type) {
      case 'html':
        _druru_error_class($code, $variables);
        break;

      case 'page':
        drupal_set_title('');
        $variables['page']['content'] = array(
          '#markup' => _druru_error_page($code),
        );
        break;
    }
  }
}

/**
 * Just returning html for error pages.
 *
 * @param int $code
 *
 * @return string
 */
function _druru_error_page($code) {
  $output = '';
  switch ($code) {
    case 404:
      $output .= '<div id="status-page">';
      $output .= '<h1>404</h1>';
      $output .= '<p class="title">' . t('Page not found') . '</p>';
      $output .= '<p class="description">' . t('The page you are looking for does not exist') . '</p>';
      $output .= '</div>';
      break;

    case 403:
      $output .= '<div id="status-page">';
      $output .= '<h1>403</h1>';
      $output .= '<p class="title">' . t('Access Denied') . '</p>';
      $output .= '<p class="description">' . t('You are not authorized to access this page.') . '</p>';
      $output .= '</div>';
      break;

    default:
      $output .= '<div id="status-page">';
      $output .= '<p class="title">' . t('Unexpected error') . '</p>';
      $output .= '<p class="description">' . t('The website encountered an unexpected error. Please try again later.') . '</p>';
      $output .= '</div>';
      break;
  }
  return $output;
}

/**
 * Add classes to body based on error code.
 *
 * @param $code
 * @param $variables
 */
function _druru_error_class($code, &$variables) {

  switch ($code) {
    case 404:
      $variables['classes_array'][] = 'page-not-found';
      break;

    case 403:
      $variables['classes_array'][] = 'access-denied';
      break;
  }
}

/**
 * Transfer option to front, which say "hey, please show me errors" or "hey, no
 * need show me all notices... Please, show me only warnings and errors!"
 * JS will be use the param and show or doesn't show errors of ajax requests.
 */
function druru_add_js_settings() {
  $error_level = variable_get('error_level', ERROR_REPORTING_DISPLAY_ALL);
  $settings    = array('druru_system_logging_settings' => $error_level);
  drupal_add_js($settings, 'setting');
}

/**
 * Performs building the contextual menu of the entity.
 *
 * @param string $links
 *   List of links, which should be rendered in the dropdown menu.
 * @param array $vars
 *   Variables for render.
 */
function druru_construct_contextual_menu($links, &$vars) {
  $vars['title_prefix'][] = array(
    '#theme'      => module_exists('ctools') ? 'ctools_dropdown' : 'links',
    '#links'      => $links,
    '#title'      => 'Menu',
    '#heading'    => array(
      'text'  => t('Menu'),
      'level' => 'div',
      'class' => 'menu-icon',
    ),
  );
}

/**
 * Colorize buttons based on the text value.
 *
 * @param string $text
 *   Button text to search against.
 *
 * @return string
 *   The specific button class to use or FALSE if not matched.
 */
function _druru_colorize_button($text) {
  // Text values containing these specific strings, which are matched first.
  $specific_strings = array(
    'btn-primary' => array(
      t('Download feature'),
    ),
    'btn-success' => array(
      t('Add effect'),
      t('Add and configure'),
      t('Send message'),
    ),
    'btn-info' => array(
      t('Save and add'),
      t('Add another item'),
      t('Update style'),
    ),
  );
  // Text values containing these generic strings, which are matches last.
  $generic_strings = array(
    'btn-primary' => array(
      t('Save'),
      t('Confirm'),
      t('Submit'),
      t('Search'),
    ),
    'btn-success' => array(
      t('Add'),
      t('Create'),
      t('Write'),
    ),
    'btn-warning' => array(
      t('Export'),
      t('Import'),
      t('Restore'),
      t('Rebuild'),
    ),
    'btn-info' => array(
      t('Apply'),
      t('Update'),
    ),
    'btn-danger' => array(
      t('Delete'),
      t('Remove'),
    ),
  );
  // Specific matching first.
  foreach ($specific_strings as $class => $strings) {
    foreach ($strings as $string) {
      if (strpos(drupal_strtolower($text), drupal_strtolower($string)) !== FALSE) {
        return $class;
      }
    }
  }
  // Generic matching last.
  foreach ($generic_strings as $class => $strings) {
    foreach ($strings as $string) {
      if (strpos(drupal_strtolower($text), drupal_strtolower($string)) !== FALSE) {
        return $class;
      }
    }
  }
  return FALSE;
}

// Include ability to iconize.
druru_include('druru', 'theme/icons.inc');

function _druru_count_unread_messages() {
  $unread_count = &drupal_static('druru_unread_exists', NULL);
  // If we have enabled module "Privatemsg" Then we need to check
  // and show label if we have unread messages.
  if (function_exists('privatemsg_unread_count') && !isset($unread_count)) {
    $unread_count = privatemsg_unread_count();
  }
  return $unread_count;
}


/**
 * @param $vars
 */
function _druru_wrap_thanks(&$vars, $type) {
  if (!empty($vars[$type]->tnx)) {
    $counter = dru_tnx_show($vars[$type], $type);
    $counter['counter']['#markup'] = druru_icon('heart', TRUE, [
        'class' => ['text-danger'],
      ]) . $counter['counter']['#counter'];
    $vars['tnx'] = render($counter);
    if(!empty($vars['content']['tnx']) && is_array($vars['content']['tnx'])) {
      hide($vars['content']['tnx']);
    }
  }
}

/**
 * @param $vars
 * @param $content
 */
function _druru_wrap_claim(&$content, $type,  $id) {
  // Handling claim data.
  if (!empty($content['claim']) && !empty($content['ticket'])) {
    $is_handled = empty($content['ticket']['#has_verdict']);
    // Prepare popover data.
    $ticket = &$content['ticket'];
    $metadata = &$ticket['metadata'];
    $category = $ticket['category']['content']['#markup'];
    $ticket['category']['icon'] = $content['claim']['icon'];
    $ticket['category']['content'] = $content['claim']['verdict'];

    $ticket['description']['icon']['#markup'] = druru_icon('commenting-o');
    $ticket['description']['#attributes']['class'][] = 'text-muted';

    $ticket['category']['#weight'] = -1;
    // Regenerate right icons.
    if($is_handled) {
      $ticket['category']['icon']['#markup'] = druru_icon('spinner', FALSE, array(
        'class' => array('fa-pulse')
      ));
      $ticket['category']['#attributes']['class'][] = 'text-warning';
    }
    else {
      $ticket['category']['icon']['#markup'] = druru_icon('check');
      $ticket['category']['#attributes']['class'][] = 'bg-info';
      $ticket['category']['#attributes']['class'][] = 'text-accent';
    }
    $metadata['created']['icon']['#markup'] = druru_icon('calendar');

    $ticket_link = &$metadata['ticket-link']['content'];
    $ticket_link['#options']['html'] = TRUE;
    $ticket_link['#text'] = druru_icon('eye') . $ticket_link['#text'];
    $author = &$metadata['author']['content'];
    $author['#options']['html'] = TRUE;
    $author['#text'] = druru_icon('user') . $author['#text'];

    foreach (element_children($metadata) as $child) {
      $metadata[$child]['#attributes']['class'][] = 'small-inline-text';
    }

    // Drop links.
    unset($ticket_link, $author);
    // Drop icons which moved inside links.
    unset($metadata['ticket-link']['icon'], $metadata['author']['icon']);
    unset($child, $metadata);


    // Generate popover.
    $content['ticket-popover'] = [];
    $content['ticket-popover']['icon'] = [
      '#theme' => 'html_tag',
      '#tag' => 'a',
      '#value' => druru_icon('balance-scale'),
      '#attributes' => [
        'tabindex' => 0,
        'role' => 'button',
        'data-toggle' => 'ticket-popover',
        'data-trigger' => 'manual',
        'title' => $category,
        'data-ticket-type' => $type,
        'data-ticket' => $id,
        'class' => [($is_handled ? 'text-warning' : '')]
      ],
    ];
    $content['ticket-popover']['content'] = [
      '#type' => 'container',
      '#attributes' => [
        'class' => ['ticket-content', 'hidden'],
        'data-ticket' => $id
      ],
      'ticket' => $ticket
    ];

    // Should be hidden because we moved info of status inside the ticket,
    // but we didn't drop this for use if needed.
    hide($content['claim']);
    hide($content['ticket']);
  }
}
