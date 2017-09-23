<?php
/**
 * @file
 * Implementation of the 'textimage_gif_transparency' image effect.
 */

/**
 * Default values for the textimage_gif_transparency effect.
 *
 * @return array
 *   Effect default data.
 */
function _textimage_gif_transparency_effect_defaults() {
  return array(
    'gif_transparency_color' => NULL,
  );
}

/**
 * Settings for 'textimage_gif_transparency' image effect.
 *
 * Implements hook_form().
 *
 * @param array $data
 *   array of settings for this action
 *
 * @return array
 *   a form definition
 */
function textimage_gif_transparency_effect_form($data) {

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_gif_transparency_effect_defaults(), $data);

  $form = array();

  // GIF transparency color.
  $form['gif_transparency_color'] = array(
    '#type' => 'textimage_color',
    '#title' => t('Color'),
    '#description'  => t('Select the color to be used for GIF transparency. <b>Note:</b> this will only be used by Textimage effects.'),
    '#allow_transparent' => FALSE,
    '#allow_opacity' => FALSE,
    '#default_value' => $data['gif_transparency_color'],
  );

  return $form;
}

/**
 * Renders 'textimage_gif_transparency' image effect summary.
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
function theme_textimage_gif_transparency_effect_summary($variables) {
  $output = NULL;

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_gif_transparency_effect_defaults(), $variables['data']);

  // GIF transparency color.
  $output .= (' ' .  t('- Color:') . ' ');
  if ($data['gif_transparency_color']) {
    $output .= theme(
      'textimage_colored_string',
      array(
        'text' => 'MMMM',
        'foreground_color' => $data['gif_transparency_color'],
        'background_color' => $data['gif_transparency_color'],
        'border' => TRUE,
        'border_color' => 'matchLuma',
      )
    );
  }
  else {
    $output .= (' ' . t('Not set'));
  }

  return $output;
}

/**
 * Implements 'textimage_gif_transparency' image effect callback.
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
function textimage_gif_transparency_effect(&$image, $data) {

  // Merge input data with effect defaults.
  $data = drupal_array_merge_deep(_textimage_gif_transparency_effect_defaults(), $data);

  // Stores GIF transparency color for later effects.
  if ($data['gif_transparency_color']) {
    TextimageImager::setState('gif_transparency_color', $data['gif_transparency_color']);
  }

  return TRUE;
}
