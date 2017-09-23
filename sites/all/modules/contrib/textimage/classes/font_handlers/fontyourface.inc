<?php
/**
 * @file
 * Textimage - Fonts handler class for @font-your-face.
 */

/**
 * Fonts handler for @font-your-face.
 *
 * Currently only provides access to fonts uploaded locally via the
 * local font uploading features.
 */
class TextimageFontsHandlerFontyourface implements TextimageFontsHandlerInterface {

  /**
   * Return an array of fonts.
   *
   * @param array $options
   *   an array of additional options.
   *
   * @return array
   *   Array of font names.
   */
  public function getList($options) {
    $list = array();
    $font_list = fontyourface_get_fonts("enabled = 1 and provider = 'local_fonts'", 'name ASC');
    if ($font_list) {
      foreach ($font_list as $font) {
        $list[] = $font->name;
      }
    }
    return $list;
  }

  /**
   * Return the URI of a font file, given its name.
   *
   * @param string $font_name
   *   the name of the font.
   * @param array $options
   *   an array of additional options.
   *
   * @return string
   *   the URI of the font file.
   */
  public function getUri($font_name, $options) {
    $font = fontyourface_get_fonts("enabled = 1 and provider = 'local_fonts' and name = '$font_name'");
    if (!empty($font)) {
      $font = array_shift($font);
      $metadata = unserialize($font->metadata);
      if (isset($metadata['font_uri']['truetype'])) {
        return $metadata['font_uri']['truetype'];
      }
    }
    return NULL;
  }
}
