<?php
/**
 * @file
 * Implementation of the 'textimage_text' image effect.
 */

/**
 * Default values for the textimage_text effect.
 *
 * @return array
 *   Effect default data.
 */
function _textimage_text_effect_defaults() {
  $default_font = _textimage_get_variable('default_font');
  return array(
    'font'          => array(
      'name'                  => $default_font['name'],
      'uri'                   => $default_font['uri'],
      'size'                  => 16,
      'angle'                 => 0,
      'color'                 => '#00000000',
      'stroke_mode'           => 'outline',
      'stroke_color'          => '#00000000',
      'outline_top'           => 0,
      'outline_right'         => 0,
      'outline_bottom'        => 0,
      'outline_left'          => 0,
      'shadow_x_offset'       => 1,
      'shadow_y_offset'       => 1,
      'shadow_width'          => 0,
      'shadow_height'         => 0,
    ),
    'layout'       => array(
      'padding_top'           => 0,
      'padding_right'         => 0,
      'padding_bottom'        => 0,
      'padding_left'          => 0,
      'x_pos'                 => 'center',
      'y_pos'                 => 'center',
      'x_offset'              => 0,
      'y_offset'              => 0,
      'background_color'      => NULL,
      'overflow_action'       => 'extend',
    ),
    'text' => array(
      'maximum_width'         => 0,
      'fixed_width'           => FALSE,
      'align'                 => 'left',
      'line_spacing'          => 0,
      'case_format'           => '',
    ),
    'text_string'             => t('Preview'),
  );
}

/**
 * Settings for 'textimage_text' image effect.
 *
 * Implements hook_form().
 *
 * @param array $data
 *   The current configuration for this image effect.
 *
 * @return array
 *   The form definition for this effect.
 */
function textimage_text_effect_form($data) {

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_text_effect_defaults(), $data);

  $form = array();

  // Preview effect.
  $data['preview_bar']['debug_visuals'] = isset($data['preview_bar']['debug_visuals']) ? $data['preview_bar']['debug_visuals'] : FALSE;
  $form['preview'] = array(
    '#type'   => 'item',
    '#markup' => '<strong>' . t('Preview') . ":</strong>\n" . '<div id="textimage-preview">' . _textimage_text_effect_preview_image($data) . '</div>',
  );
  // Preview bar.
  $form['preview_bar'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
  );
  // Refresh button.
  $form['preview_bar']['preview'] = array(
    '#type'  => 'button',
    '#value' => t('Refresh preview'),
    '#ajax'  => array(
      'callback' => '_textimage_text_effect_form_ajax',
    ),
  );
  // Visual aids.
  $form['preview_bar']['debug_visuals'] = array(
    '#type' => 'checkbox',
    '#title' => t('Visual aids in preview'),
    '#default_value' => FALSE,
  );

  // Settings.
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // ---- Text default.
  $form['settings']['text_default'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Text default'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['settings']['text_default']['text_string'] = array(
    '#type'  => 'textarea',
    '#title' => t('Default text'),
    '#default_value' => $data['text_string'],
    '#description' => t('Enter the default text string for this effect. You can also enter tokens, that Textimage will resolve when applying the effect.'),
    '#rows' => 3,
    '#required' => TRUE,
  );
  if (_textimage_module_exists('token', TEXTIMAGE_TOKEN_MIN_VERSION)) {
    $form['settings']['text_default']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node', 'user', 'file', 'textimage'),
      '#global_types' => TRUE,
      '#click_insert' => TRUE,
    );
  }

  // ---- Font settings.
  $form['settings']['font'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Font settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $font_options = drupal_map_assoc(TextimageFonts::getList());
  $form['settings']['font']['name'] = array(
    '#type'    => 'select',
    '#title'   => t('Font'),
    '#options' => $font_options,
    '#default_value' => $data['font']['name'],
    '#description' => t('Select the font to be used in this image. If no fonts are listed, check the <a href="!path">settings for Fonts</a>.', array('!path' => url('admin/config/media/textimage/settings'))),
    '#required'    => TRUE,
  );
  $form['settings']['font']['size'] = array(
    '#type'  => 'textfield',
    '#title' => t('Size'),
    '#description'   => t('Enter the size of the text to be generated.'),
    '#default_value' => $data['font']['size'],
    '#maxlength' => 5,
    '#size' => 3,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );
  $form['settings']['font']['angle'] = array(
    '#type'  => 'textfield',
    '#title' => t('Rotation'),
    '#maxlength' => 4,
    '#size' => 4,
    '#field_suffix' => t('&deg;'),
    '#description' => t('Enter the angle in degrees at which the text will be displayed. Positive numbers rotate the text clockwise, negative numbers counter-clockwise.'),
    '#default_value' => $data['font']['angle'],
    '#element_validate' => array('element_validate_number'),
  );
  $form['settings']['font']['color'] = array(
    '#type' => 'textimage_color',
    '#title' => t('Font color'),
    '#description'  => t('Set the font color.'),
    '#allow_opacity' => TRUE,
    '#default_value' => $data['font']['color'],
  );
  // Outline.
  $form['settings']['font']['stroke'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Outline / Shadow'),
    '#description'   => t('Optionally add an outline or shadow around the font. Enter the information in pixels.'),
  );
  $stroke_options = array(
    'outline' => t('Outline'),
    'shadow' => t('Shadow'),
  );
  $form['settings']['font']['stroke']['mode'] = array(
    '#type'    => 'radios',
    '#title'   => t('Mode'),
    '#options' => $stroke_options,
    '#default_value' => $data['font']['stroke_mode'],
  );
  $form['settings']['font']['stroke']['top'] = array(
    '#type' => 'textfield',
    '#title' => t('Top'),
    '#default_value' => $data['font']['outline_top'],
    '#maxlength' => 2,
    '#size' => 3,
    '#field_suffix' => 'px',
    '#element_validate' => array('element_validate_integer'),
    '#states' => array(
      'visible' => array(
        ':radio[name="data[settings][font][stroke][mode]"]' => array('value' => 'outline'),
      ),
    ),
  );
  $form['settings']['font']['stroke']['right'] = array(
    '#type' => 'textfield',
    '#title' => t('Right'),
    '#default_value' => $data['font']['outline_right'],
    '#maxlength' => 2,
    '#size' => 3,
    '#field_suffix' => 'px',
    '#element_validate' => array('element_validate_integer'),
    '#states' => array(
      'visible' => array(
        ':radio[name="data[settings][font][stroke][mode]"]' => array('value' => 'outline'),
      ),
    ),
  );
  $form['settings']['font']['stroke']['bottom'] = array(
    '#type' => 'textfield',
    '#title' => t('Bottom'),
    '#default_value' => $data['font']['outline_bottom'],
    '#maxlength' => 2,
    '#size' => 3,
    '#field_suffix' => 'px',
    '#element_validate' => array('element_validate_integer'),
    '#states' => array(
      'visible' => array(
        ':radio[name="data[settings][font][stroke][mode]"]' => array('value' => 'outline'),
      ),
    ),
  );
  $form['settings']['font']['stroke']['left'] = array(
    '#type' => 'textfield',
    '#title' => t('Left'),
    '#default_value' => $data['font']['outline_left'],
    '#maxlength' => 2,
    '#size' => 3,
    '#field_suffix' => 'px',
    '#element_validate' => array('element_validate_integer'),
    '#states' => array(
      'visible' => array(
        ':radio[name="data[settings][font][stroke][mode]"]' => array('value' => 'outline'),
      ),
    ),
  );
  $form['settings']['font']['stroke']['x_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Horizontal offset'),
    '#default_value' => $data['font']['shadow_x_offset'],
    '#maxlength' => 3,
    '#size' => 3,
    '#field_suffix' => 'px',
    '#element_validate' => array('element_validate_integer'),
    '#states' => array(
      'visible' => array(
        ':radio[name="data[settings][font][stroke][mode]"]' => array('value' => 'shadow'),
      ),
    ),
  );
  $form['settings']['font']['stroke']['y_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical offset'),
    '#default_value' => $data['font']['shadow_y_offset'],
    '#maxlength' => 3,
    '#size' => 3,
    '#field_suffix' => 'px',
    '#element_validate' => array('element_validate_integer'),
    '#states' => array(
      'visible' => array(
        ':radio[name="data[settings][font][stroke][mode]"]' => array('value' => 'shadow'),
      ),
    ),
  );
  $form['settings']['font']['stroke']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Horizontal elongation'),
    '#default_value' => $data['font']['shadow_width'],
    '#maxlength' => 2,
    '#size' => 3,
    '#field_suffix' => 'px',
    '#element_validate' => array('element_validate_integer'),
    '#states' => array(
      'visible' => array(
        ':radio[name="data[settings][font][stroke][mode]"]' => array('value' => 'shadow'),
      ),
    ),
  );
  $form['settings']['font']['stroke']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical elongation'),
    '#default_value' => $data['font']['shadow_height'],
    '#maxlength' => 2,
    '#size' => 3,
    '#field_suffix' => 'px',
    '#element_validate' => array('element_validate_integer'),
    '#states' => array(
      'visible' => array(
        ':radio[name="data[settings][font][stroke][mode]"]' => array('value' => 'shadow'),
      ),
    ),
  );
  $form['settings']['font']['stroke']['color'] = array(
    '#type' => 'textimage_color',
    '#title' => t('Color'),
    '#description'  => t('Set the outline/shadow color.'),
    '#allow_opacity' => TRUE,
    '#default_value' => $data['font']['stroke_color'],
  );

  // ---- Text settings.
  $form['settings']['text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Inner width.
  $form['settings']['text']['maximum_width'] = array(
    '#type'  => 'textfield',
    '#title' => t('Maximum width'),
    '#field_suffix' => t('px'),
    '#description' => t('Maximum width of the text image, inclusive of padding. Text lines wider than this will be wrapped. Leave blank to disable wrapping. <b>Note:</b> in case of rotation, the width of the final image rendered will differ, to accommodate the rotation. If you need a strict width/height, add image resize/scale/crop effects afterwards.'),
    '#default_value' => $data['text']['maximum_width'],
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('element_validate_integer'),
  );
  $form['settings']['text']['fixed_width'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Fixed width?'),
    '#description' => t('If checked, the width will always be equal to the maximum width.'),
    '#default_value' => $data['text']['fixed_width'],
    '#states' => array(
      'visible' => array(
        ':input[name="data[settings][text][maximum_width]"]' => array('filled' => TRUE),
      ),
    ),
  );
  // Text alignment.
  $form['settings']['text']['align'] = array(
    '#type'  => 'select',
    '#title' => t('Text alignment'),
    '#options' => array(
      'left' => t('Left'),
      'center' => t('Center'),
      'right' => t('Right'),
    ),
    '#default_value' => $data['text']['align'],
    '#description' => t('Select how the text should be aligned within the resulting image. The default aligns to the left.'),
  );
  // Line spacing (Leading).
  $form['settings']['text']['line_spacing'] = array(
    '#type'  => 'textfield',
    '#title' => t('Line spacing (Leading)'),
    '#field_suffix'  => t('px'),
    '#default_value' => $data['text']['line_spacing'],
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('element_validate_integer'),
    '#description' => t('Specify the space in pixels to be added between text lines (Leading).'),
  );
  $form['settings']['text']['case_format'] = array(
    '#type'  => 'select',
    '#title' => t('Case format'),
    '#options' => array(
      '' => t('Default'),
      'upper' => t('UPPERCASE'),
      'lower' => t('lowercase'),
      'ucwords' => t('Uppercase Words'),
      'ucfirst' => t('Uppercase first'),
    ),
    '#description' => t('Convert the input text to a desired format. The default makes no changes to input text.'),
    '#default_value' => $data['text']['case_format'],
  );

  // ---- Layout settings.
  $form['settings']['layout'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Layout settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Position.
  $form['settings']['layout']['position'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Position'),
  );
  $form['settings']['layout']['position']['placement'] = array(
    '#type' => 'radios',
    '#title' => t('Placement'),
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
    '#default_value' => implode('-', array($data['layout']['x_pos'], $data['layout']['y_pos'])),
    '#description' => t('Position of the text on the underlying image.'),
  );
  $form['settings']['layout']['position']['x_offset'] = array(
    '#type'  => 'textfield',
    '#title' => t('Horizontal offset'),
    '#field_suffix'  => 'px',
    '#description'   => t('Additional horizontal offset from placement.'),
    '#default_value' => $data['layout']['x_offset'],
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('element_validate_integer'),
  );
  $form['settings']['layout']['position']['y_offset'] = array(
    '#type'  => 'textfield',
    '#title' => t('Vertical offset'),
    '#field_suffix'  => 'px',
    '#description'   => t('Additional vertical offset from placement.'),
    '#default_value' => $data['layout']['y_offset'],
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('element_validate_integer'),
  );
  // Overflow action.
  $form['settings']['layout']['position']['overflow_action'] = array(
    '#type' => 'radios',
    '#title' => t('Overflow'),
    '#default_value' => $data['layout']['overflow_action'],
    '#options' => array(
      'extend' => t('<b>Extend image.</b> The underlying image will be extended to fit the text.'),
      'crop' => t('<b>Crop text.</b> Only the part of the text fitting in the image is rendered.'),
      'scaletext' => t('<b>Scale text.</b> The text will be scaled to fit the underlying image.'),
    ),
    '#description' => t('Action to take if text overflows the underlying image.'),
  );
  // Padding.
  $form['settings']['layout']['padding'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Padding'),
    '#description' => t('Specify the padding in pixels to be added around the generated text.'),
  );
  $form['settings']['layout']['padding']['top'] = array(
    '#type'  => 'textfield',
    '#title' => t('Top'),
    '#field_suffix'  => t('px'),
    '#default_value' => $data['layout']['padding_top'],
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('element_validate_integer'),
  );
  $form['settings']['layout']['padding']['right'] = array(
    '#type'  => 'textfield',
    '#title' => t('Right'),
    '#field_suffix'  => t('px'),
    '#default_value' => $data['layout']['padding_right'],
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('element_validate_integer'),
  );
  $form['settings']['layout']['padding']['bottom'] = array(
    '#type'  => 'textfield',
    '#title' => t('Bottom'),
    '#field_suffix'  => t('px'),
    '#default_value' => $data['layout']['padding_bottom'],
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('element_validate_integer'),
  );
  $form['settings']['layout']['padding']['left'] = array(
    '#type'  => 'textfield',
    '#title' => t('Left'),
    '#field_suffix'  => t('px'),
    '#default_value' => $data['layout']['padding_left'],
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('element_validate_integer'),
  );
  // Background color.
  $form['settings']['layout']['background_color'] = array(
    '#type' => 'textimage_color',
    '#title' => t('Background color'),
    '#description'  => t('Select the color you wish to use for the background of the text.'),
    '#allow_transparent' => TRUE,
    '#allow_opacity' => TRUE,
    '#default_value' => $data['layout']['background_color'],
  );

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'textimage') . '/misc/css/textimage.admin.css',
  );
  $form['#element_validate'][] = 'textimage_text_effect_form_validate';

  return $form;
}

/**
 * Settings for 'textimage_text' image effect - form validation.
 */
function textimage_text_effect_form_validate($form, &$form_state) {

  $form_values = $form_state['values']['data']['settings'];

  // Get x-y position from the anchor element.
  list($form_values['layout']['position']['x_pos'], $form_values['layout']['position']['y_pos']) = explode('-', $form_values['layout']['position']['placement']);
  unset ($form_values['layout']['position']['placement']);

  // Get the font URI.
  $font_uri = TextimageFonts::getUri($form_values['font']['name']);

  $form_state['values']['data'] = array(
    'font'   => array(
      'name'                 => $form_values['font']['name'],
      'uri'                  => $font_uri,
      'size'                 => $form_values['font']['size'],
      'angle'                => $form_values['font']['angle'],
      'color'                => $form_values['font']['color'],
      'stroke_mode'          => $form_values['font']['stroke']['mode'],
      'stroke_color'         => $form_values['font']['stroke']['color'],
      'outline_top'          => $form_values['font']['stroke']['top'],
      'outline_right'        => $form_values['font']['stroke']['right'],
      'outline_bottom'       => $form_values['font']['stroke']['bottom'],
      'outline_left'         => $form_values['font']['stroke']['left'],
      'shadow_x_offset'      => $form_values['font']['stroke']['x_offset'],
      'shadow_y_offset'      => $form_values['font']['stroke']['y_offset'],
      'shadow_width'         => $form_values['font']['stroke']['width'],
      'shadow_height'        => $form_values['font']['stroke']['height'],
    ),
    'layout' => array(
      'padding_top'          => $form_values['layout']['padding']['top'],
      'padding_right'        => $form_values['layout']['padding']['right'],
      'padding_bottom'       => $form_values['layout']['padding']['bottom'],
      'padding_left'         => $form_values['layout']['padding']['left'],
      'x_pos'                => $form_values['layout']['position']['x_pos'],
      'y_pos'                => $form_values['layout']['position']['y_pos'],
      'x_offset'             => $form_values['layout']['position']['x_offset'],
      'y_offset'             => $form_values['layout']['position']['y_offset'],
      'overflow_action'      => $form_values['layout']['position']['overflow_action'],
      'background_color'     => $form_values['layout']['background_color'],
    ),
    'text'   => array(
      'maximum_width'        => $form_values['text']['maximum_width'],
      'fixed_width'          => $form_values['text']['fixed_width'],
      'align'                => $form_values['text']['align'],
      'case_format'          => $form_values['text']['case_format'],
      'line_spacing'         => $form_values['text']['line_spacing'],
    ),
    'text_string'            => $form_values['text_default']['text_string'],
  );
}

/**
 * AJAX callback to refresh the Textimage preview.
 */
function _textimage_text_effect_form_ajax($form, $form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_html('#textimage-preview', _textimage_text_effect_preview_image($form_state['values']['data'])),
    ),
  );
}

/**
 * Deliver a preview of the textimage with the current settings.
 *
 * @param array $data
 *   The current configuration for this image effect.
 *
 * @return array
 *   The HTML to the preview image.
 */
function _textimage_text_effect_preview_image($data) {
  $data['layout']['x_pos'] = 'center';
  $data['layout']['y_pos'] = 'center';
  $data['layout']['x_offset'] = 0;
  $data['layout']['y_offset'] = 0;
  $data['layout']['overflow_action'] = 'extend';
  $data['debug_visuals'] = $data['preview_bar']['debug_visuals'];
  return theme('textimage_direct_image', array(
      'text' => array($data['text_string']),
      'effects' => array(
        array(
          'name' => 'textimage_text',
          'data' => $data,
        ),
      ),
      'title' => t('Preview'),
      'alt' => t('Display preview not available.'),
      'caching' => FALSE,
    )
  );
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
function theme_textimage_text_effect_summary($variables) {

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_text_effect_defaults(), $variables['data']);

  $output = ' - ' . t('Font:') . ' ' . $data['font']['name'];
  $output .= (' - ' . t('Size: @size', array('@size' => $data['font']['size'])));

  if ($data['font']['angle']) {
    $output = $output . ' ' . t('- Rotate: @angle°', array(
      '@angle' => $data['font']['angle'],
    ));
  }

  $output .= (' - ' . t('Color:') . ' ' . theme(
    'textimage_colored_string',
    array(
      'text' => t('Sample'),
      'foreground_color' => $data['font']['color'],
      'background_color' => $data['layout']['background_color'],
    )
  ));
  $background_opacity = _textimage_rgba_to_opacity($data['layout']['background_color']);
  $font_opacity = _textimage_rgba_to_opacity($data['font']['color']);
  $stroke_opacity = _textimage_rgba_to_opacity($data['font']['stroke_color']);
  if ($font_opacity <> 100 || $stroke_opacity <> 100 || $background_opacity <> 100) {
    $output .= (
      ', ' . t(
        'opacity: bg @background_opacity%, fg @font_opacity%',
        array(
          '@background_opacity' => $background_opacity,
          '@font_opacity' => $font_opacity,
        )
      )
    );
    if ($stroke_opacity && $stroke_opacity <> 100) {
      $output .= (
        ', ' . t(
          '@stroke_mode @stroke_opacity%',
          array(
            '@stroke_mode' => ($data['font']['stroke_mode'] == 'outline') ? t('outline') : t('shadow'),
            '@stroke_opacity' => $stroke_opacity,
          )
        )
      );
    }
  }

  return $output;
}

/**
 * Implements 'textimage_text' image effect callback.
 *
 * Overlay text on an image resource.
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
function textimage_text_effect($image, $data) {

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_text_effect_defaults(), $data);

  // Get the text wrapper resource.
  if (!$wrapper = _textimage_get_text_wrapper($image, $data)) {
    return FALSE;
  }

  // Background image dimensions.
  $image_width = $image->info['width'];
  $image_height = $image->info['height'];

  // Offset wrapper dimensions.
  $offset_wrapper = array();

  // Determine needed resizing/repositioning of background image and/or
  // text wrapper image, based on the settings.
  switch ($data['layout']['overflow_action']) {
    case 'extend':
      // The background image new dimensions, after extension.
      $image_new = array(
        'xpos' => 0,
        'ypos' => 0,
        'width' => $image_width,
        'height' => $image_height,
      );

      // The size of the frame sides for color filling.
      $frame = array(
        'top' => 0,
        'right' => 0,
        'bottom' => 0,
        'left' => 0,
      );

      // Check wrapper image overflowing the original image.
      list($resized, $offset_wrapper) = _textimage_background_image_resize($image, $wrapper, $data, $image_new, $frame);
      if ($resized) {
        // Call out to canvasactions_definecanvas_effect(), transparent
        // background.
        $canvas_data = array(
          'RGB' => array(
            'HEX' => NULL,
          ),
          'under' => TRUE,
          'exact' => $image_new,
        );
        if (!canvasactions_definecanvas_effect($image, $canvas_data)) {
          return FALSE;
        }
        // Color fill the frame with carried on background color.
        if ($main_bg_color = TextimageImager::getState('background_color')) {
          // Top rectangle.
          if ($frame['top']) {
            $points = array(
              0, 0,
              $image_new['width'] - 1, 0,
              $image_new['width'] - 1, $frame['top'] - 1,
              0, $frame['top'] - 1,
            );
            _textimage_draw_rectangle($image, $points, $main_bg_color);
          }
          // Bottom rectangle.
          if ($frame['bottom']) {
            $points = array(
              0, $image_height + $frame['top'],
              $image_new['width'] - 1, $image_height + $frame['top'],
              $image_new['width'] - 1, $image_new['height'] - 1,
              0, $image_new['height'] - 1,
            );
            _textimage_draw_rectangle($image, $points, $main_bg_color);
          }
          // Left rectangle.
          if ($frame['left']) {
            $points = array(
              0, $frame['top'],
              $frame['left'] - 1, $frame['top'],
              $frame['left'] - 1, $frame['top'] + $image_height - 1,
              0, $frame['top'] + $image_height - 1,
            );
            _textimage_draw_rectangle($image, $points, $main_bg_color);
          }
          // Right rectangle.
          if ($frame['right']) {
            $points = array(
              $frame['left'] + $image_width, $frame['top'],
              $image_new['width'] - 1, $frame['top'],
              $image_new['width'] - 1, $frame['top'] + $image_height - 1,
              $frame['left'] + $image_width, $frame['top'] + $image_height - 1,
            );
            _textimage_draw_rectangle($image, $points, $main_bg_color);
          }
        }
      }
      break;

    case 'scaletext':
      // Check if scaling down is needed.
      list($resized, $offset_wrapper) = _textimage_wrapper_resize($image, $wrapper, $data);
      if ($resized) {
        $scale_data = array(
          'width' => $offset_wrapper['width'],
          'height' => $offset_wrapper['height'],
          'upscale' => 0,
        );
        if (!image_scale_effect($wrapper, $scale_data)) {
          return FALSE;
        }
      }
      break;

    case 'crop':
    default:
      // Nothing to do, just place the wrapper at offset required.
      $x_offset = ceil(image_filter_keyword($data['layout']['x_pos'], $image_width, $wrapper->info['width']));
      $y_offset = ceil(image_filter_keyword($data['layout']['y_pos'], $image_height, $wrapper->info['height']));
      $offset_wrapper['xpos'] = $x_offset + $data['layout']['x_offset'];
      $offset_wrapper['ypos'] = $y_offset + $data['layout']['y_offset'];
      break;

  }

  // Finally, lay the wrapper over the source image.
  if (!empty($offset_wrapper)) {
    if (!image_overlay($image, $wrapper, $offset_wrapper['xpos'], $offset_wrapper['ypos'])) {
      return FALSE;
    }
  }

  // Reset transparency color for .gif format.
  if ($image->info['mime_type'] == 'image/gif') {
    image_toolkit_invoke('textimage_set_transparency', $image, array(TextimageImager::getState('gif_transparency_color')));
  }

  return TRUE;
}

/**
 * Implements 'textimage_text' image dimensions callback.
 */
function textimage_text_effect_dimensions(array &$dimensions, array $data) {

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_text_effect_defaults(), $data);

  // Dimensions are potentially affected only if the effect is set to
  // autoextend the background image in case of wrapper overflow.
  if ($data['layout']['overflow_action'] == 'extend') {

    // Dummy image object.
    $image = new stdClass();
    $image->info['width'] = $dimensions['width'];
    $image->info['height'] = $dimensions['height'];
    $image->info['extension'] = 'png';
    $image->info['mime_type'] = 'image/png';
    $image->toolkit = image_get_toolkit();

    // Get the text wrapper resource.
    if (!$wrapper = _textimage_get_text_wrapper($image, $data)) {
      return;
    }

    // The background image new dimensions, after extension.
    $image_new = array(
      'xpos' => 0,
      'ypos' => 0,
      'width' => $image->info['width'],
      'height' => $image->info['height'],
    );

    // Checks if resizing needed.
    list($resized, $offset_wrapper) = _textimage_background_image_resize($image, $wrapper, $data, $image_new);
    if ($resized) {
      $dimensions['width'] = $image_new['width'];
      $dimensions['height'] = $image_new['height'];
    }

  }
}

/**
 * Get the image containing the text.
 *
 * This is separated from textimage_text_effect() so that it can also be used
 * by the textimage_text_effect_dimensions() function.
 */
function _textimage_get_text_wrapper($image, array $data) {

  // Include toolkit specific image functions.
  if (!_textimage_load_toolkit_functions()) {
    return NULL;
  }

  // Check font path.
  $font_uri = $data['font']['uri'];
  $data['font']['uri'] = _textimage_get_font_path($image, $data['font']['uri']);
  if (!$data['font']['uri']) {
    _textimage_diag(
      t(
        "Textimage could not find the font file @fontfile for font @fontname",
        array(
          '@fontfile' => $font_uri,
          '@fontname' => $data['font']['name'],
        )
      ),
      WATCHDOG_ERROR,
      __FUNCTION__);
    return NULL;
  }

  // If the effect is executed outside of the context of the TextimageImager
  // class (e.g. by the core Image module), then the text_string has not been
  // pre-processed to translate tokens or apply text conversion.
  if (!(TextimageImager::getState('building_module') == 'textimage')) {
    $data['text_string'] = TextimageImager::processTextString($data['text_string'], $data['text']['case_format']);
  }

  // Determine if outline/shadow is required.
  $outline = $shadow = FALSE;
  if ($data['font']['stroke_mode'] == 'outline' && ($data['font']['outline_top'] || $data['font']['outline_right'] || $data['font']['outline_bottom'] || $data['font']['outline_left']) && $data['font']['stroke_color']) {
    $outline = TRUE;
  }
  elseif ($data['font']['stroke_mode'] == 'shadow' && ($data['font']['shadow_x_offset'] || $data['font']['shadow_y_offset'] || $data['font']['shadow_width'] || $data['font']['shadow_height']) && $data['font']['stroke_color']) {
    $shadow = TRUE;
  }

  // Add stroke to padding to ensure inner box includes entire font space.
  if ($outline) {
    $data['layout']['padding_top'] += $data['font']['outline_top'];
    $data['layout']['padding_right'] += $data['font']['outline_right'];
    $data['layout']['padding_bottom'] += $data['font']['outline_bottom'];
    $data['layout']['padding_left'] += $data['font']['outline_left'];
  }
  elseif ($shadow) {
    $data['layout']['padding_top'] += ($data['font']['shadow_y_offset'] < 0 ? -$data['font']['shadow_y_offset'] : 0);
    $data['layout']['padding_right'] += ($data['font']['shadow_x_offset'] > 0 ? $data['font']['shadow_x_offset'] : 0);
    $data['layout']['padding_bottom'] += ($data['font']['shadow_y_offset'] > 0 ? $data['font']['shadow_y_offset'] : 0);
    $data['layout']['padding_left'] += ($data['font']['shadow_x_offset'] < 0 ? -$data['font']['shadow_x_offset'] : 0);
    $shadow_width = ($data['font']['shadow_x_offset'] != 0) ? $data['font']['shadow_width'] + 1 : $data['font']['shadow_width'];
    $shadow_height = ($data['font']['shadow_y_offset'] != 0) ? $data['font']['shadow_height'] + 1 : $data['font']['shadow_height'];
    $net_right = $shadow_width + ($data['font']['shadow_x_offset'] >= 0 ? 0 : $data['font']['shadow_x_offset']);
    $data['layout']['padding_right'] += ($net_right > 0 ? $net_right : 0);
    $net_bottom = $shadow_height + ($data['font']['shadow_y_offset'] >= 0 ? 0 : $data['font']['shadow_y_offset']);
    $data['layout']['padding_bottom'] += ($net_bottom > 0 ? $net_bottom : 0);
  }

  // Perform text wrapping, if necessary.
  if ($data['text']['maximum_width'] > 0) {
    $data['text_string'] = _textimage_wrap_text(
      $image,
      $data['text_string'],
      $data['font']['size'],
      $data['font']['uri'],
      $data['text']['maximum_width'] - $data['layout']['padding_left'] - $data['layout']['padding_right'] - 1,
      $data['text']['align']
    );
  }

  // Load text lines to array elements.
  $text_lines = explode("\n", $data['text_string']);
  $num_lines = count($text_lines);

  // Calculate bounding boxes.
  // --------------------------------------------------------------------
  // Inner box: the exact bounding box of the text.
  // Outer box: the box where the inner box is - can be different because
  // of padding.
  // Wrapper:the canvass where the outer box is laid.
  // --------------------------------------------------------------------
  //
  // Get inner box, for horizontal text, unpadded.
  $inner_box = _textimage_get_bounding_box($image, $data['text_string'], $num_lines, $data['font']['size'], $data['font']['uri']);

  // Adjust to fixed width, if requested.
  if ($data['text']['fixed_width'] && !empty($data['text']['maximum_width'])) {
    $inner_box->set('width', $data['text']['maximum_width'] - $data['layout']['padding_left'] - $data['layout']['padding_right']);
  }

  // Determine average text line height.
  $line_height = round($inner_box->get('height') / $num_lines);

  // Manage leading (line spacing), adding total line spacing to height.
  if ($data['text']['line_spacing']) {
    $inner_box->set('height', $inner_box->get('height') + ($data['text']['line_spacing'] * ($num_lines - 1)));
  }

  // Apply padding to get outer box.
  $outer_box = clone $inner_box;
  $outer_box->set('width', $outer_box->get('width') + $data['layout']['padding_right'] + $data['layout']['padding_left']);
  $outer_box->set('height', $outer_box->get('height') + $data['layout']['padding_top'] + $data['layout']['padding_bottom']);

  // Get details for the rotated/translated boxes.
  $outer_box_t = $outer_box->getTranslatedBox(
    $data['font']['angle']
  );
  $inner_box_t = $inner_box->getTranslatedBox(
    $data['font']['angle'],
    array(
      $data['layout']['padding_left'],
      $data['layout']['padding_top'],
    ),
    $outer_box_t->get('topLeftCornerPosition')
  );

  // Create the wrapper image object as a canvass for the text.
  $wrapper = new stdClass();
  $wrapper->info['width'] = $outer_box_t->get('width');
  $wrapper->info['height'] = $outer_box_t->get('height');
  $wrapper->info['extension'] = $image->info['extension'];
  $wrapper->info['mime_type'] = $image->info['mime_type'];
  $wrapper->toolkit = $image->toolkit;

  // Calls image generation for the wrapper image.
  $data_textimage = array(
    'font' => $data['font'],
    'layout' => $data['layout'],
    'text' => $data['text'],
    'text_lines' => $text_lines,
    'inner_width' => $inner_box->get('width'),
    'inner_height' => $inner_box->get('height'),
    'inner_basepoint' => $inner_box->get('basepoint'),
    'topLeftCornerPosition' => $outer_box_t->get('topLeftCornerPosition'),
    'inner_box' => $inner_box_t->get('points'),
    'outer_box' => $outer_box_t->get('points'),
    'line_height' => $line_height,
    'debug_visuals' => isset($data['debug_visuals']) ? $data['debug_visuals'] : FALSE,
    'gif_transparency_color' => TextimageImager::getState('gif_transparency_color'),
  );
  if (!image_toolkit_invoke('textimage_text_to_image', $wrapper, array($data_textimage))) {
    return NULL;
  }
  return $wrapper;
}

/**
 * Recalculate background image size.
 *
 * When wrapper overflows the original image, and autoextent is set on.
 */
function _textimage_background_image_resize($image, $wrapper, $data, &$image_new, &$frame = NULL) {

  $resized = FALSE;

  // Background image dimensions.
  $image_width = $image->info['width'];
  $image_height = $image->info['height'];

  // Wrapper image dimensions.
  $wrapper_width = $wrapper->info['width'];
  $wrapper_height = $wrapper->info['height'];

  // Determine wrapper offset, based on placement option.
  // This is just taking into account the image and wrapper dimensions;
  // additional offset explicitly specified is considered later.
  $x_offset = ceil(image_filter_keyword($data['layout']['x_pos'], $image_width, $wrapper_width));
  $y_offset = ceil(image_filter_keyword($data['layout']['y_pos'], $image_height, $wrapper_height));

  // The position of the wrapper, once offset as per explicit
  // input. Width and height are not relevant for the algorithm,
  // but would be determined as follows:
  // Width:
  //  if ($wrapper_width < $image_width) then
  //  $wrapper_width + abs($data['layout']['x_offset']) else
  //  $wrapper_width
  // Height:
  //  if ($wrapper_height < $image_height) then
  //  $wrapper_height + abs($data['layout']['y_offset']) else
  //  $wrapper_height
  $offset_wrapper = array(
    'xpos' => $x_offset + $data['layout']['x_offset'],
    'ypos' => $y_offset + $data['layout']['y_offset'],
  );

  // If offset wrapper overflows to the left, background image
  // will be shifted to the right.
  if ($offset_wrapper['xpos'] < 0) {
    $image_new['width'] = $image_width - $offset_wrapper['xpos'];
    $image_new['xpos'] = -$offset_wrapper['xpos'];
    $offset_wrapper['xpos'] = 0;
    if (isset($frame)) {
      $frame['left'] = $image_new['width'] - $image_width;
    }
    $resized = TRUE;
  }

  // If offset wrapper overflows to the top, background image
  // will be shifted to the bottom.
  if ($offset_wrapper['ypos'] < 0) {
    $image_new['height'] = $image_height - $offset_wrapper['ypos'];
    $image_new['ypos'] = -$offset_wrapper['ypos'];
    $offset_wrapper['ypos'] = 0;
    if (isset($frame)) {
      $frame['top'] = $image_new['height'] - $image_height;
    }
    $resized = TRUE;
  }

  // If offset wrapper overflows to the right, background image
  // will be extended to the right.
  if (($offset_wrapper['xpos'] + $wrapper_width) > $image_new['width']) {
    $tmp = $image_new['width'];
    $image_new['width'] = $offset_wrapper['xpos'] + $wrapper_width;
    if (isset($frame)) {
      $frame['right'] = $image_new['width'] - $tmp;
    }
    $resized = TRUE;
  }

  // If offset wrapper overflows to the bottom, background image
  // will be extended to the bottom.
  if (($offset_wrapper['ypos'] + $wrapper_height) > $image_new['height']) {
    $tmp = $image_new['height'];
    $image_new['height'] = $offset_wrapper['ypos'] + $wrapper_height;
    if (isset($frame)) {
      $frame['bottom'] = $image_new['height'] - $tmp;
    }
    $resized = TRUE;
  }

  return array($resized, $offset_wrapper);
}

/**
 * Recalculate wrapper image size.
 *
 * When wrapper overflows the original image, and scaling is set on.
 */
function _textimage_wrapper_resize($image, $wrapper, $data) {

  $resized = FALSE;

  // Background image dimensions.
  $image_width = $image->info['width'];
  $image_height = $image->info['height'];

  // Wrapper image dimensions.
  $wrapper_width = $wrapper->info['width'];
  $wrapper_height = $wrapper->info['height'];

  // Determine wrapper offset, based on placement option and direct
  // offset indicated in settings.
  $offset_wrapper = array(
    'xpos' => ceil(image_filter_keyword($data['layout']['x_pos'], $image_width, $wrapper_width)) + $data['layout']['x_offset'],
    'ypos' => ceil(image_filter_keyword($data['layout']['y_pos'], $image_height, $wrapper_height)) + $data['layout']['y_offset'],
  );

  // Position of wrapper's bottom right point.
  $xc_pos = $offset_wrapper['xpos'] + $wrapper_width;
  $yc_pos = $offset_wrapper['ypos'] + $wrapper_height;

  // Redetermine offset wrapper position and size based on
  // background image size.
  $offset_wrapper['xpos'] = max(0, $offset_wrapper['xpos']);
  $offset_wrapper['ypos'] = max(0, $offset_wrapper['ypos']);
  $xc_pos = min($image_width, $xc_pos);
  $yc_pos = min($image_height, $yc_pos);
  $offset_wrapper['width'] = $xc_pos - $offset_wrapper['xpos'];
  $offset_wrapper['height'] = $yc_pos - $offset_wrapper['ypos'];

  // If negative width/height, then the wrapper is totally
  // overflowing the background, and we cannot resize it.
  if ($offset_wrapper['width'] < 0 || $offset_wrapper['height'] < 0) {
    return array(FALSE, array());
  }

  // Determine if scaling needed. Take the side that is shrinking
  // most.
  $width_resize_index = $offset_wrapper['width'] / $wrapper_width;
  $height_resize_index = $offset_wrapper['height'] / $wrapper_height;
  if ($width_resize_index < 1 || $height_resize_index < 1) {
    $resized = TRUE;
    if ($width_resize_index < $height_resize_index) {
      $offset_wrapper['height'] = NULL;
    }
    else {
      $offset_wrapper['width'] = NULL;
    }
  }

  return array($resized, $offset_wrapper);
}

/**
 * Helpers
 */

/**
 * Matches all 'P' Unicode character classes (punctuation)
 */
if (!defined('PREG_CLASS_PUNCTUATION')) {
  define(
    'PREG_CLASS_PUNCTUATION',
    '\x{21}-\x{23}\x{25}-\x{2a}\x{2c}-\x{2f}\x{3a}\x{3b}\x{3f}\x{40}\x{5b}-\x{5d}' .
    '\x{5f}\x{7b}\x{7d}\x{a1}\x{ab}\x{b7}\x{bb}\x{bf}\x{37e}\x{387}\x{55a}-\x{55f}' .
    '\x{589}\x{58a}\x{5be}\x{5c0}\x{5c3}\x{5f3}\x{5f4}\x{60c}\x{60d}\x{61b}\x{61f}' .
    '\x{66a}-\x{66d}\x{6d4}\x{700}-\x{70d}\x{964}\x{965}\x{970}\x{df4}\x{e4f}' .
    '\x{e5a}\x{e5b}\x{f04}-\x{f12}\x{f3a}-\x{f3d}\x{f85}\x{104a}-\x{104f}\x{10fb}' .
    '\x{1361}-\x{1368}\x{166d}\x{166e}\x{169b}\x{169c}\x{16eb}-\x{16ed}\x{1735}' .
    '\x{1736}\x{17d4}-\x{17d6}\x{17d8}-\x{17da}\x{1800}-\x{180a}\x{1944}\x{1945}' .
    '\x{2010}-\x{2027}\x{2030}-\x{2043}\x{2045}-\x{2051}\x{2053}\x{2054}\x{2057}' .
    '\x{207d}\x{207e}\x{208d}\x{208e}\x{2329}\x{232a}\x{23b4}-\x{23b6}\x{2768}-' .
    '\x{2775}\x{27e6}-\x{27eb}\x{2983}-\x{2998}\x{29d8}-\x{29db}\x{29fc}\x{29fd}' .
    '\x{3001}-\x{3003}\x{3008}-\x{3011}\x{3014}-\x{301f}\x{3030}\x{303d}\x{30a0}' .
    '\x{30fb}\x{fd3e}\x{fd3f}\x{fe30}-\x{fe52}\x{fe54}-\x{fe61}\x{fe63}\x{fe68}' .
    '\x{fe6a}\x{fe6b}\x{ff01}-\x{ff03}\x{ff05}-\x{ff0a}\x{ff0c}-\x{ff0f}\x{ff1a}' .
    '\x{ff1b}\x{ff1f}\x{ff20}\x{ff3b}-\x{ff3d}\x{ff3f}\x{ff5b}\x{ff5d}\x{ff5f}-' .
    '\x{ff65}'
  );
}

/**
 * Matches all 'Z' Unicode character classes (separators)
 */
if (!defined('PREG_CLASS_SEPARATOR')) {
  define('PREG_CLASS_SEPARATOR', '\x{20}\x{a0}\x{1680}\x{180e}\x{2000}-\x{200a}\x{2028}\x{2029}\x{202f}\x{205f}\x{3000}');
}

if (!function_exists('drupal_preg_match')) {
  /**
   * Unicode-safe preg_match().
   *
   * Search subject for a match to the regular expression given in pattern,
   * but return offsets in characters, where preg_match would return offsets
   * in bytes.
   *
   * @see http://php.net/manual/en/function.preg-match.php
   * @see http://drupal.org/node/465638
   */
  function drupal_preg_match($pattern, $subject, &$matches, $flags = NULL, $offset = 0) {
    // Convert the offset value from characters to bytes.
    // NOTE - strlen is used on purpose here, instead of drupal_strlen
    $offset = strlen(drupal_substr($subject, 0, $offset));

    $return_value = preg_match($pattern, $subject, $matches, $flags, $offset);

    if ($return_value && ($flags & PREG_OFFSET_CAPTURE)) {
      foreach ($matches as &$match) {
        // Convert the offset returned by preg_match from bytes back to
        // characters.
        // NOTE - substr is used on purpose here, instead of drupal_substr
        $match[1] = drupal_strlen(substr($subject, 0, $match[1]));
      }
    }
    return $return_value;
  }
}

/**
 * Wrap text for rendering at a given width.
 *
 * @param object $image
 *   Image object.
 * @param string $text
 *   Text string in UTF-8 encoding.
 * @param int $font_size
 *   Font size.
 * @param string $font_uri
 *   URI of the TrueType font to use.
 * @param int $maximum_width
 *   Maximum width allowed for each line.
 *
 * @return string
 *   Text string, with newline characters to separate each line.
 */
function _textimage_wrap_text($image, $text, $font_size, $font_uri, $maximum_width) {
  // State variables for the search interval.
  $end = 0;
  $begin = 0;
  $fit = $begin;

  // Note: we count in bytes for speed reasons, but maintain character
  // boundaries.
  while (TRUE) {

    $match = array();

    // Find the next wrap point (always after trailing whitespace).
    if (drupal_preg_match('/[' . PREG_CLASS_PUNCTUATION . '][' . PREG_CLASS_SEPARATOR . ']*|[' . PREG_CLASS_SEPARATOR . ']+/u', $text, $match, PREG_OFFSET_CAPTURE, $end)) {
      $end = $match[0][1] + drupal_strlen($match[0][0]);
    }
    else {
      $end = drupal_strlen($text);
    }

    // Fetch text, removing trailing white-space, and measure it.
    $line  = preg_replace('/[' . PREG_CLASS_SEPARATOR . ']+$/u', '', drupal_substr($text, $begin, $end - $begin));
    $width = _textimage_measure_text_width($image, $line, 1, $font_size, $font_uri);

    // See if line extends past the available space.
    if ($width > $maximum_width) {
      // If this is the first word, we need to truncate it.
      if ($fit == $begin) {
        // Cut off letters until it fits.
        while (drupal_strlen($line) > 0 && $width > $maximum_width) {
          $line  = drupal_substr($line, 0, -1);
          $width = _textimage_measure_text_width($image, $line, 1, $font_size, $font_uri);
        }
        // If no fit was found, the image is too narrow.
        $fit = drupal_strlen($line) ? $begin + drupal_strlen($line) : $end;
      }
      // We have a valid fit for the next line. Insert a line-break and reset
      // the search interval.
      if (drupal_substr($text, $fit - 1, 1) == ' ') {
        $first_part = drupal_substr($text, 0, $fit - 1);
      }
      else {
        $first_part = drupal_substr($text, 0, $fit);
      }
      $last_part  = drupal_substr($text, $fit);
      $text  = $first_part . "\n" . $last_part;
      $begin = ++$fit;
      $end   = $begin;
    }
    else {
      // We can fit this text. Wait for now.
      $fit = $end;
    }

    if ($end == drupal_strlen($text)) {
      // All text fits. No more changes are needed.
      break;
    }
  }
  return $text;
}

/**
 * Return the path of the font file, in a format usable by current toolkit.
 *
 * @param object $image
 *   Image object.
 * @param string $font_uri
 *   URI of the TrueType font to use.
 *
 * @return string
 *   Fully qualified font file path.
 */
function _textimage_get_font_path($image, $font_uri) {
  $data = array(
    'font_uri' => $font_uri,
  );
  return image_toolkit_invoke('textimage_get_font_path', $image, array($data));
}

/**
 * Return a text bounding box.
 *
 * @param object $image
 *   Image object.
 * @param string $text
 *   Text string in UTF-8 encoding.
 * @param int $lines
 *   The number of lines the text is composed of.
 * @param int $font_size
 *   Font size.
 * @param string $font_uri
 *   URI of the TrueType font to use.
 * @param float $angle
 *   Text rotation angle.
 *
 * @return TextimageTextbox
 *   Textbox object.
 */
function _textimage_get_bounding_box($image, $text, $lines, $font_size, $font_uri, $angle = 0) {
  $data = array(
    'size' => $font_size,
    'angle' => $angle,
    'fontfile' => $font_uri,
    'text' => $text,
    'lines' => $lines,
  );
  return image_toolkit_invoke('textimage_get_bounding_box', $image, array($data));
}

/**
 * Measure text box width.
 *
 * @param object $image
 *   Image object.
 * @param string $text
 *   Text string in UTF-8 encoding.
 * @param int $lines
 *   The number of lines the text is composed of.
 * @param int $font_size
 *   Font size.
 * @param string $font_uri
 *   URI of the TrueType font to use.
 *
 * @return array
 *   An associative array of box measurements.
 */
function _textimage_measure_text_width($image, $text, $lines, $font_size, $font_uri) {
  $box = _textimage_get_bounding_box($image, $text, $lines, $font_size, $font_uri);
  return $box->get('width');
}

/**
 * Draw a rectangle.
 *
 * @param object $image
 *   Image object.
 * @param array $points
 *   Box points coordinates array.
 * @param string $rgba
 *   RGBA color of the rectangle.
 * @param bool $luma
 *   if TRUE, convert RGBA to best match using luma.
 */
function _textimage_draw_rectangle($image, $points, $rgba, $luma = FALSE) {

  // Check color.
  if ($rgba && $luma) {
    $rgba = textimage_match_luma($rgba);
  }

  // Invoke toolkit.
  $data = array(
    'points' => $points,
    'num_points' => 4,
    'border_color' => NULL,
    'fill_color' => $rgba,
  );
  return image_toolkit_invoke('textimage_draw_polygon', $image, array($data));
}

/**
 * Draw a box.
 *
 * @param object $image
 *   Image object.
 * @param array $points
 *   Box points coordinates array.
 * @param string $rgba
 *   RGBA color of the rectangle.
 * @param bool $luma
 *   if TRUE, convert RGBA to best match using luma.
 */
function _textimage_draw_box($image, $points, $rgba, $luma = FALSE) {

  // Check color.
  if (!$rgba) {
    $rgba = '#00000000';
  }
  elseif ($luma) {
    $rgba = textimage_match_luma($rgba);
  }

  // Invoke toolkit.
  $data = array(
    'points' => $points,
    'num_points' => 4,
    'border_color' => $rgba,
    'fill_color' => NULL,
  );
  return image_toolkit_invoke('textimage_draw_polygon', $image, array($data));
}

/**
 * Display a polygon enclosing the text line, and conspicuous points.
 *
 * Credit to Ruquay K Calloway
 *
 * @param object $image
 *   Image object.
 * @param TextimageTextbox $box
 *   Textbox object to draw (inclusing basepoint).
 * @param string $rgba
 *   RGBA color of the rectangle.
 * @param bool $luma
 *   if TRUE, convert RGBA to best match using luma.
 *
 * @see http://ruquay.com/sandbox/imagettf
 */
function _textimage_draw_debug_box($image, TextimageTextbox $box, $rgba, $luma = FALSE) {

  // Check color.
  if (!$rgba) {
    $rgba = '#00000000';
  }
  elseif ($luma) {
    $rgba = textimage_match_luma($rgba);
  }

  // Retrieve points.
  $points = $box->get('points');

  // Draw box.
  _textimage_draw_box($image, $points, $rgba);

  // Draw diagonal.
  $data = array(
    'a' => array($points[0], $points[1]),
    'b' => array($points[4], $points[5]),
    'rgba' => $rgba,
  );
  image_toolkit_invoke('textimage_draw_line', $image, array($data));

  // Conspicuous points.
  $orange = '#FF640000';
  $yellow = '#FFFF0000';
  $green  = '#00FF0000';
  $dotsize = 6;

  // Box corners.
  for ($i = 0; $i < 8; $i += 2) {
    $col = $i < 4 ? $orange : $yellow;
    $data = array(
      'c' => array($points[$i], $points[$i + 1]),
      'width' => $dotsize,
      'height' => $dotsize,
      'rgba' => $col,
    );
    image_toolkit_invoke('textimage_draw_ellipse', $image, array($data));
  }

  // Font baseline.
  $data = array(
    'c' => $box->get('basepoint'),
    'width' => $dotsize,
    'height' => $dotsize,
    'rgba' => $green,
  );
  image_toolkit_invoke('textimage_draw_ellipse', $image, array($data));

}
