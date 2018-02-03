<?php

/**
 * @file
 * icons.inc
 * Provide Bootstrap Glyphicons support via the Icon API module.
 * Also contain functions for work with icons.
 */

// Attach css and js to all pages.
druru_attach_icons();

/************************************************/
/*                     HOOKS                    */
/************************************************/

/**
 * Implements hook_icon_providers().
 */
function druru_icon_providers() {
  $providers['druru'] = array(
    'title' => t('Bootstrap'),
    'url'   => 'http://getbootstrap.com/components/#glyphicons',
  );
  return $providers;
}

/**
 * Implements hook_icon_bundles().
 */
function druru_icon_bundles() {
  $theme = variable_get('theme_default', $GLOBALS['theme']);
  $cdn = theme_get_setting('druru_bootstrap_cdn', $theme);
  $bundles = array();
  if ($cdn
    || (!$cdn
      && file_exists(drupal_get_path('theme', $theme)
        . '/bootstrap/fonts/glyphicons-halflings-regular.ttf'))
  ) {
    $bundles['druru'] = array(
      'render'     => 'sprite',
      'provider'   => 'druru',
      'title'      => t('Bootstrap'),
      'version'    => t('Icons by Glyphicons'),
      'variations' => array(
        'icon-white' => t('White'),
      ),
      'settings'   => array(
        'tag' => 'i',
      ),
      'icons'      => array(
        'glyphicon-adjust'                 => 'adjust',
        'glyphicon-align-center'           => 'align-center',
        'glyphicon-align-justify'          => 'align-justify',
        'glyphicon-align-left'             => 'align-left',
        'glyphicon-align-right'            => 'align-right',
        'glyphicon-arrow-down'             => 'arrow-down',
        'glyphicon-arrow-left'             => 'arrow-left',
        'glyphicon-arrow-right'            => 'arrow-right',
        'glyphicon-arrow-up'               => 'arrow-up',
        'glyphicon-asterisk'               => 'asterisk',
        'glyphicon-backward'               => 'backward',
        'glyphicon-ban-circle'             => 'ban-circle',
        'glyphicon-barcode'                => 'barcode',
        'glyphicon-bell'                   => 'bell',
        'glyphicon-bold'                   => 'bold',
        'glyphicon-book'                   => 'book',
        'glyphicon-bookmark'               => 'bookmark',
        'glyphicon-briefcase'              => 'briefcase',
        'glyphicon-bullhorn'               => 'bullhorn',
        'glyphicon-calendar'               => 'calendar',
        'glyphicon-camera'                 => 'camera',
        'glyphicon-certificate'            => 'certificate',
        'glyphicon-check'                  => 'check',
        'glyphicon-chevron-down'           => 'chevron-down',
        'glyphicon-chevron-left'           => 'chevron-left',
        'glyphicon-chevron-right'          => 'chevron-right',
        'glyphicon-chevron-up'             => 'chevron-up',
        'glyphicon-circle-arrow-down'      => 'circle-arrow-down',
        'glyphicon-circle-arrow-left'      => 'circle-arrow-left',
        'glyphicon-circle-arrow-right'     => 'circle-arrow-right',
        'glyphicon-circle-arrow-up'        => 'circle-arrow-up',
        'glyphicon-cloud'                  => 'cloud',
        'glyphicon-cloud-download'         => 'cloud-download',
        'glyphicon-cloud-upload'           => 'cloud-upload',
        'glyphicon-cog'                    => 'cog',
        'glyphicon-collapse-down'          => 'collapse-down',
        'glyphicon-collapse-up'            => 'collapse-up',
        'glyphicon-comment'                => 'comment',
        'glyphicon-compressed'             => 'compressed',
        'glyphicon-copyright-mark'         => 'copyright-mark',
        'glyphicon-credit-card'            => 'credit-card',
        'glyphicon-cutlery'                => 'cutlery',
        'glyphicon-dashboard'              => 'dashboard',
        'glyphicon-download'               => 'download',
        'glyphicon-download-alt'           => 'download-alt',
        'glyphicon-earphone'               => 'earphone',
        'glyphicon-edit'                   => 'edit',
        'glyphicon-eject'                  => 'eject',
        'glyphicon-envelope'               => 'envelope',
        'glyphicon-euro'                   => 'euro',
        'glyphicon-exclamation-sign'       => 'exclamation-sign',
        'glyphicon-expand'                 => 'expand',
        'glyphicon-export'                 => 'export',
        'glyphicon-eye-close'              => 'eye-close',
        'glyphicon-eye-open'               => 'eye-open',
        'glyphicon-facetime-video'         => 'facetime-video',
        'glyphicon-fast-backward'          => 'fast-backward',
        'glyphicon-fast-forward'           => 'fast-forward',
        'glyphicon-file'                   => 'file',
        'glyphicon-film'                   => 'film',
        'glyphicon-filter'                 => 'filter',
        'glyphicon-fire'                   => 'fire',
        'glyphicon-flag'                   => 'flag',
        'glyphicon-flash'                  => 'flash',
        'glyphicon-floppy-disk'            => 'floppy-disk',
        'glyphicon-floppy-open'            => 'floppy-open',
        'glyphicon-floppy-remove'          => 'floppy-remove',
        'glyphicon-floppy-save'            => 'floppy-save',
        'glyphicon-floppy-saved'           => 'floppy-saved',
        'glyphicon-folder-close'           => 'folder-close',
        'glyphicon-folder-open'            => 'folder-open',
        'glyphicon-font'                   => 'font',
        'glyphicon-forward'                => 'forward',
        'glyphicon-fullscreen'             => 'fullscreen',
        'glyphicon-gbp'                    => 'gbp',
        'glyphicon-gift'                   => 'gift',
        'glyphicon-glass'                  => 'glass',
        'glyphicon-globe'                  => 'globe',
        'glyphicon-hand-down'              => 'hand-down',
        'glyphicon-hand-left'              => 'hand-left',
        'glyphicon-hand-right'             => 'hand-right',
        'glyphicon-hand-up'                => 'hand-up',
        'glyphicon-hd-video'               => 'hd-video',
        'glyphicon-hdd'                    => 'hdd',
        'glyphicon-header'                 => 'header',
        'glyphicon-headphones'             => 'headphones',
        'glyphicon-heart'                  => 'heart',
        'glyphicon-heart-empty'            => 'heart-empty',
        'glyphicon-home'                   => 'home',
        'glyphicon-import'                 => 'import',
        'glyphicon-inbox'                  => 'inbox',
        'glyphicon-indent-left'            => 'indent-left',
        'glyphicon-indent-right'           => 'indent-right',
        'glyphicon-info-sign'              => 'info-sign',
        'glyphicon-italic'                 => 'italic',
        'glyphicon-leaf'                   => 'leaf',
        'glyphicon-link'                   => 'link',
        'glyphicon-list'                   => 'list',
        'glyphicon-list-alt'               => 'list-alt',
        'glyphicon-lock'                   => 'lock',
        'glyphicon-log-in'                 => 'log-in',
        'glyphicon-log-out'                => 'log-out',
        'glyphicon-magnet'                 => 'magnet',
        'glyphicon-map-marker'             => 'map-marker',
        'glyphicon-minus'                  => 'minus',
        'glyphicon-minus-sign'             => 'minus-sign',
        'glyphicon-move'                   => 'move',
        'glyphicon-music'                  => 'music',
        'glyphicon-new-window'             => 'new-window',
        'glyphicon-off'                    => 'off',
        'glyphicon-ok'                     => 'ok',
        'glyphicon-ok-circle'              => 'ok-circle',
        'glyphicon-ok-sign'                => 'ok-sign',
        'glyphicon-open'                   => 'open',
        'glyphicon-paperclip'              => 'paperclip',
        'glyphicon-pause'                  => 'pause',
        'glyphicon-pencil'                 => 'pencil',
        'glyphicon-phone'                  => 'phone',
        'glyphicon-phone-alt'              => 'phone-alt',
        'glyphicon-picture'                => 'picture',
        'glyphicon-plane'                  => 'plane',
        'glyphicon-play'                   => 'play',
        'glyphicon-play-circle'            => 'play-circle',
        'glyphicon-plus'                   => 'plus',
        'glyphicon-plus-sign'              => 'plus-sign',
        'glyphicon-print'                  => 'print',
        'glyphicon-pushpin'                => 'pushpin',
        'glyphicon-qrcode'                 => 'qrcode',
        'glyphicon-question-sign'          => 'question-sign',
        'glyphicon-random'                 => 'random',
        'glyphicon-record'                 => 'record',
        'glyphicon-refresh'                => 'refresh',
        'glyphicon-registration-mark'      => 'registration-mark',
        'glyphicon-remove'                 => 'remove',
        'glyphicon-remove-circle'          => 'remove-circle',
        'glyphicon-remove-sign'            => 'remove-sign',
        'glyphicon-repeat'                 => 'repeat',
        'glyphicon-resize-full'            => 'resize-full',
        'glyphicon-resize-horizontal'      => 'resize-horizontal',
        'glyphicon-resize-small'           => 'resize-small',
        'glyphicon-resize-vertical'        => 'resize-vertical',
        'glyphicon-retweet'                => 'retweet',
        'glyphicon-road'                   => 'road',
        'glyphicon-save'                   => 'save',
        'glyphicon-saved'                  => 'saved',
        'glyphicon-screenshot'             => 'screenshot',
        'glyphicon-sd-video'               => 'sd-video',
        'glyphicon-search'                 => 'search',
        'glyphicon-send'                   => 'send',
        'glyphicon-share'                  => 'share',
        'glyphicon-share-alt'              => 'share-alt',
        'glyphicon-shopping-cart'          => 'shopping-cart',
        'glyphicon-signal'                 => 'signal',
        'glyphicon-sort'                   => 'sort',
        'glyphicon-sort-by-alphabet'       => 'sort-by-alphabet',
        'glyphicon-sort-by-alphabet-alt'   => 'sort-by-alphabet-alt',
        'glyphicon-sort-by-attributes'     => 'sort-by-attributes',
        'glyphicon-sort-by-attributes-alt' => 'sort-by-attributes-alt',
        'glyphicon-sort-by-order'          => 'sort-by-order',
        'glyphicon-sort-by-order-alt'      => 'sort-by-order-alt',
        'glyphicon-sound-5-1'              => 'sound-5-1',
        'glyphicon-sound-6-1'              => 'sound-6-1',
        'glyphicon-sound-7-1'              => 'sound-7-1',
        'glyphicon-sound-dolby'            => 'sound-dolby',
        'glyphicon-sound-stereo'           => 'sound-stereo',
        'glyphicon-star'                   => 'star',
        'glyphicon-star-empty'             => 'star-empty',
        'glyphicon-stats'                  => 'stats',
        'glyphicon-step-backward'          => 'step-backward',
        'glyphicon-step-forward'           => 'step-forward',
        'glyphicon-stop'                   => 'stop',
        'glyphicon-subtitles'              => 'subtitles',
        'glyphicon-tag'                    => 'tag',
        'glyphicon-tags'                   => 'tags',
        'glyphicon-tasks'                  => 'tasks',
        'glyphicon-text-height'            => 'text-height',
        'glyphicon-text-width'             => 'text-width',
        'glyphicon-th'                     => 'th',
        'glyphicon-th-large'               => 'th-large',
        'glyphicon-th-list'                => 'th-list',
        'glyphicon-thumbs-down'            => 'thumbs-down',
        'glyphicon-thumbs-up'              => 'thumbs-up',
        'glyphicon-time'                   => 'time',
        'glyphicon-tint'                   => 'tint',
        'glyphicon-tower'                  => 'tower',
        'glyphicon-transfer'               => 'transfer',
        'glyphicon-trash'                  => 'trash',
        'glyphicon-tree-conifer'           => 'tree-conifer',
        'glyphicon-tree-deciduous'         => 'tree-deciduous',
        'glyphicon-unchecked'              => 'unchecked',
        'glyphicon-upload'                 => 'upload',
        'glyphicon-usd'                    => 'usd',
        'glyphicon-user'                   => 'user',
        'glyphicon-volume-down'            => 'volume-down',
        'glyphicon-volume-off'             => 'volume-off',
        'glyphicon-volume-up'              => 'volume-up',
        'glyphicon-warning-sign'           => 'warning-sign',
        'glyphicon-wrench'                 => 'wrench',
        'glyphicon-zoom-in'                => 'zoom-in',
        'glyphicon-zoom-out'               => 'zoom-out',
      ),
    );
  }
  return $bundles;
}


/************************************************/
/*               Custom functions               */
/************************************************/

/**
 * Add css file with icons to page.
 */
function druru_attach_icons() {
  static $attached = FALSE;
  if (!$attached && theme_get_setting('druru_iconize')) {
    drupal_add_css(druru_get_icons_css_path());
    $icons = druru_get_icons_library();
    drupal_add_js(array(
      'druru' => array(
        'icons' => array(
          'provider' => $icons['title'],
          'key'      => $icons['prefix'],
          'throbber' => $icons['throbber'],
        ),
      ),
    ), 'setting');
    $attached = TRUE;
  }
}

/**
 * Get path to css file with style for icons.
 *
 * @param bool|FALSE $absolute
 *   If set to true, then will returned absolute path (start from server root)
 *   or relative (start from DRUPAL_ROOT) otherwise.
 *
 * @return null|string
 *   Path of css file.
 */
function druru_get_icons_css_path($absolute = FALSE) {
  $path = NULL;
  $library = druru_get_icons_library();
  if ($library) {
    $font_path = '/css/' . $library['name'] . '.css';
    $theme_info = $GLOBALS['theme_info'];
    $themes[] = $theme_info->name;
    if (isset($theme_info->base_theme)) {
      $themes[] = $theme_info->base_theme;
    }
    foreach ($themes as $theme) {
      $path_candidate = drupal_get_path('theme', $theme) . $font_path;
      if (file_exists(DRUPAL_ROOT . '/' . $path_candidate)) {
        $path = ($absolute ? DRUPAL_ROOT . '/' : '') . $path_candidate;
      }
    }
  }
  return $path;
}

/**
 * Theme a Bootstrap Glyphicon.
 *
 * @param string $name
 *   The icon name, minus the "glyphicon-" prefix.
 * @param bool   $force
 *   Allows to create icon even if "Iconize" property is disabled.
 * @param array  $attributes
 *   Additional attributes which will rendered inside icon tag.
 *
 * @return string
 *   The icon HTML markup.
 */
function druru_icon($name, $force = FALSE, $attributes = array()) {
  $output = '';

  // If $name is stacked, then we think, that it's stacked icon.
  if (is_array($name)) {
    // Just for consistency.
    $icons = $name;
    $size = isset($icons['size']) ? $icons['size'] : NULL;
    $output .= '<i class="fa-stack ' . $size . '">';
    foreach ($icons as $item) {
      $force = !empty($item['force']);
      $attributes = !empty($item['attributes']) ? $item['attributes'] : array();
      $output .= druru_icon($item['name'], $force, $attributes);
    }
    $output .= '</i>';
    return $output;
  }

  $selected_library = theme_get_setting('druru_icons_library');
  // Break if iconize is disabled,
  // but continue if ned to force create an icon.
  if (!$force && !theme_get_setting('druru_iconize') || !$selected_library) {
    return $output;
  }

  // If the icon is not available in css,
  // then no need to create any tags without icon.
  if (!in_array($name, druru_available_icons())) {
    return $output;
  }

  // Get key of selected icons library.
  $prefix = theme_get_setting('druru_icons_library');

  // Attempt to use the Icon API module, if enabled and it generates output.
//  if (module_exists('icon')) {
//    $output = theme('icon', array(
//      'bundle' => 'druru',
//      'icon'   => $prefix . '-' . $name,
//    ));
//  }

  // Mimic the Icon API markup if module is not exists.
  if (empty($output)) {
    $attributes['class'][] = 'icon';
    $attributes['class'][] = $prefix;
    $attributes['class'][] = $prefix . '-' . $name;
    $attributes['aria-hidden'] = 'true';
    $output = ' <i' . drupal_attributes($attributes) . '></i> ';
  }
  return $output;
}

/**
 * Searches icon key by human-readdable title.
 *
 * Example.
 * If pass string 'Delete', then can be returned `trash`.
 *
 * @param string $title
 *
 * @return null|string
 */
function druru_search_icon_key($title) {
  $icon = NULL;
  if ($library = druru_get_icons_library()) {
    // Try to search exact match.
    foreach ($library['icons'] as $key => $names) {
      if (array_search($title, $names) !== FALSE) {
        $icon = $key;
        break;
      }
    }
    // Will to search occurrence otherwise.
    if (!$icon) {
      foreach ($library['icons'] as $key => $names) {
        foreach ($names as $name) {
          if (strpos(drupal_strtolower($title), drupal_strtolower($name))
            !== FALSE
          ) {
            $icon = $key;
            break(2);
          }
        }
      }
    }
    unset($key, $names);
  }
  return $icon;
}

/**
 * Iconize buttons based on the value.
 *
 * @param string $text
 *   Button text to search against.
 *
 * @return string
 *   The icon markup or FALSE if not matched.
 */
function druru_get_icon_by_title($text) {
  $icon_key = druru_search_icon_key($text);
  return $icon_key ? druru_icon($icon_key) : FALSE;
}

/**
 * Returns list of libraries of titles and related icon keys.
 *
 * First level of returned array must be keyed as prefix of icon.
 * Example: for icons of Bootstrap Framework key should be `glyphicon`,
 * because icons has such an HTML <i class="glyphicon glyphicon-<key>"></i>
 *
 * Second level must have next required keys:
 *   - title: Human readable name of the icons (showed at admin-side)
 *   - name:  it's machine name of font. Related to name of css file.
 *   - icons: array of declared icons where key it's icon suffix (<key>) and
 *            value is list of related titles that will be searched in buttons,
 *            links and other elements.
 *            See example in variable $base_icons inside the function.
 *
 * @return array
 *   Icons library info.
 */
function druru_icons_libraries() {
  $base_icons = array(
    'ban-circle'  => array(
      t('Discard'),
      t('Ban'),
    ),
    'check'       => array(
      t('Checked'),
    ),
    'cog'         => array(
      t('Manage'),
      t('Configure'),
    ),
    'download'    => array(
      t('Download'),
    ),
    'envelope'    => array(
      t('New email'),
      t('E-mail new password'),
      t('Create new email'),
      t('Create new message'),
    ),
    'export'      => array(
      t('Export'),
    ),
    'eye-open'    => array(
      t('Preview'),
      t('See'),
      t('Show'),
    ),
    'eye-close'    => array(
      t('Unpublished'),
    ),
    'floppy-disk' => array(
      t('Save'),
      t('Complete'),
      t('Done'),
    ),
    'import'      => array(
      t('Import'),
    ),
    'ok'          => array(
      t('Ok'),
      t('Check'),
    ),
    'pencil'      => array(
      t('Edit'),
    ),
    'plus'        => array(
      t('Add'),
      t('Create'),
      t('Write'),
    ),
    'remove'      => array(),
    'search'      => array(
      t('Search'),
      t('Find'),
    ),
    'send'        => array(
      t('Send'),
    ),
    'sign-in'     => array(
      t('Log in'),
    ),
    'trash'       => array(
      t('Delete'),
      t('Remove'),
    ),
    'upload'      => array(
      t('Upload'),
    ),
    'unchecked'   => array(
      t('Unchecked'),
    ),
  );
  $icons = array(
    'glyphicon' => array(
      'title'    => t('Bootstrap Glyphicons'),
      'name'     => 'glyphicons',
      'prefix'   => 'glyphicon',
      'throbber' => 'glyphicon-refresh',
      'icons'    => array(),
    ),
    'fa'        => array(
      'title'    => t('Font Awesome'),
      'name'     => 'font-awesome',
      'prefix'   => 'fa',
      'throbber' => 'fa-circle-o-notch',
      'icons'    => array(
        'floppy-o'       => $base_icons['floppy-disk'],
        'ban'            => $base_icons['ban-circle'],
        'eye'            => $base_icons['eye-open'],
        'eye-slash'      => $base_icons['eye-close'],
        'check'          => $base_icons['ok'],
        'check-square-o' => $base_icons['check'],
        'square-o'       => $base_icons['unchecked'],
        'paper-plane-o'  => $base_icons['send'],
        'line-chart'     => array(
          t('Interactive table'),
        ),
        'file-excel-o'   => array(
          t('Excel'),
        ),
        'user-plus'      => array(
          t('Create new account'),
        ),
        'github'      => array(
          t('Github'),
        ),
        'telegram'    => array(
          t('Telegram'),
        ),
        'comments'      => array(
          t('Gitter'),
        ),
        'youtube'      => array(
          t('YouTube'),
        ),
        'twitter'      => array(
          t('Twitter'),
        ),
        'facebook'      => array(
          t('Facebook'),
        ),
        'vk'      => array(
          t('VK'),
        ),
        'heart'      => array(
          t('Yandex.Money'),
        ),
      ),
    ),
  );

  // Filling with base icons.
  foreach ($icons as &$pack) {
    $pack['icons'] += $base_icons;
  }

  return $icons;
}

/**
 * Returns active selected library by theme settings from druru_icons_libraries.
 *
 * @return NULL|array
 *   Active library info if selected, null otherwise.
 *
 * @see druru_icons_libraries()
 */
function druru_get_icons_library() {
  $library = NULL;
  $get_library = variable_get('druru_icons_library_func', 'druru_icons_libraries');
  if (function_exists($get_library) && $libs = call_user_func($get_library)) {
    $selected_library = theme_get_setting('druru_icons_library') ?: 'glyphicon';
    if (isset($libs[$selected_library])) {
      $library = $libs[$selected_library];
    }
  }
  elseif (user_access('administer site configuration')) {
    drupal_set_message(t('Function %function does not exists. Can not get icons.', array(
      '%function' => $get_library,
    )), 'warning');
  }
  return $library;
}

/**
 * Try to fetch all available icon keys from css file.
 *
 * @return array|null
 *   List of available icon keys, null if any step of fetching has error.
 */
function druru_available_icons() {
  $icons = &drupal_static(__FUNCTION__);
  if ($icons === NULL) {
    $css = druru_get_icons_css_path('absolute');
    if (file_exists($css)) {
      $styles = file_get_contents($css);
      $prefix = theme_get_setting('druru_icons_library');
      preg_match_all('/\.' . $prefix . '\-(.*)\:/', $styles, $icons);
      // First key contains all match (with symbols like `.fa-cog:`)
      // Second - only `cog`.
      $icons = isset($icons[1]) ? $icons[1] : array();
    }
  }
  return $icons;
}

/**
 * Fetch libraries list as
 *  prefix => Human readable name
 *
 * @return array
 */
function druru_get_libraries_list() {
  $icon_libraries = array();
  $get_library = variable_get('druru_icons_library_func', 'druru_icons_libraries');
  if (function_exists($get_library) && $libs = call_user_func($get_library)) {
    // Fetch existing libraries list.
    foreach ($libs as $library_key => $library) {
      $icon_libraries[$library_key] = $library['title'];
    }
  }
  elseif (user_access('administer site configuration')) {
    drupal_set_message(t('Function %function does not exists. Can not get icons.', array(
      '%function' => $get_library,
    )), 'warning');
  }
  return $icon_libraries;
}
