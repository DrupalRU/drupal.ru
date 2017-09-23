<?php
/**
 * @file
 * Textimage - Styles handling class.
 */

/**
 * Styles management class.
 *
 * Handles Textimage style related requests, and integrates with core
 * image module.
 */
abstract class TextimageStyles {

  /**
   * Return default setting for Textimage styles.
   */
  public static function defaultSettings() {
    return array(
      'uri_scheme' => _textimage_get_variable('store_scheme'),
    );
  }

  /**
   * Check if a Textimage style exists.
   *
   * @param string $style_name
   *   name of the style to check for existence
   *
   * @return bool
   *   TRUE if style exists, otherwise FALSE
   */
  public static function exists($style_name) {
    return self::get($style_name) ? TRUE : FALSE;
  }

  /**
   * Get a Textimage style by name.
   *
   * @param string $style_name
   *   style name
   *
   * @return array
   *   the $style array enriched with a 'textimage' key where
   *   textimage specific data is kept, or NULL if non existing.
   */
  public static function get($style_name) {
    $style = image_style_load($style_name);
    return self::isTextimage($style) ? $style : NULL;
  }

  /**
   * Get an array of all Textimage styles available.
   *
   * @return array
   *   the list of Textimage styles available
   */
  public static function getList() {
    $styles = image_styles();
    $list = array();
    foreach ($styles as $style_name => $style_info) {
      if ($style = self::get($style_name)) {
        $list[] = $style;
      }
    }
    return $list;
  }

  /**
   * Get an array of Textimage styles for using as select list options.
   *
   * @param bool $include_empty
   *   If TRUE a <none> option will be inserted in the options array.
   *
   * @return array
   *   Array of image styles with both key and value set to style name.
   */
  public static function getOptions($include_empty = TRUE) {
    $image_options = image_style_options($include_empty);
    $options = array();
    foreach ($image_options as $option => $value) {
      if ($option == '') {
        $options[$option] = $value;
        continue;
      }
      $style = self::get($option);
      if (self::isTextimage($style)) {
        $options[$option] = $value;
      }
    }
    return $options;
  }

  /**
   * Check if an image style is Textimage relevant.
   *
   * Also, it loads Textimage properties to the 'textimage' key of the
   * style array.
   *
   * @param array $style
   *   the image style to check
   *
   * @return bool
   *   TRUE if style is Textimage relevant, otherwise FALSE
   */
  public static function isTextimage(&$style) {
    if (!isset($style['effects'])) {
      return FALSE;
    }
    foreach ($style['effects'] as $effect_info) {
      if (strpos($effect_info['name'], 'textimage') === 0) {
        if (!is_array($style['textimage'])) {
          $style['textimage'] = empty($style['textimage']) ? array() : (array) (unserialize($style['textimage']));
        }
        $style['textimage'] += self::defaultSettings();
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Save Textimage style data.
   *
   * @param array $style
   *   image style to save
   */
  public static function save($style, $on_delete = FALSE) {
    // A change to the name of the style.
    if (isset($style['old_name']) && $style['old_name'] != $style['name']) {
      // Change style name in fields.
      $instances = field_read_instances();
      // Loop through all fields searching for textimage fields.
      foreach ($instances as $instance) {
        $instance_changed = FALSE;
        foreach ($instance['display'] as $view_mode => $display) {
          // Check if the formatter involves an image style.
          if ($display['type'] == 'textimage' && $display['settings']['image_style'] == $style['old_name']) {
            // Update display information for any instance using the image
            // style that was just deleted.
            $instance['display'][$view_mode]['settings']['image_style'] = $style['name'];
            $instance_changed = TRUE;
          }
        }
        if ($instance_changed) {
          field_update_instance($instance);
        }
      }
      // Void store.
      if (!$on_delete) {
        db_update('textimage_store')
          ->fields(array(
            'style_name' => $style['name'],
            'is_void' => 1,
          ))
          ->condition('style_name', $style['old_name'], '=')
          ->execute();
      }
      return;
    }

    // Void store.
    db_update('textimage_store')
      ->fields(array(
        'is_void' => 1,
      ))
      ->condition('style_name', $style['name'], '=')
      ->execute();
  }

  /**
   * Flushes Textimage style data.
   *
   * Clears immediate cache and all the image files associated.
   *
   * @param array $style
   *   the style being flushed
   */
  public static function flush($style) {
    cache_clear_all('tiid:', 'cache_textimage', TRUE);
    // Clear hashed filename images.
    if (file_exists(_textimage_get_store_path('styled_hashed/') . $style['name'])) {
      file_unmanaged_delete_recursive(_textimage_get_store_path('styled_hashed/') . $style['name']);
    }
    // Clear images, checking in all available schemes.
    foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $wrapper => $wrapper_data) {
      if (file_exists($wrapper . '://textimage/' . $style['name'])) {
        file_unmanaged_delete_recursive($wrapper . '://textimage/' . $style['name']);
      }
    }
  }

  /**
   * Delete Textimage style data.
   *
   * @param array $style
   *   the style being deleted
   */
  public static function delete($style) {
    // Clear store.
    db_delete('textimage_store')
      ->condition('style_name', $style['old_name'], '=')
      ->execute();
    // Change style in fields, if a new textimage style is indicated.
    if ($style['name']) {
      if (self::get($style['name'])) {
        self::save($style, TRUE);
      }
    }
  }

  /**
   * Build an outline of the style effects' data, given the style name.
   *
   * @param array $style_name
   *   Image style name.
   *
   * @return array
   *   a simple array, where each element is an associative array of
   *   'name' => the effect name
   *   'data' => the effect data
   */
  public static function getStyleEffectsOutline($style_name) {
    if ($style = self::get($style_name)) {
      return self::getEffectsOutline($style);
    }
    else {
      return NULL;
    }
  }

  /**
   * Build an outline of the style effects' data, given the style array.
   *
   * This is used to build a thumbnail of just the effects' data, that can be
   * used to produce a md5 hash without bringing in db dependent values.
   * It also removes 'textimage_text' text input in the admin form for the
   * effect, as this is a separate input to the hash.
   *
   * @param array $style
   *   image style
   *
   * @return array
   *   a simple array, where each element is an associative array of
   *   'name' => the effect name
   *   'data' => the effect data
   */
  public static function getEffectsOutline($style) {
    if (isset($style['effects'])) {
      $effects_outline = array();
      foreach ($style['effects'] as $effect_info) {
        $i = array(
          'name' => $effect_info['name'],
          'data' => $effect_info['data'],
        );
        $effects_outline[] = $i;
      }
      return $effects_outline;
    }
    else {
      return NULL;
    }
  }

  /**
   * Build a virtual style from an effects data outline.
   *
   * This is used to enable the direct creation of a textimage using core
   * image module functions and API. The style array returned is 'virtual',
   * i.e. it does not get saved to db. It is used to be passed to
   * image_style_create_derivative() to build a picture.
   *
   * @param array $effects_outline
   *   a simple array, where each element is an associative array of
   *   'name' => the effect name
   *   'data' => the effect data
   *
   * @return array
   *   image style
   */
  public static function buildFromEffectsOutline($effects_outline) {
    $definitions = image_effect_definitions();
    $style = array();
    $style['effects'] = array();
    foreach ($effects_outline as $effect) {
      if ($ed = $definitions[$effect['name']]) {
        $ed['data'] = $effect['data'];
        $style['effects'][] = $ed;
      }
    }
    return $style;
  }

}
