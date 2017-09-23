<?php
/**
 * @file
 * Textimage - Fonts handler class for Textimage.
 */

/**
 * Fonts handler for Textimage.
 *
 * Basic font handler if no other modules are available. Just provides access to
 * fonts stored in a directory, specified in configuration.
 */
class TextimageFontsHandlerTextimage implements TextimageFontsHandlerInterface {

  /**
   * Return an array of fonts.
   *
   * Scans through files available in the directory specified through
   * configuration.
   *
   * @param array $options
   *   an array of additional options.
   *
   * @return array
   *   Array of font names.
   */
  public function getList($options) {
    $fonts_path = isset($options['fonts_path']) ? $options['fonts_path'] : _textimage_get_variable('fonts_path');
    $filelist = array();
    if (is_dir($fonts_path) && $handle = opendir($fonts_path)) {
      while ($file_name = readdir($handle)) {
        if (preg_match("/\.[ot]tf$/i", $file_name) == 1) {
          $font = self::getData($fonts_path . '/' . $file_name);
          $filelist[$file_name] = $font['name'];
        }
      }
      closedir($handle);
    }
    asort($filelist);
    return $filelist;
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
    $fonts_path = isset($options['fonts_path']) ? $options['fonts_path'] : _textimage_get_variable('fonts_path');
    if (is_dir($fonts_path) && $handle = opendir($fonts_path)) {
      while ($file_name = readdir($handle)) {
        if (preg_match("/\.[ot]tf$/i", $file_name) == 1) {
          $font = self::getData($fonts_path . '/' . $file_name);
          if ($font_name == $font['name']) {
            return $font['file'];
          }
        }
      }
      closedir($handle);
    }
    return NULL;
  }

  /**
   * Helper - Return the font information.
   *
   * Scans the font file to return tags information.
   *
   * @param string $uri
   *   the URI of the font file.
   *
   * @return array
   *   an associative array with the following keys:
   *   'copyright' => Copyright information
   *   'family' => Font family
   *   'subfamily' => Font subfamily
   *   'name' => Font name
   *   'file' => Font file URI
   */
  protected function getData($uri) {
    $realpath = drupal_realpath($uri);
    $fd = fopen($realpath, "r");
    $text = fread($fd, filesize($realpath));
    fclose($fd);

    $number_of_tabs = $this->dec2hex(ord($text[4])) . $this->dec2hex(ord($text[5]));
    for ($i = 0; $i < hexdec($number_of_tabs); $i++) {
      $tag = $text[12 + $i * 16] . $text[12 + $i * 16 + 1] . $text[12 + $i * 16 + 2] . $text[12 + $i * 16 + 3];
      if ($tag == "name") {
        $offset_name_table_hex = $this->dec2hex(ord($text[12 + $i * 16 + 8])) . $this->dec2hex(ord($text[12 + $i * 16 + 8 + 1])) . $this->dec2hex(ord($text[12 + $i * 16 + 8 + 2])) . $this->dec2hex(ord($text[12 + $i * 16 + 8 + 3]));
        $offset_name_table_dec = hexdec($offset_name_table_hex);
        $offset_storage_hex = $this->dec2hex(ord($text[$offset_name_table_dec + 4])) . $this->dec2hex(ord($text[$offset_name_table_dec + 5]));
        $offset_storage_dec = hexdec($offset_storage_hex);
        $number_name_records_hex = $this->dec2hex(ord($text[$offset_name_table_dec + 2])) . $this->dec2hex(ord($text[$offset_name_table_dec + 3]));
        $number_name_records_dec = hexdec($number_name_records_hex);
        break;
      }
    }

    $storage_dec = $offset_storage_dec + $offset_name_table_dec;
    $font = array(
      'copyright' => '',
      'family' => '',
      'subfamily' => '',
      'name' => '',
      'file' => $uri,
    );

    for ($j = 0; $j < $number_name_records_dec; $j++) {
      // Platform ID (hex) would be:
      // dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 0])) .
      // dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 1]))
      $name_id_hex = $this->dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 6])) . $this->dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 7]));
      $name_id_dec = hexdec($name_id_hex);
      $string_length_hex = $this->dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 8])) . $this->dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 9]));
      $string_length_dec = hexdec($string_length_hex);
      $string_offset_hex = $this->dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 10])) . $this->dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 11]));
      $string_offset_dec = hexdec($string_offset_hex);

      if ($name_id_dec == 0 && empty($font['copyright'])) {
        for ($l = 0; $l < $string_length_dec; $l++) {
          if (ord($text[$storage_dec + $string_offset_dec + $l]) >= 32) {
            $font['copyright'] .= $text[$storage_dec + $string_offset_dec + $l];
          }
        }
      }

      if ($name_id_dec == 1 && empty($font['family'])) {
        for ($l = 0; $l < $string_length_dec; $l++) {
          if (ord($text[$storage_dec + $string_offset_dec + $l]) >= 32) {
            $font['family'] .= $text[$storage_dec + $string_offset_dec + $l];
          }
        }
      }

      if ($name_id_dec == 2 && empty($font['subfamily'])) {
        for ($l = 0; $l < $string_length_dec; $l++) {
          if (ord($text[$storage_dec + $string_offset_dec + $l]) >= 32) {
            $font['subfamily'] .= $text[$storage_dec + $string_offset_dec + $l];
          }
        }
      }

      if ($name_id_dec == 4 && empty($font['name'])) {
        for ($l = 0; $l < $string_length_dec; $l++) {
          if (ord($text[$storage_dec + $string_offset_dec + $l]) >= 32) {
            $font['name'] .= $text[$storage_dec + $string_offset_dec + $l];
          }
        }
      }

      if ($font['copyright'] != "" && $font['family'] != "" && $font['subfamily'] != "" && $font['name'] != "") {
        break;
      }
    }

    return $font;
  }

  /**
   * Helper - Convert a dec to a hex.
   *
   * @param int $dec
   *   an integer number
   *
   * @return string
   *   the number represented as hex
   */
  protected function dec2hex($dec) {
    $hex = dechex($dec);
    return str_repeat("0", 2 - drupal_strlen($hex)) . drupal_strtoupper($hex);
  }
}
