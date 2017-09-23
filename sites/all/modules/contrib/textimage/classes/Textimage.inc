<?php

/**
 * @file
 * Textimage - Textimage class.
 */

class Textimage {

  /**
   * Textimage id.
   *
   * It is a MD5 hash of the textimage effects and data.
   *
   * @var string
   */
  protected $id = NULL;

  /**
   * If this Textimage has been processed.
   *
   * @var bool
   */
  protected $processed = FALSE;

  /**
   * Textimage metadata.
   *
   * @var array
   */
  protected $imageData = array();

  /**
   * Textimage execution time.
   *
   * @var int
   */
  protected $timer = NULL;

  /**
   * Textimage URI.
   *
   * @var string
   */
  protected $uri = NULL;

  /**
   * Image style used for this Textimage.
   *
   * @var array
   */
  protected $style = NULL;

  /**
   * The array of image effects for this Textimage.
   *
   * @var array
   */
  protected $effects = array();

  /**
   * The array of text elements for this Textimage.
   *
   * @var array
   */
  protected $text = array();

  /**
   * The file extension for this Textimage.
   *
   * @var string
   */
  protected $extension = 'png';

  /**
   * If this Textimage has to be cached.
   *
   * @var bool
   */
  protected $caching = TRUE;

  /**
   * A node entity to resolve node tokens.
   *
   * @var object
   */
  protected $node = NULL;

  /**
   * An image file entity.
   *
   * The source file used to build the image derivative in standard image
   * system context. Also used to track Textimages from image fields formatted
   * through Textimage field display formatter and to resolve file tokens.
   *
   * @var object
   */
  protected $sourceImageFile = NULL;

  /**
   * If this Textimage has to use a hash filename instead of human readable.
   *
   * @var bool
   */
  protected $forceHashedFilename = FALSE;

  /**
   * If this Textimage has to be created at a specific URI.
   *
   * @var bool
   */
  protected $forcedUri = FALSE;

  /**
   * If Textimage has to provide user notification of errors.
   *
   * @var bool
   */
  protected $userMessages = TRUE;

  /**
   * Set a property to a specified value.
   *
   * A Textimage already processed will not allow changes.
   *
   * @param string $property
   *   the property to set
   * @param mixed $value
   *   the value to set
   *
   * @return self
   *   this object
   */
  protected function set($property, $value) {
    if (!property_exists($this, $property)) {
      throw new TextimageException(t("Attempted to set non existing property '@property'.", array('@property' => $property)));
    }
    if (!$this->processed) {
      $this->$property = $value;
    }
    else {
      throw new TextimageException(t("Attempted to set property '@property' when image was processed already.", array('@property' => $property)));
    }
    return $this;
  }

  /**
   * Set the image style.
   *
   * @param object $image_style
   *   the image style to be used to derive the Textimage
   *
   * @return self
   *   this object
   */
  public function style($image_style) {
    if (TextimageStyles::isTextimage($image_style)) {
      $this->set('style', $image_style);
      $this->set('effects', TextimageStyles::getEffectsOutline($this->style));
    }
    return $this;
  }

  /**
   * Set the image style, from the style name.
   *
   * @param string $image_style_name
   *   the name of the image style to be used to derive the Textimage
   *
   * @return self
   *   this object
   */
  public function styleByName($image_style_name) {
    if ($image_style_name) {
      // Retrieve Textimage style.
      if ($image_style = TextimageStyles::get($image_style_name)) {
        $this->set('style', $image_style);
        $this->set('effects', TextimageStyles::getEffectsOutline($this->style));
      }
      else {
        _textimage_diag(t("Textimage could not find image style '@style'.", array('@style' => $image_style_name)), WATCHDOG_ERROR, NULL, $this->userMessages);
      }
    }
    else {
      _textimage_diag(t("Image style not specified while processing a Textimage."), WATCHDOG_ERROR, NULL, $this->userMessages);
    }
    return $this;
  }

  /**
   * Set the image effects.
   *
   * @param array $effects
   *   An array of image effects. Since Textimage manipulates effects before
   *   rendering the image, the style effects are copied here to allow that.
   *
   * @return self
   *   this object
   */
  public function effects(array $effects) {
    return $this->set('effects', $effects);
  }

  /**
   * Set the image file extension.
   *
   * @param string $extension
   *   The file extension to be used (jpg/png/gif).
   *
   * @return self
   *   this object
   */
  public function extension($extension) {
    return $this->set('extension', $extension);
  }

  /**
   * Set the image source file.
   *
   * @param object $source_image_file
   *   A file entity.
   *
   * @return self
   *   this object
   */
  public function sourceImageFile($source_image_file) {
    return $this->set('sourceImageFile', $source_image_file);
  }

  /**
   * Set a node entity to resolve node tokens.
   *
   * @param object $node
   *   A node entity.
   *
   * @return self
   *   this object
   */
  public function node($node) {
    return $this->set('node', $node);
  }

  /**
   * Set caching.
   *
   * @param bool $caching
   *   TRUE if caching is required for this Textimage.
   *
   * @return self
   *   this object
   */
  public function setCaching($caching) {
    // If destination URI has been forced, this setting is not effective.
    if (!$this->forcedUri) {
      $this->set('caching', $caching);
    }
    return $this;
  }

  /**
   * Set image destination URI.
   *
   * @param string $uri
   *   A valid URI.
   *
   * @return self
   *   this object
   */
  public function setTargetUri($uri) {
    if ($uri) {
      $this->set('uri', $uri);
      $this->set('caching', FALSE);
      $this->set('forcedUri', TRUE);
    }
    return $this;
  }

  /**
   * Set user notification of errors.
   *
   * @param bool $user_messages
   *   TRUE if Textimage errors need to be notified to users.
   *
   * @return self
   *   this object
   */
  public function setUserMessages($user_messages) {
    return $this->set('userMessages', $user_messages);
  }

  /**
   * Force hashed filename.
   *
   * @param bool $force_hashed_filename
   *   TRUE if Textimage has to use an hashed filename even if a human
   *   readable one could be attempted.
   *
   * @return self
   *   this object
   */
  public function setHashedFilename($force_hashed_filename) {
    return $this->set('forceHashedFilename', $force_hashed_filename);
  }

  /**
   * Return the Textimage id.
   *
   * @return string
   *   A MD5 hash.
   */
  public function id() {
    return $this->processed ? $this->id : NULL;
  }

  /**
   * Return the processed text.
   *
   * @return array
   *   An array of fully processed text elements.
   */
  public function getText() {
    return $this->processed ? $this->text : array();
  }

  /**
   * Return the URI of the Textimage.
   *
   * @return string
   *   An URI.
   */
  public function getUri() {
    return $this->processed ? $this->uri : NULL;
  }

  /**
   * Return the URL of the Textimage.
   *
   * @return string
   *   An URL.
   */
  public function getUrl() {
    return $this->processed ? ($this->uri ? file_create_url($this->uri) : NULL) : NULL;
  }

  /**
   * Load Textimage metadata from store.
   *
   * If the image file is missing at URI, it is rebuilt.
   *
   * @param string $id
   *   The id of the Textimage to load.
   *
   * @return self
   *   this object
   */
  public function load($id) {

    // Do not re-process.
    if ($this->processed) {
      return $this;
    }

    // Check if we have the hash in store.
    $stored_image = db_select('textimage_store', 'ic')
        ->fields('ic')
        ->condition('tiid', $id, '=')
        ->execute()
        ->fetchAssoc();

    // Not in stock, return.
    if (!$stored_image) {
      return $this;
    }

    // Restore properties.
    $this->id = $stored_image['tiid'];
    $is_void = $stored_image['is_void'];
    $this->styleByName($stored_image['style_name']);
    if ($is_void) {
      $this->effects = unserialize($stored_image['effects_outline']);
    }
    $this->imageData = unserialize($stored_image['image_data']);
    $this->text = $this->imageData['text'];
    $this->extension = $this->imageData['extension'];
    if (!empty($this->imageData['forceHashedFilename'])) {
      $this->forceHashedFilename = $this->imageData['forceHashedFilename'];
    }
    $this->timer = $stored_image['timer'];

    // In stock, check file is there.
    if (is_file($stored_image['uri'])) {
      $this->uri = $stored_image['uri'];
      $this->processed = TRUE;
    }
    else {
      // If not, rebuild image file.
      $this->buildImage();
    }

    return $this;
  }

  /**
   * Process the Textimage, with the required raw text.
   *
   * @param array $text
   *   An array of text strings, with tokens not resolved.
   *
   * @return self
   *   this object
   */
  public function process($text) {

    // Do not re-process.
    if ($this->processed) {
      return $this;
    }

    // Effects must be loaded.
    if (empty($this->effects)) {
      _textimage_diag(t("Textimage had no image effects to process."), WATCHDOG_ERROR, NULL, $this->userMessages);
      return $this;
    }

    // Normalise $text to an array.
    if (!$text) {
      $text = array();
    }
    if (!is_array($text)) {
      $text = array($text);
    }

    // Build an array with default text from effects.
    $default_text = array();
    foreach ($this->effects as &$effect) {
      if ($effect['name'] == 'textimage_text') {
        $default_text[] = $effect['data']['text_string'];
      }
    }

    // Process text to resolve tokens and required case conversions.
    $processed_text = array();
    foreach ($this->effects as $e_data) {
      if ($e_data['name'] == 'textimage_text') {
        $text_item = array_shift($text);
        $default_text_item = array_shift($default_text);
        if ($text_item) {
          // Replace any tokens in text with run-time values.
          $text_item = ($text_item == '[textimage:default]') ? $default_text_item : $text_item;
          $processed_text[] = TextimageImager::processTextString($text_item, $e_data['data']['text']['case_format'], $this->node, $this->sourceImageFile);
        }
        elseif ($default_text_item) {
          $processed_text[] = TextimageImager::processTextString($default_text_item, $e_data['data']['text']['case_format'], $this->node, $this->sourceImageFile);
        }
        else {
          $processed_text[] = t('* Missing text *');
        }
      }
    }
    $this->text = $processed_text;

    // Remove default text from effects outline, as actual runtime text goes
    // separately to the hash.
    foreach ($this->effects as &$effect) {
      if ($effect['name'] == 'textimage_text') {
        unset($effect['data']['text_string']);
      }
    }

    // Data for this textimage. Use a dummy filename at this stage,
    // purely to resolve the mime type.
    $this->imageData = array(
      'text'       => $this->text,
      'filemime'   => file_get_mimetype('dummy.' . $this->extension),
      'extension'  => $this->extension,
      'source'     => $this->sourceImageFile ? $this->sourceImageFile->uri : NULL,
    );
    if ($this->forceHashedFilename) {
      $this->imageData['forceHashedFilename'] = TRUE;
    }

    // Get md5 hash, being the Textimage id, for cache checking.
    $hash_input = array(
      'effects_outline'     => $this->effects,
      'image_data'          => $this->imageData,
    );
    $this->id = md5(serialize($hash_input));

    // Check cache and/or store and return if db and file hit.
    if ($this->caching && $this->getCached()) {
      $this->processed = TRUE;
      return $this;
    }

    // Build the image.
    $this->buildImage();

    return $this;
  }

  /**
   * Build the image via core image_style_create_derivative() function.
   *
   * @return self
   *   this object
   */
  protected function buildImage() {

    // Track the image generation time.
    timer_start('Textimage::process');

    // Get URI of the to-be image file.
    if (!$this->uri) {
      $this->buildUri();
    }

    // Inject processed text in the textimage_text effects data.
    $effects = $this->effects;
    $processed_text = $this->text;
    foreach ($effects as &$e_data) {
      if ($e_data['name'] == 'textimage_text') {
        $e_data['data']['text_string'] = array_shift($processed_text);
      }
    }

    // If no source image specified, we are processing a pure Textimage
    // request. In that case we need to use a source dummy 1x1 image stored
    // in textimage/misc/images, and prepend an additional
    // 'textimage_background' effect to ensure we start with a clean
    // background.
    $source = isset($this->sourceImageFile) ? $this->sourceImageFile->uri : NULL;
    if (!$source) {
      $source = drupal_get_path('module', 'textimage') . '/misc/images/base.' . $this->extension;
      $cleanup_effect_id = max(array_keys($effects)) + 1;
      $cleanup_effect = array(
        $cleanup_effect_id => image_effect_definition_load('textimage_background'),
      );
      $cleanup_effect[$cleanup_effect_id]['data'] = array(
        'background_image' => array(
          'mode' => '',
        ),
      );
      $effects = $cleanup_effect + $effects;
    }

    // Build a runtime-only style.
    $runtime_style = TextimageStyles::buildFromEffectsOutline($effects);

    // Reset state.
    TextimageImager::setState();
    TextimageImager::setState('building_module', 'textimage');

    // Try a lock to the file generation process. If cannot get the lock,
    // return success if the file exists already. Otherwise return failure.
    $lock_name = 'textimage_process:' . $this->uri;
    if (!$lock_acquired = lock_acquire($lock_name)) {
      return file_exists($this->uri) ? TRUE : FALSE;
    }

    // Generate the image.
    if (!$this->processed = image_style_create_derivative($runtime_style, $source, $this->uri)) {
      if (isset($this->style)) {
        _textimage_diag(t("Textimage failed to build an image for image style '@style'.", array('@style' => $this->style['name'])), WATCHDOG_ERROR, NULL, $this->userMessages);
      }
      else {
        _textimage_diag(t("Textimage failed to build an image."), WATCHDOG_ERROR, NULL, $this->userMessages);
      }
    }

    // Release lock.
    if (!empty($lock_acquired)) {
      lock_release($lock_name);
    }

    // Reset state.
    TextimageImager::setState();

    // Track the image generation time.
    $timer = timer_stop('Textimage::process');

    // Saves db imagestore data.
    if ($this->processed && $this->caching) {
      $this->setCached();
      $this->timer = $timer['time'];
      $this->putInStore();
    }

  }

  /**
   * Set URI to image file.
   *
   * If file name is human readable, then image would go to:
   *
   *   {style_wrapper}://textimage/{style}/{file name}.{extension}
   *
   * in all other cases, an appropriate directory structure is in place to
   * support styled, unstyled and uncached (temporary) image files:
   *
   * for images with a supporting image style (styled) -
   *   {textimage_store_wrapper}://textimage_store/styled_hashed/{style}/{file name}.{extension}
   *
   * for images generated via direct theme (unstyled) -
   *   {textimage_store_wrapper}://textimage_store/unstyled_hashed/{file name}.{extension}
   *
   * for uncached, temporary -
   *   {textimage_store_wrapper}://textimage_store/uncached/{file name}.{extension}
   */
  protected function buildUri() {

    // If style and caching are set, then try a clear file uri.
    if ($this->style && $this->caching && !$this->forceHashedFilename && $this->getStyledImageClearFileUri()) {
      return;
    }

    // Otherwise, the hash will be the file name, and files stored in
    // textimage_store.
    if ($this->caching) {
      $base_name = $this->id . '.' . $this->extension;
      if ($this->style) {
        $this->uri = _textimage_get_store_path('styled_hashed/') . $this->style['name'] . '/' . $base_name;
      }
      else {
        $this->uri = _textimage_get_store_path('unstyled_hashed/') . $base_name;
      }
    }
    else {
      $base_name = md5(session_id() . microtime()) . '.' . $this->extension;
      $this->uri = _textimage_get_store_path('uncached/') . $base_name;
    }

  }

  /**
   * Set URI to a human readable name for the image file, if possible.
   *
   * If a style-based image is requested, then hopefully a human readable
   * file name can be set.
   *
   * @return bool
   *   TRUE if URI is set to human readable file name
   */
  protected function getStyledImageClearFileUri() {

    // Get a single string out of all the text.
    $file_name = implode('-+-', $this->text);

    // Filenames longer than 200 characters will fail in most filesystems.
    if (drupal_strlen($file_name) > 200) {
      // Need to proceed with hash-based file names.
      _textimage_diag(
        t(
          "Textimage clear file name too long: @file_name...",
          array(
            '@file_name' => drupal_substr($file_name, 0, 60),
          )
        ),
        WATCHDOG_DEBUG,
        NULL,
        $this->userMessages
      );
      return FALSE;
    }

    // Strip control characters (ASCII value < 32). Though these are allowed
    // in some filesystems, not many applications handle them well. Also, strip
    // slashes and backslashes that usually indicate directories.
    $base_name = preg_replace('/[\x00-\x1F]|\\/|\\\\/u', '_', $file_name);
    if (drupal_substr(PHP_OS, 0, 3) == 'WIN') {
      // These characters are not allowed in Windows filenames.
      $base_name = str_replace(array(':', '*', '?', '"', '<', '>', '|'), '_', $base_name);
    }
    if ($file_name <> $base_name) {
      // Need to proceed with hash-based file names.
      _textimage_diag(
        t(
          "Textimage clear file name contains unallowed characters: @file_name...",
          array(
            '@file_name' => drupal_substr($file_name, 0, 60),
          )
        ),
        WATCHDOG_DEBUG,
        NULL,
        $this->userMessages
      );
      return FALSE;
    }

    $base_name = $file_name . '.' . $this->extension;
    $this->uri = $this->style['textimage']['uri_scheme'] . '://textimage/' . $this->style['name'] . '/' . $base_name;
    return TRUE;
  }

  /**
   * Get a cached Textimage.
   *
   * Cache and store are checked for existing image files.
   *
   * @return bool
   *   TRUE if an existing image file can be used, FALSE if no hit
   */
  protected function getCached() {

    // At first, check cache.
    if ($cached = cache_get('tiid:' . $this->id, 'cache_textimage')) {
      if (is_file($cached->data['uri'])) {
        $this->uri = $cached->data['uri'];
        return TRUE;
      }
    }

    // No cache. Check if we have the hash in store.
    $stored_image = db_select('textimage_store', 'ic')
        ->fields('ic')
        ->condition('tiid', $this->id, '=')
        ->execute()
        ->fetchAssoc();

    // Not in stock, return to make.
    if (!$stored_image) {
      return FALSE;
    }

    // In stock, check file is there.
    $uri = $stored_image['uri'];
    if (is_file($uri)) {
      $this->uri = $uri;
      $this->setCached();
      return TRUE;
    }
    else {
      return FALSE;
    }

  }

  /**
   * Cache image uri.
   *
   * @return self
   *   this object
   */
  protected function setCached() {
    $data = array(
      'uri' => $this->uri,
    );
    cache_set('tiid:' . $this->id, $data, 'cache_textimage');
    return $this;
  }

  /**
   * Store image details.
   */
  protected function putInStore() {
    $stored_image = array(
      'tiid' => $this->id,
      'is_void' => 0,
      'style_name' => $this->style ? (isset($this->style['name']) ? $this->style['name'] : NULL) : NULL,
      'uri' => $this->uri,
      'effects_outline' => $this->effects,
      'image_data' => $this->imageData,
      'timer' => $this->timer,
      'timestamp' => REQUEST_TIME,
    );
    try {
      drupal_write_record('textimage_store', $stored_image);
    }
    catch(Exception $error) {
      drupal_write_record('textimage_store', $stored_image, array('tiid'));
    }
  }

}

/**
 * Exception thrown by Textimage on failure.
 */
class TextimageException extends Exception {

  /**
   * Constructs a TextimageImagerTokenException object.
   */
  public function __construct($message) {
    parent::__construct(t("Textimage error: @message", array('@message' => $message)), 0);
  }

}
