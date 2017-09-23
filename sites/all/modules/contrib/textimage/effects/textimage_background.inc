<?php
/**
 * @file
 * Implementation of the 'textimage_background' image effect.
 */

/**
 * Default values for the textimage_background effect.
 *
 * @return array
 *   Effect default data.
 */
function _textimage_background_effect_defaults() {
  return array(
    'background_image' => array(
      'mode' => 'passthrough',
      'uri' => NULL,
      'fid' => 0,
    ),
    'background' => array(
      'color' => NULL,
      'repeat' => TRUE,
    ),
    'exact' => array(
      'width'    => '',
      'height'   => '',
      'xpos' => 'center',
      'ypos' => 'center',
      'dimensions' => 'scale',
      'crop' => 'center-center',
    ),
    'relative' => array(
      'leftdiff' => '',
      'rightdiff' => '',
      'topdiff' => '',
      'bottomdiff' => '',
    ),
  );
}

/**
 * Settings for 'textimage_background' image effect.
 *
 * Implements hook_form().
 *
 * @param array $data
 *   array of settings for this action
 *
 * @return array
 *   a form definition
 */
function textimage_background_effect_form($data) {

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_background_effect_defaults(), $data);

  $form = array();

  // Background image mode.
  $form['background_image'] = array(
    '#name' => 'background_image',
    '#type' => 'fieldset',
    '#title' => t('Background image'),
    '#description' => t('Select a background image to be used.'),
  );
  // Background image mode - options.
  $image_options = array(
    '' => t('No background image.'),
    'passthrough' => t('Inherit image from previous effect.'),
    'select' => t('Select image.'),
  );
  $form['background_image']['mode'] = array(
    '#type' => 'radios',
    '#options' => $image_options,
    '#default_value' => $data['background_image']['mode'],
  );
  // Background image mode - image selection.
  if (_textimage_get_variable('backgrounds_handling_module') == 'media') {
    // Media module available - use media form element.
    if (!isset($data['background_image']['fid'])) {
      $data['background_image']['fid'] = 0;
    }
    $scheme_options = array();
    foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
      $scheme_options[$scheme] = $scheme;
    }
    $form['background_image']['fid'] = array(
      '#title' => NULL,
      '#type' => 'media',
      '#media_options' => array(
        'global' => array(
          'types' => array('image'),
          'file_extensions' => 'png gif jpg jpeg',
          'schemes' => $scheme_options,
        ),
      ),
      '#description' => NULL,
      '#default_value' => array('fid' => $data['background_image']['fid']),
      '#states' => array(
        'visible' => array(
          ':input[name="data[background_image][mode]"]' => array('value' => 'select'),
        ),
      ),
    );
  }
  else {
    // Default - use dropdown with list of images.
    $image_files = drupal_map_assoc(_textimage_background_image_list(_textimage_get_variable('backgrounds_path')));
    if (empty($image_files)) {
      _textimage_diag(
        t(
          'No background images available. Make sure at least one image is available in the directory specified in the <a href="@url">configuration page</a>.',
          array(
            '@url' => url('admin/config/media/textimage'),
          )
        ),
        WATCHDOG_WARNING
      );
    }
    $form['background_image']['uri'] = array(
      '#type'  => 'select',
      '#options' => $image_files,
      '#default_value' => $data['background_image']['uri'] ? pathinfo($data['background_image']['uri'], PATHINFO_BASENAME) : '',
      '#states' => array(
        'visible' => array(
          ':input[name="data[background_image][mode]"]' => array('value' => 'select'),
        ),
      ),
    );
  }

  // Background color.
  $form['background'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => 'Background color',
    '#description'  => t('Select the color you wish to use for the background of the image. This color will be placed around the image or fill the background if no image is selected.'),
  );
  $form['background']['color'] = array(
    '#type' => 'textimage_color',
    '#title' => t('Color'),
    '#allow_transparent' => TRUE,
    '#allow_opacity' => TRUE,
    '#default_value' => $data['background']['color'],
  );
  $form['background']['repeat'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Use color for subsequent Textimage effects'),
    '#description' => t('If checked, this color will be used as a filler in case Textimage effects applied later need to extend the size of the image.'),
    '#default_value' => $data['background']['repeat'],
  );

  // Background image exact dimensions.
  $form['exact'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => 'Exact size',
    '#description'  => t('Set the background image to an exact size, either width or heigth. If only one of width or heigth is set, the other dimension will be automatically calculated based on resize/scale/crop options.'),
  );
  if (!$data['exact']['width'] && !$data['exact']['height']) {
    $form['exact']['#collapsed'] = TRUE;
  }
  $form['exact']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $data['exact']['width'],
    '#description' => t('Enter a value in pixels.'),
    '#size' => 5,
    '#field_suffix' => 'px',
  );
  $form['exact']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $data['exact']['height'],
    '#description' => t('Enter a value in pixels.'),
    '#size' => 5,
    '#field_suffix' => 'px',
  );
  $form['exact']['position'] = array(
    '#type' => 'radios',
    '#title' => t('Position'),
    '#options' => array(
      'left-top'      => t('Top') . ' ' . t('Left'),
      'center-top'    => t('Top') . ' ' . t('Center'),
      'right-top'     => t('Top') . ' ' . t('Right'),
      'left-center'   => t('Center') . ' ' . t('Left'),
      'center-center' => t('Center'),
      'right-center'  => t('Center') . ' ' . t('Right'),
      'left-bottom'   => t('Bottom') . ' ' . t('Left'),
      'center-bottom' => t('Bottom') . ' ' . t('Center'),
      'right-bottom'  => t('Bottom') . ' ' . t('Right'),
    ),
    '#theme' => 'image_anchor',
    '#default_value' => implode('-', array($data['exact']['xpos'], $data['exact']['ypos'])),
    '#description' => t('Position of the image on the resulting canvas, if the background image selected is smaller than the canvas.'),
  );
  $form['exact']['dimensions'] = array(
    '#type' => 'radios',
    '#title' => t('Image overflow'),
    '#default_value' => $data['exact']['dimensions'],
    '#options' => array(
      'scale' => t('Scale image to fit.'),
      'crop' => t('Crop image.'),
      'resize' => t('Resize image to fit - allowing possible distortion.'),
    ),
    '#description' => t('Action to take when the background image selected is bigger size than the canvas.'),
  );
  $form['exact']['crop'] = array(
    '#type' => 'radios',
    '#title' => t('Crop anchor'),
    '#options' => array(
      'left-top'      => t('Top') . ' ' . t('Left'),
      'center-top'    => t('Top') . ' ' . t('Center'),
      'right-top'     => t('Top') . ' ' . t('Right'),
      'left-center'   => t('Center') . ' ' . t('Left'),
      'center-center' => t('Center'),
      'right-center'  => t('Center') . ' ' . t('Right'),
      'left-bottom'   => t('Bottom') . ' ' . t('Left'),
      'center-bottom' => t('Bottom') . ' ' . t('Center'),
      'right-bottom'  => t('Bottom') . ' ' . t('Right'),
    ),
    '#theme' => 'image_anchor',
    '#default_value' => $data['exact']['crop'],
    '#description' => t('Select which part of the selected background image should be cropped.'),
    '#states' => array(
      'visible' => array(
        ':input[name="data[exact][dimensions]"]' => array('value' => 'crop'),
      ),
    ),
  );

  // Background image relative dimensions.
  $form['relative'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Relative size'),
    '#description' => t('Set the background image to a relative size, based on the original image dimensions. Use to add simple borders or expand by a fixed amount. Negative values will crop the image.'),
    'topdiff' => array(
      '#type' => 'textfield',
      '#title' => t('Top'),
      '#default_value' => $data['relative']['topdiff'],
      '#size' => 6,
      '#description' => t('Enter a value in pixels.'),
      '#field_suffix' => 'px',
    ),
    'rightdiff' => array(
      '#type' => 'textfield',
      '#title' => t('Right'),
      '#default_value' => $data['relative']['rightdiff'],
      '#size' => 6,
      '#description' => t('Enter a value in pixels.'),
      '#field_suffix' => 'px',
    ),
    'bottomdiff' => array(
      '#type' => 'textfield',
      '#title' => t('Bottom'),
      '#default_value' => $data['relative']['bottomdiff'],
      '#size' => 6,
      '#description' => t('Enter a value in pixels.'),
      '#field_suffix' => 'px',
    ),
    'leftdiff' => array(
      '#type' => 'textfield',
      '#title' => t('Left'),
      '#default_value' => $data['relative']['leftdiff'],
      '#size' => 6,
      '#description' => t('Enter a value in pixels.'),
      '#field_suffix' => 'px',
    ),
  );
  if (!$data['relative']['leftdiff'] && !$data['relative']['rightdiff'] && !$data['relative']['topdiff'] && !$data['relative']['bottomdiff']) {
    $form['relative']['#collapsed'] = TRUE;
  }

  $form['#element_validate'][] = 'textimage_background_effect_form_validate';

  return $form;
}

/**
 * Settings for 'textimage_background' image effect - form validation.
 */
function textimage_background_effect_form_validate($element, &$form_state) {
  $v = &$form_state['values']['data'];
  if ($v['background_image']['mode'] == 'select') {
    if (_textimage_get_variable('backgrounds_handling_module') == 'media') {
      if (empty($v['background_image']['fid']['fid'])) {
        form_set_error('background_image', t('Select an image, or choose another option for the background image.'));
        return;
      }
    }
    elseif (empty($v['background_image']['uri'])) {
      form_set_error('background_image', t('Select an image, or choose another option for the background image.'));
      return;
    }
  }
  if ($v['background_image']['mode'] <> 'select') {
    unset(
      $v['background_image']['fid'],
      $v['background_image']['uri']
    );
  }
  if (isset($v['background_image']['fid'])) {
    $v['background_image']['fid'] = $v['background_image']['fid']['fid'];
    $file = file_load($v['background_image']['fid']);
    $v['background_image']['uri'] = $file->uri;
  }
  if (!isset($v['background_image']['fid']) and isset($v['background_image']['uri'])) {
    $v['background_image']['uri'] = _textimage_get_variable('backgrounds_path') . '/' . $v['background_image']['uri'];
  }
  list($v['exact']['xpos'], $v['exact']['ypos']) = explode('-', $v['exact']['position']);
  unset ($v['exact']['position']);
}

/**
 * Renders 'textimage_background' image effect summary.
 *
 * Implements theme_hook().
 *
 * @param array $variables
 *   An associative array containing:
 *   - data: The current configuration of the image effect.
 *
 * @return string
 *   The HTML for the summary of the image effect.
 */
function theme_textimage_background_effect_summary($variables) {
  $output = NULL;

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_background_effect_defaults(), $variables['data']);

  switch ($data['background_image']['mode']) {
    case 'select':
      $output .= t('- Image: <b>@image</b>', array('@image' => $data['background_image']['uri']));
      break;

    case 'passthrough':
      $output .= t('- Inherit image from previous effect');
      break;

    case '':
    default:
      $output .= t('- No image');
  }

  // Sizing.
  if ($data['exact']['width'] or $data['exact']['height']) {
    // Exact size.
    $output = $output . ' ' . t('- Exact size:') . ' ';
    if ($data['exact']['width'] and $data['exact']['height']) {
      $output .= t(
        '@wx@h',
        array(
          '@w' => $data['exact']['width'] ? $data['exact']['width'] : t('src'),
          '@h' => $data['exact']['height'] ? $data['exact']['height'] : t('src'),
        )
      );
    }
    elseif ($data['exact']['width']) {
      $output .= t(
        'width @w',
        array(
          '@w' => $data['exact']['width'],
        )
      );
    }
    else {
      $output .= t(
        'height @h',
        array(
          '@h' => $data['exact']['height'],
        )
      );
    }
  }
  elseif ($data['relative']['leftdiff'] or $data['relative']['rightdiff'] or $data['relative']['topdiff'] or $data['relative']['bottomdiff']) {
    // Relative size.
    $output = $output . ' ' . t('- Relative size:') . ' ';
    $output .= $data['relative']['leftdiff'] ? (' ' . t('left: @size', array('@size' => $data['relative']['leftdiff']))) : NULL;
    $output .= $data['relative']['rightdiff'] ? (' ' . t('right: @size', array('@size' => $data['relative']['rightdiff']))) : NULL;
    $output .= $data['relative']['topdiff'] ? (' ' . t('top: @size', array('@size' => $data['relative']['topdiff']))) : NULL;
    $output .= $data['relative']['bottomdiff'] ? (' ' . t('bottom: @size', array('@size' => $data['relative']['bottomdiff']))) : NULL;
  }

  // Color.
  $output .= (' ' .  t('- Background color:') . ' ');
  if ($data['background']['color']) {
    $output .= theme(
      'textimage_colored_string',
      array(
        'text' => 'MMMM',
        'foreground_color' => $data['background']['color'],
        'background_color' => $data['background']['color'],
        'border' => TRUE,
        'border_color' => 'matchLuma',
      )
    );
  }
  else {
    $output .= (' ' . t('Transparent'));
  }
  $background_opacity = _textimage_rgba_to_opacity($data['background']['color']);
  if ($background_opacity <> 100) {
    $output .= (', ' . t('opacity: @background_opacity%', array('@background_opacity' => $background_opacity)));
  }

  return $output;
}

/**
 * Implements 'textimage_background' image effect callback.
 *
 * @param object $image
 *   An image object.
 *
 * @param array $data
 *   An array of attributes to use.
 *
 * @return bool
 *   true on success, false on failure to apply the effect.
 */
function textimage_background_effect(&$image, $data) {

  // Include toolkit specific image functions.
  if (!_textimage_load_toolkit_functions()) {
    return FALSE;
  }

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_background_effect_defaults(), $data);

  // Handle background image.
  switch ($data['background_image']['mode']) {
    // If a background image is selected, it will override any
    // image built thus far.
    case 'select':
      if (!isset($data['background_image']['uri'])) {
        _textimage_diag(t('Textimage could not find an image to load.'), WATCHDOG_ERROR, __FUNCTION__);
        return FALSE;
      }
      $new_image = image_load($data['background_image']['uri']);
      if ($new_image) {
        foreach ($new_image as $property => $value) {
          $image->$property = $value;
        }
      }
      else {
        _textimage_diag(t('Textimage failed loading image %image', array('%image' => $data['background_image']['uri'])), WATCHDOG_ERROR, __FUNCTION__);
        return FALSE;
      }
      break;

    // If passing through the image from the last effect, no special
    // treatment needed.
    case 'passthrough':
      break;

    // If explicitly requested not to have a background image, create
    // a transparent 1x1 image.
    case '':
    default:
      $new_image = image_toolkit_invoke('textimage_create_transparent', $image, array(
          1,
          1,
          TextimageImager::getState('gif_transparency_color'),
        )
      );
      if ($new_image) {
        foreach ($new_image as $property => $value) {
          $image->$property = $value;
        }
      }
      break;

  }

  $success = TRUE;

  // Handle exact sizing impacts on original image.
  if ($data['exact']['width'] || $data['exact']['height']) {
    // If current image is larger than size requested, call out to
    // scale/resize/crop effects to reduce image size depending
    // on options selected.
    if ($data['exact']['width'] <= $image->info['width'] || $data['exact']['height'] <= $image->info['height']) {

      switch ($data['exact']['dimensions']) {
        case 'scale':
          $scale_data = array(
            'width' => $data['exact']['width'],
            'height' => $data['exact']['height'],
            'upscale' => 0,
          );
          $success = image_scale_effect($image, $scale_data);
          break;

        case 'resize':
          $resize_data = array(
            'width' => $data['exact']['width'] ? $data['exact']['width'] : $image->info['width'],
            'height' => $data['exact']['height'] ? $data['exact']['height'] : $image->info['height'],
          );
          $success = image_resize_effect($image, $resize_data);
          break;

        case 'crop':
          $crop_data = array(
            'width' => min($data['exact']['width'], $image->info['width']),
            'height' => min($data['exact']['height'], $image->info['height']),
            'anchor' => $data['exact']['crop'],
          );
          $success = image_crop_effect($image, $crop_data);
          break;
      }

      if (!$success) {
        _textimage_diag(t('Textimage failed image processing.'), WATCHDOG_ERROR, __FUNCTION__);
        return FALSE;
      }
    }
  }

  // If resizing, call out to canvasactions_definecanvas_effect()
  // to finalise layout.
  if ($data['exact']['width'] || $data['exact']['height'] || $data['relative']['leftdiff'] || $data['relative']['rightdiff'] || $data['relative']['topdiff'] || $data['relative']['bottomdiff']) {
    $canvas_data = array(
      'RGB' => array(
        'HEX' => $data['background']['color'],
      ),
      'under' => TRUE,
      'exact' => array(
        'width' => $data['exact']['width'],
        'height' => $data['exact']['height'],
        'xpos' => $data['exact']['xpos'],
        'ypos' => $data['exact']['ypos'],
      ),
      'relative' => array(
        'leftdiff' => $data['relative']['leftdiff'],
        'rightdiff' => $data['relative']['rightdiff'],
        'topdiff' => $data['relative']['topdiff'],
        'bottomdiff' => $data['relative']['bottomdiff'],
      ),
    );

    if ($image->info['mime_type'] == 'image/gif') {
      // For .gif format, if transparent background set transparency color.
      if (!$canvas_data['RGB']['HEX']) {
        $canvas_data['RGB']['HEX'] = TextimageImager::getState('gif_transparency_color');
      }
      $success = canvasactions_definecanvas_effect($image, $canvas_data);
    }
    else {
      $success = canvasactions_definecanvas_effect($image, $canvas_data);
    }
  }

  if (!$success) {
    _textimage_diag(t('Textimage failed image processing.'), WATCHDOG_ERROR, __FUNCTION__);
    return FALSE;
  }

  // Stores background color for later effects.
  if ($data['background']['repeat']) {
    TextimageImager::setState('background_color', $data['background']['color']);
  }

  // Reset transparency color for .gif format.
  if ($image->info['mime_type'] == 'image/gif') {
    image_toolkit_invoke('textimage_set_transparency', $image, array(TextimageImager::getState('gif_transparency_color')));
  }

  return $success;
}

/**
 * Implements 'textimage_background' image dimensions callback.
 */
function textimage_background_effect_dimensions(array &$dimensions, array $data) {

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_background_effect_defaults(), $data);

  // If exact size WxH, set and return.
  if ($data['exact']['width'] and $data['exact']['height']) {
    $dimensions['width'] = $data['exact']['width'];
    $dimensions['height'] = $data['exact']['height'];
    return;
  }

  // Fetches WxH of the background image.
  switch ($data['background_image']['mode']) {
    case 'select':
      if (!isset($data['background_image']['uri'])) {
        _textimage_diag(t('Textimage could not find an image to load.'), WATCHDOG_ERROR, __FUNCTION__);
        return;
      }
      $new_image = image_load($data['background_image']['uri']);
      if (!$new_image) {
        _textimage_diag(t('Textimage failed loading image %image', array('%image' => $data['background_image']['uri'])), WATCHDOG_ERROR, __FUNCTION__);
        return;
      }
      $width = $new_image->info['width'];
      $height = $new_image->info['height'];
      break;

    // If passing through the image from the last effect, retain
    // the size.
    case 'passthrough':
      $width = $dimensions['width'];
      $height = $dimensions['height'];
      break;

    // If explicitly requested not to have a background image, WxH is 1x1.
    case '':
    default:
      $width = $height = 1;
      break;

  }

  if ($data['exact']['width'] or $data['exact']['height']) {
    // Handle exact sizing.
    if ($data['exact']['width'] <= $width or $data['exact']['height'] <= $height) {
      // If current image is larger than size requested, call out to
      // scale/resize/crop effects to reduce image size depending
      // on options selected.
      switch ($data['exact']['dimensions']) {
        case 'scale':
          $scale_data = array(
            'width' => $data['exact']['width'],
            'height' => $data['exact']['height'],
            'upscale' => 0,
          );
          image_scale_dimensions($dimensions, $scale_data);
          return;

        case 'resize':
          $resize_data = array(
            'width' => $data['exact']['width'] ? $data['exact']['width'] : $width,
            'height' => $data['exact']['height'] ? $data['exact']['height'] : $height,
          );
          image_resize_dimensions($dimensions, $resize_data);
          return;

        case 'crop':
          $crop_data = array(
            'width' => min($data['exact']['width'], $width),
            'height' => min($data['exact']['height'], $height),
            'anchor' => $data['exact']['crop'],
          );
          image_resize_dimensions($dimensions, $crop_data);
          return;

      }
      _textimage_diag(t('Textimage image dimensions resizing failed.'), WATCHDOG_ERROR, __FUNCTION__);
      return FALSE;
    }
  }
  elseif ($data['relative']['leftdiff'] or $data['relative']['rightdiff'] or $data['relative']['topdiff'] or $data['relative']['bottomdiff']) {
    // Handle relative size.
    $dimensions['width'] = $width + $data['relative']['leftdiff'] + $data['relative']['rightdiff'];
    $dimensions['height'] = $height + $data['relative']['topdiff'] + $data['relative']['bottomdiff'];
    return;
  }
}

/**
 * Returns an array of files with image extensions in the specified directory.
 *
 * @param string $images_dir
 *   URL of the images directory.
 *
 * @return array
 *   Array of image files.
 */
function _textimage_background_image_list($images_dir) {
  $filelist = array();
  if (is_dir($images_dir) && $handle = opendir($images_dir)) {
    while ($file = readdir($handle)) {
      if (preg_match("/\.gif|\.png|\.jpg$/i", $file) == 1) {
        $filelist[] = $file;
      }
    }
    closedir($handle);
  }
  return $filelist;
}
