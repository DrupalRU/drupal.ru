<?php
/**
 * @file
 * Textimage - Font management classes.
 */

/**
 * Font management class.
 *
 * Interacts with font management modules (handlers) to provide
 * information on fonts available on the site for Textimage to use.
 */
abstract class TextimageFonts {

  /**
   * Font handler class in use.
   */
  protected static $handler;

  /**
   * Name of the font module handler.
   */
  protected static $handlingModule;

  /**
   * Set font handler.
   *
   * @param string $module
   *   optional - the name of the module to be set for handling font requests.
   *   If not specified, will use the module specified at configuration.
   *
   * @return class
   *   the handler class or NULL if not available.
   */
  protected static function setHandler($module = NULL) {
    if (!isset(self::$handler) or ($module and $module <> self::$handlingModule)) {
      self::$handlingModule = $module ? $module : _textimage_get_variable('fonts_handling_module');
      if (!module_exists(self::$handlingModule)) {
        _textimage_diag(t("Font handling module '@module' is no longer available.", array('@module' => self::$handlingModule)), WATCHDOG_ERROR, __FUNCTION__);
        return NULL;
      }
      $class = 'TextimageFontsHandler' . drupal_ucfirst(self::$handlingModule);
      self::$handler = new $class();
    }
    return self::$handler;
  }

  /**
   * Get a list of font names.
   *
   * @param string $module
   *   optional - the name of the module to be set for handling font requests.
   * @param array $options
   *   optional - an array of options.
   *
   * @return array
   *   a simple array with the list of font names.
   */
  public static function getList($module = NULL, $options = array()) {
    if (self::setHandler($module) and $list = self::$handler->getList($options)) {
      return $list;
    }
    return array();
  }

  /**
   * Get the URI of a font file.
   *
   * @param string $font_name
   *   the name of the font.
   * @param string $module
   *   optional - the name of the module to be set for handling font requests.
   * @param array $options
   *   optional - an array of options.
   *
   * @return string
   *   the URI of the font file.
   */
  public static function getUri($font_name, $module = NULL, $options = array()) {
    self::setHandler($module);
    return self::$handler->getUri($font_name, $options);
  }
}

/**
 * Fonts handler interface.
 *
 * Defines the methods that font handling classes have to implement.
 */
interface TextimageFontsHandlerInterface {

  /**
   * Get a list of font names.
   *
   * @param array $options
   *   an array of additional options.
   *
   * @return array
   *   Array of font names.
   */
  public function getList($options);

  /**
   * Get the URI of a font file.
   *
   * @param string $font_name
   *   the name of the font.
   * @param array $options
   *   an array of additional options.
   *
   * @return string
   *   the URI of the font file.
   */
  public function getUri($font_name, $options);
}
