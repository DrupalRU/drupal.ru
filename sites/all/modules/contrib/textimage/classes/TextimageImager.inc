<?php
/**
 * @file
 * Textimage - Image processor class.
 */

/**
 * Textimage - Image processor class.
 */
abstract class TextimageImager {

  /**
   * Get a Textimage URL, building the image if necessary.
   *
   * @param name $style_name
   *   the image style name.
   * @param array $effects_outline
   *   an outline of the style's effects data.
   * @param array $text
   *   text to be used to deliver the image.
   * @param string $extension
   *   (optional) file extension to be delivered (png jpg jpeg gif). Defaults
   *   to 'png'.
   * @param bool $caching
   *   (optional) TRUE if image caching has to be used. Defaults to TRUE.
   * @param object $node
   *   (optional) a node entity. It is used for resolving the tokens in the
   *   text effects.
   * @param object $source_image_file
   *   (optional) a file entity. It is used for resolving the tokens
   *   in the text effects.
   * @param string $target_uri
   *   (optional) defines the URI where the Textimage image file should be
   *   saved. Disables caching.
   *
   * @return array
   *   the URL of the image, or NULL in case of failure
   */
  public static function getImageUrl($style_name, $effects_outline, $text, $extension = 'png', $caching = TRUE, $node = NULL, $source_image_file = NULL, $target_uri = NULL) {
    $data = array(
      'style_name' => $style_name,
      'effects' => $effects_outline,
      'text' => $text,
      'target_uri' => $target_uri,
      'extension' => $extension,
      'caching' => $caching,
      'node' => $node,
      'source_image_file' => $source_image_file,
    );
    return self::getTextimage($data)->getUrl();
  }

  /**
   * Get a Textimage URI, building the image if necessary.
   *
   * @param name $style_name
   *   the image style name.
   * @param array $effects_outline
   *   an outline of the style's effects data.
   * @param array $text
   *   text to be used to deliver the image.
   * @param string $extension
   *   (optional) file extension to be delivered (png jpg jpeg gif). Defaults
   *   to 'png'.
   * @param bool $caching
   *   (optional) TRUE if image caching has to be used. Defaults to TRUE.
   * @param object $node
   *   (optional) a node entity. It is used for resolving the tokens in the
   *   text effects.
   * @param object $source_image_file
   *   (optional) a file entity. It is used for resolving the tokens
   *   in the text effects.
   * @param string $target_uri
   *   (optional) defines the URI where the Textimage image file should be
   *   saved. Disables caching.
   *
   * @return array
   *   the URI of the image, or NULL in case of failure
   */
  public static function getImageUri($style_name, $effects_outline, $text, $extension = 'png', $caching = TRUE, $node = NULL, $source_image_file = NULL, $target_uri = NULL) {
    $data = array(
      'style_name' => $style_name,
      'effects' => $effects_outline,
      'text' => $text,
      'target_uri' => $target_uri,
      'extension' => $extension,
      'caching' => $caching,
      'node' => $node,
      'source_image_file' => $source_image_file,
    );
    return self::getTextimage($data)->getUri();
  }

  /**
   * Process image delivery request.
   *
   * @param array $style
   *   the image style
   * @param array $effects_outline
   *   an outline of the style's effects data
   * @param array $text
   *   text to be used to deliver the image
   * @param string $extension
   *   file extension to be delivered (png jpg jpeg gif)
   * @param bool $caching
   *   (optional) TRUE if image caching has to be used. Defaults to TRUE.
   * @param object $node
   *   (optional) a node entity. It is used for resolving the tokens in the
   *   text effects.
   * @param object $source_image_file
   *   (optional) a file entity. It is used for resolving the tokens
   *   in the text effects.
   * @param string $target_uri
   *   (optional) defines the URI where the Textimage image file should be
   *   saved. Disables caching.
   *
   * @return array
   *   the uri of the image, or NULL in case of failure
   */
  public static function processImageRequest($style, $effects_outline, &$text, $extension, $caching = TRUE, $node = NULL, $source_image_file = NULL, $target_uri = NULL) {
    $data = array(
      'style' => $style,
      'effects' => $effects_outline,
      'text' => $text,
      'target_uri' => $target_uri,
      'extension' => $extension,
      'caching' => $caching,
      'node' => $node,
      'source_image_file' => $source_image_file,
      'user_messages' => FALSE,
    );
    $textimage = self::getTextimage($data);
    $text = $textimage->getText();
    return $textimage->getUri();
  }

  /**
   * Get a Textimage, building the image if necessary.
   */
  public static function getTextimage($data = array()) {
    $data += array(
      'style' => NULL,
      'user_messages' => TRUE,
      'force_hashed_filename' => FALSE,
    );

    $textimage = new Textimage();

    if ($data['style']) {
      $textimage->style($data['style']);
    }
    elseif ($data['style_name']) {
      $textimage->styleByName($data['style_name']);
    }
    else {
      $textimage->effects(!empty($data['effects']) ? $data['effects'] : array());
    }

    if ($data['source_image_file']) {
      $textimage->sourceImageFile($data['source_image_file']);
    }
    if ($data['node']) {
      $textimage->node($data['node']);
    }

    if ($data['target_uri']) {
      $textimage->setTargetUri($data['target_uri']);
    }
    else {
      $textimage->setCaching($data['caching']);
    }

    if (isset($data['user_messages'])) {
      $textimage->setUserMessages($data['user_messages']);
    }

    if ($data['force_hashed_filename']) {
      $textimage->setHashedFilename(TRUE);
    }

    return $textimage->extension($data['extension'])->process($data['text']);
  }

  /**
   * Process text string, detokenise and apply case conversion.
   */
  public static function processTextString($text, $case_format, $node = NULL, $source_image_file = NULL) {

    // Replace any tokens in text with run-time values.
    global $user;
    $text = token_replace(
      $text,
      array(
        'user' => $user,
        'node' => $node,
        'file' => $source_image_file,
      )
    );
    // Convert case, if requested.
    switch ($case_format) {
      case 'upper':
        return drupal_strtoupper($text);

      case 'lower':
        return drupal_strtolower($text);

      case 'ucfirst':
        return drupal_ucfirst($text);

      case 'ucwords':
        return preg_replace_callback('/(\w+)/', array('TextimageImager', 'uppercaseWords'), $text);

      default:
        return $text;

    }
  }

  /**
   * Callback for capitalizing the first letter of each word in a string.
   */
  protected static function uppercaseWords($matches) {
    return drupal_ucfirst($matches[1]);
  }

  /**
   * Get an Imager state variable.
   *
   * @param string $variable
   *   state variable
   *
   * @return mixed
   *   returned variable, NULL if undefined
   */
  public static function getState($variable = NULL) {
    if ($variable) {
      return self::setState($variable);
    }
    return NULL;
  }

  /**
   * Set an Imager state variable.
   *
   * @param string $variable
   *   state variable
   * @param mixed $value
   *   value to set, or NULL to return current value
   *
   * @return mixed
   *   variable value
   */
  public static function setState($variable = NULL, $value = NULL) {
    static $keys;

    if (!isset($keys) or !$variable) {
      $keys = array();
    }

    if ($variable) {
      if ($value) {
        $keys[$variable] = $value;
        return $value;
      }
      else {
        return isset($keys[$variable]) ? $keys[$variable] : NULL;
      }
    }
  }

  /**
   * Textimage tokens replacement.
   *
   * @param string $key
   *   The Textimage token key within the main token [textimage:key:...].
   *   Key can take 'uri' or 'url' values.
   * @param array $tokens
   *   The tokens to resolve.
   * @param object $node
   *   The node for which to resolve the tokens.
   *
   * @return array
   *   An array of token replacements.
   */
  public static function processTokens($key, $tokens, $node) {

    // Need to avoid endless loops, that would occur if there are
    // circular references in the tokens. Set static variables for
    // the nesting level and the stack of fields accessed so far.
    static $nesting_level;
    static $field_stack;
    if (!isset($nesting_level)) {
      $nesting_level = 0;
      $field_stack = array();
    }
    else {
      $nesting_level++;
    }

    // Get tokens specific for the required key.
    $sub_tokens = token_find_with_prefix($tokens, $key);

    // Return immediately if none, or no node.
    if (empty($sub_tokens) || !$node) {
      self::rollbackStack($nesting_level, $field_stack);
      return array();
    }

    // Determine the callback function.
    switch ($key) {
      case 'uri':
        $callback_method = 'getImageUri';
        break;

      case 'url':
        $callback_method = 'getImageUrl';
        break;

    }

    // Loops through the tokens to resolve.
    $replacements = array();
    foreach ($sub_tokens as $sub_token => $original) {

      // Clear current nesting level field stack.
      unset($field_stack[$nesting_level]);

      // Get token elements.
      $sub_token_array = explode(':', $sub_token);

      // Get requested field name, continue if missing.
      $field_name = isset($sub_token_array[0]) ? $sub_token_array[0] : NULL;
      if (!$field_name) {
        continue;
      }

      // Check for recursion, i.e. the field is already engaged in a
      // token resolution. Throw a TextimageImagerTokenException in case.
      if (in_array($field_name, $field_stack)) {
        self::rollbackStack($nesting_level, $field_stack);
        throw new TextimageImagerTokenException($original);
      }

      // Set current requested field in the field stack.
      $field_stack[$nesting_level] = $field_name;

      // Get requested display mode, default to 'default'.
      $display_mode = isset($sub_token_array[1]) ? $sub_token_array[1] : 'default';

      // Get requested sequence, default to NULL.
      $index = isset($sub_token_array[2]) ? $sub_token_array[2] : NULL;

      // Get general field info, continue if missing.
      $field_info = field_info_field($field_name);
      if (!$field_info) {
        continue;
      }

      // Get node (bundle) dependent field info, continue if missing.
      $node_type = $node->type;
      $instance_info = field_info_instance('node', $field_name, $node_type);
      if (!$instance_info) {
        continue;
      }

      // Get info on module providing formatting, continue if missing.
      $display_module = isset($instance_info['display'][$display_mode]['module']) ? $instance_info['display'][$display_mode]['module'] : NULL;
      if (!$display_module) {
        continue;
      }

      // At this point, if Textimage is providing field formatting for the
      // current field, we can proceed accessing the data needed to resolve
      // the token.
      if ($display_module == 'textimage') {

        // Get the image style used for the field formatting.
        $image_style = isset($instance_info['display'][$display_mode]['settings']['image_style']) ? $instance_info['display'][$display_mode]['settings']['image_style'] : NULL;
        if (!$image_style) {
          continue;
        }

        // Get the field items.
        $items = field_get_items('node', $node, $field_name);

        // Invoke Textimage API functions to return the token value requested.
        if ($field_info['module'] == 'text') {
          // Text field. Get sanitized text items and return a single image.
          $text = TextimageImager::getTextFieldText($items, $field_info, $instance_info, $node);
          try {
            $replacements[$original] = call_user_func(array('TextimageImager', $callback_method),
              $image_style,
              NULL,
              $text,
              'png',
              TRUE,
              $node
            );
          }
          catch (TextimageImagerTokenException $e) {
            // Callback ended up in circular loop, mark the failing token.
            $replacements[$original] = str_replace('textimage', 'void-textimage', $original);
            if ($nesting_level > 0) {
              // Returns up in the nesting of iteration with the failing token.
              self::rollbackStack($nesting_level, $field_stack);
              throw new TextimageImagerTokenException($e->getToken());
            }
            else {
              // Inform about the token failure.
              $msg = t("Textimage token @token in node '@node_title' can not be resolved (circular reference). Remove the token to avoid this message.",
                array(
                  '@token' => $original,
                  '@node_title' => $node->title,
                )
              );
              _textimage_diag($msg, WATCHDOG_WARNING);
            }
          }
        }
        elseif ($field_info['module'] == 'image') {
          // Image field. Get a separate Textimage from each of the images
          // in the field.
          try {
            $ret = array();
            foreach ($items as $item) {
              // Get source image from the image field item.
              $source_image_file = file_load($item['fid']);
              $ret[] = call_user_func(array('TextimageImager', $callback_method),
                $image_style,
                NULL,
                NULL,
                'png',
                TRUE,
                $node,
                $source_image_file
              );
            }
            // Return a single URI/URL if requested, or a comma separated
            // list of all the URIs/URLs generated.
            if (!is_null($index) && isset($ret[$index])) {
              $replacements[$original] = $ret[$index];
            }
            else {
              $replacements[$original] = implode(',', $ret);
            }
          }
          catch (TextimageImagerTokenException $e) {
            // Callback ended up in circular loop, mark the failing token.
            $replacements[$original] = str_replace('textimage', 'void-textimage', $original);
            if ($nesting_level > 0) {
              // Returns up in the nesting of iteration with the failing token.
              self::rollbackStack($nesting_level, $field_stack);
              throw new TextimageImagerTokenException($e->getToken());
            }
            else {
              // Inform about the token failure.
              $msg = t("Textimage token @token in node '@node_title' can not be resolved (circular reference). Remove the token to avoid this message.",
                array(
                  '@token' => $original,
                  '@node_title' => $node->title,
                )
              );
              _textimage_diag($msg, WATCHDOG_WARNING);
            }
          }
        }
      }
    }

    // Return to previous iteration.
    self::rollbackStack($nesting_level, $field_stack);
    return $replacements;
  }

  /**
   * Helper method to rollback nesting static variables in processTokens.
   */
  protected static function rollbackStack(&$nesting_level, &$field_stack) {
    if ($nesting_level) {
      unset($field_stack[$nesting_level]);
      $nesting_level--;
    }
    else {
      $nesting_level = NULL;
    }
  }

  /**
   * Retrieves text from a Text field.
   *
   * Text gets sanitized for use within Textimage: HTML tags are
   * stripped.
   *
   * @param array $items
   *   Field items.
   * @param array $field
   *   The field where items are contained.
   * @param array $instance
   *   The field instance.
   * @param object $node
   *   The node where the items are contained.
   *
   * @return array
   *   An array of sanitized text items.
   */
  public static function getTextFieldText($items, $field, $instance, $node) {
    $text = array();
    if (!empty($items)) {
      foreach ($items as $item) {
        $text[] = strip_tags($item['value']);
      }
    }
    return $text;
  }

}

/**
 * Exception thrown by TextimageImager on token processing failure.
 */
class TextimageImagerTokenException extends Exception {

  /**
   * The failing token.
   *
   * @var string
   */
  protected $token;

  /**
   * Constructs a TextimageImagerTokenException object.
   */
  public function __construct($token) {
    parent::__construct(t("Textimage token @token could not be resolved.", array('@token' => $token)), 0);
    $this->token = $token;
  }

  /**
   * Gets failing token.
   */
  public function getToken() {
    return $this->token;
  }

}
