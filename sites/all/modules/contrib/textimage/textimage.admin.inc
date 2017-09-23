<?php
/**
 * @file
 * Textimage - Admin page callbacks.
 */

/**
 * Make alterations to the core 'image_style_form' form.
 *
 * A fieldset is added if the image style is Textimage relevant.
 */
function _textimage_image_style_form_form_alter(&$form, &$form_state, $form_id) {
  if (TextimageStyles::isTextimage($form_state['image_style'])) {
    $form['textimage_options'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Textimage options'),
      '#description' => t('Define Textimage options specific for this image style.'),
    );
    // Define file storage wrapper used for the style images.
    $scheme_options = array();
    foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
      $scheme_options[$scheme] = $stream_wrapper['name'];
    }
    $form['textimage_options']['uri_scheme'] = array(
      '#type' => 'radios',
      '#options' => $scheme_options,
      '#title' => t('Image destination'),
      '#description' => t('Select where Textimage image files should be stored. Private file storage has significantly more overhead than public files, but allows access restriction.'),
      '#default_value' => $form_state['image_style']['textimage']['uri_scheme'],
    );
    // Adds a submit handler to deal with textimage options.
    $form['#submit'][] = '_textimage_image_style_form_textimage_submit';
  }
}

/**
 * Submit handler to deal with a style's Textimage options.
 *
 * @see _textimage_image_style_form_form_alter()
 */
function _textimage_image_style_form_textimage_submit($form, &$form_state) {
  $style = TextimageStyles::get($form_state['values']['name']);
  // Update Textimage-specific style options.
  if ($style) {
    $style['textimage']['uri_scheme'] = $form_state['values']['textimage_options']['uri_scheme'];
    image_style_save($style);
  }
}

/**
 * Textimage settings form.
 */
function textimage_form_settings($form, &$form_state) {

  // Main Textimage store location.
  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $default_scheme = _textimage_get_variable('store_scheme');
  $default_scheme = isset($scheme_options[$default_scheme]) ? $default_scheme : 'public';
  $form['textimage_store'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Textimage store location'),
  );
  $form['textimage_store']['store_scheme'] = array(
    '#type' => 'radios',
    '#options' => $scheme_options,
    '#title' => t('Scheme'),
    '#description' => t('Select where the main Textimage file structure should be stored. It is recommended to keep it in the <strong>private</strong> file storage area.'),
    '#default_value' => $default_scheme,
  );

  // Fonts handler.
  $fonts_handling_module_options = array();
  $fonts_handling_module_options['textimage'] = t('Textimage');
  if (_textimage_module_exists('fontyourface', TEXTIMAGE_FONTYOURFACE_MIN_VERSION)) {
    $fonts_handling_module_options['fontyourface'] = t('@font-your-face');
  }
  $fonts_handling_module_selected = isset($form_state['values']['fonts_handling_module']) ? $form_state['values']['fonts_handling_module'] : _textimage_get_variable('fonts_handling_module');
  $default_font = _textimage_get_variable('default_font');

  // Fonts list.
  $fonts_list_options = array();
  if ($fonts_handling_module_selected == 'textimage') {
    $fonts_list_options['fonts_path'] = isset($form_state['values']['fonts_path']) ? $form_state['values']['fonts_path'] : _textimage_get_variable('fonts_path');
  }
  $fonts_list = TextimageFonts::getList($fonts_handling_module_selected, $fonts_list_options);
  if (!$fonts_list) {
    _textimage_diag(t('No fonts available. Make sure at least one font is installed, and select a default font for Textimage to use.'), WATCHDOG_WARNING);
  }
  $fonts_options = array();
  if (empty($fonts_list) or ($default_font['name'] == '' or  !TextimageFonts::getUri($default_font['name']))) {
    $fonts_options[''] = t('- none -');
  }
  $fonts_options += drupal_map_assoc($fonts_list);

  // Fonts.
  $form['fonts'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Fonts'),
    '#description' => t('Number of fonts available: @fonts', array('@fonts' => count($fonts_list))),
    '#prefix'    => '<div id="wrapper-fonts">',
    '#suffix'    => '</div>',
  );
  $form['fonts']['fonts_handling_module'] = array(
    '#type'    => 'select',
    '#title'   => t('Fonts handling module'),
    '#options' => $fonts_handling_module_options,
    '#default_value' => $fonts_handling_module_selected,
    '#description' => t(
      'Select the module Textimage has to use to manage fonts. If <a href="!path">@font-your-face</a> is installed and selected, Textimage will allow selection of enabled <em>local</em> fonts.',
      array(
        '!path' => url('http://drupal.org/project/fontyourface'),
      )
    ),
    '#required'    => TRUE,
    '#ajax'  => array(
      'callback' => 'textimage_form_settings_ajax',
      'wrapper' => 'wrapper-fonts',
    ),
  );
  $form['fonts']['fonts_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => _textimage_get_variable('fonts_path'),
    '#maxlength' => 255,
    '#description' => t('Location of the directory where the fonts are stored.'),
    '#states' => array(
      'visible' => array(
        ':input[name="fonts_handling_module"]' => array('value' => 'textimage'),
      ),
    ),
    '#ajax'  => array(
      'callback' => 'textimage_form_settings_ajax',
      'wrapper' => 'wrapper-fonts',
    ),
  );
  $form['fonts']['default_font_name'] = array(
    '#type'    => 'select',
    '#title'   => t('Default font'),
    '#options' => $fonts_options,
    '#default_value' => $default_font['name'],
    '#description' => t('Select the default font to be used by Textimage.'),
    '#required'    => TRUE,
  );

  // Background images.
  $backgrounds_handling_module_options = array();
  $backgrounds_handling_module_options['textimage'] = t('Textimage');
  if (_textimage_module_exists('media', TEXTIMAGE_MEDIA_MIN_VERSION)) {
    $backgrounds_handling_module_options['media'] = t('Media');
  }
  $backgrounds_handling_module_option_selected = isset($form_state['values']['backgrounds_handling_module']) ? $form_state['values']['backgrounds_handling_module'] : _textimage_get_variable('backgrounds_handling_module');
  $form['images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Background images'),
  );
  $form['images']['backgrounds_handling_module'] = array(
    '#type'    => 'select',
    '#title'   => t('Backgrounds handling module'),
    '#options' => $backgrounds_handling_module_options,
    '#default_value' => $backgrounds_handling_module_option_selected,
    '#description' => t(
      'Select the module Textimage has to use to manage background images. If <a href="!path">Media</a> is installed and selected, Textimage will use the Media browser to select images.',
      array(
        '!path' => url('http://drupal.org/project/media'),
      )
    ),
    '#required'    => TRUE,
  );
  $form['images']['backgrounds_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => _textimage_get_variable('backgrounds_path'),
    '#maxlength' => 255,
    '#description' =>
    t('Location of the directory where the background images are stored.') . ' ' .
    t('Relative paths will be resolved relative to the Drupal installation directory.'),
    '#states' => array(
      'visible' => array(
        ':input[name="backgrounds_handling_module"]' => array('value' => 'textimage'),
      ),
    ),
  );

  // Color selector.
  $color_selector_options = array();
  $color_selector_options['textbox'] = t('Textbox');
  if (_textimage_module_exists('jquery_colorpicker', TEXTIMAGE_JQUERY_COLORPICKER_MIN_VERSION)) {
    $color_selector_options['jquery_colorpicker'] = t('jQuery Colorpicker');
  }
  $form['color'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Colors'),
  );
  $form['color']['color_selector'] = array(
    '#type'    => 'select',
    '#title'   => t('Color selector'),
    '#options' => $color_selector_options,
    '#default_value' => _textimage_get_variable('color_selector'),
    '#description' => t(
      'Select how colors should be input in administration forms. If <a href="!path">jQuery Colorpicker</a> is installed and selected, it provides a better alternative to mere hex code entry in a textbox.',
      array(
        '!path' => url('http://drupal.org/project/jquery_colorpicker'),
      )
    ),
    '#required'    => TRUE,
  );

  // Maintenance.
  $form['maintenance'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Maintenance'),
  );
  $form['maintenance']['flush_all'] = array(
    '#type' => 'submit',
    '#name' => 'flush_all',
    '#value' => t('Cleanup Textimage'),
  );

  // Save configuration.
  $form['save'] = array(
    '#type' => 'submit',
    '#name' => 'save',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * AJAX callback to refresh the fonts fieldset.
 */
function textimage_form_settings_ajax($form, $form_state) {
  return $form['fonts'];
}

/**
 * Textimage settings form - submission.
 */
function textimage_form_settings_submit($form, &$form_state) {

  if ($form_state['triggering_element']['#name'] == 'flush_all') {
    // Redirect to cleanup.
    $form_state['redirect'] = 'admin/config/media/textimage/cleanup';
    return;
  }

  $v = $form_state['values'];

  if ($v['store_scheme'] != _textimage_get_variable('store_scheme')) {
    _textimage_flush_all();
    _textimage_set_variable('store_scheme', $v['store_scheme']);
  }

  _textimage_set_variable('fonts_handling_module', $v['fonts_handling_module']);
  _textimage_set_variable('fonts_path', $v['fonts_path']);
  $options = array();
  if ($v['fonts_handling_module'] == 'textimage') {
    $options['fonts_path'] = $v['fonts_path'];
  }
  $font_uri = TextimageFonts::getUri($v['default_font_name'], $v['fonts_handling_module'], $options);
  _textimage_set_variable(
    'default_font',
    array(
      'name' => $v['default_font_name'],
      'uri' => $font_uri,
    )
  );
  _textimage_set_variable('backgrounds_handling_module', $v['backgrounds_handling_module']);
  _textimage_set_variable('backgrounds_path', $v['backgrounds_path']);
  _textimage_set_variable('color_selector', $v['color_selector']);

  drupal_set_message(t('The configuration options have been saved.'), 'status');
}

/**
 * Textimage cleanup form.
 */
function textimage_flush_all_form($form, &$form_state) {
  return confirm_form(
    $form,
    t('Textimage cleanup'),
    'admin/config/media/textimage',
    t('Are you sure you want to cleanup Textimage? This will remove all image files generated via Textimage, flush all the Textimage image styles, and clear all entries cached and stored on the database.'),
    t('Proceed'),
    t('Cancel')
  );
}

/**
 * Textimage cleanup form - submission.
 */
function textimage_flush_all_form_submit($form, &$form_state) {
  _textimage_flush_all();
  $form_state['redirect'] = 'admin/config/media/textimage';
}
