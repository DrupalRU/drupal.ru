<?php

/**
 * @file
 * Functionality and helper functions for LOST CHARACTER CAPTCHA administration.
 */

/**
 * Function for the settings form
 */
function lost_character_captcha_settings_form() {
  $form = array();
  // Form element for the number of characters to lose
  $form['lost_character_captcha_quantity'] = array(
    '#type' => 'select',
    '#title' => t('Number of characters to lose'),
    '#default_value' => variable_get('lost_character_captcha_quantity', 1),
    '#description' => t('Select how many characters should be lost in the CAPTCHA.'),
    '#options' => drupal_map_assoc(array(1, 2, 3)),
  );
  // Form element for hinting
  $form['lost_character_captcha_enable_hint'] = array(
    '#type' => 'checkbox',
    '#title' => t('Put "%hinter" where the characters are lost as a hint', array('%hinter' => LOST_CHARACTER_CAPTCHA_HINTER)),
    '#default_value' => variable_get('lost_character_captcha_enable_hint', TRUE),
    '#description' => t('Enable this option to make it easier to determine the lost characters.'),
  );
  // Form elements for the word pool
  _text_captcha_word_pool_form_items($form,
    'lost_character_captcha_word_pool',
    'Word pool',
    'Enter the words to use, separated with spaces. Make sure every word is unambiguously recognizable when characters are lost. Avoid for example verbs, adverbs, plural forms, too short words, names. Also make sure the words are well known to your intended public.',
    LOST_CHARACTER_CAPTCHA_DEFAULT_WORD_POOL
  );
  // Add a pre_render callback
  $form['#pre_render'][] = 'lost_character_captcha_settings_form_pre_render';
  // Add validation function
  $form['#validate'][] = 'lost_character_captcha_settings_form_validate';
  // Add buttons and return
  return system_settings_form($form);
}

/**
 * Pre_render function
 */
function lost_character_captcha_settings_form_pre_render($form) {
  // Set a warning if the numbers to lose is to big and if hinting is off
  if (variable_get('lost_character_captcha_quantity', 1) > 2 && !variable_get('lost_character_captcha_enable_hint', TRUE)) {
    drupal_set_message(t('Losing more than two characters without indication where they are lost could be too hard for a human. Check your settings.'), 'warning');
  }
  return $form;
}

/**
 * Validation function for the settings form
 */
function lost_character_captcha_settings_form_validate($form, &$form_state) {
  $lost_quantity = (int) $form_state['values']['lost_character_captcha_quantity'];
  $hinting = (int)($form_state['values']['lost_character_captcha_enable_hint']);
  $min_length = 3 + 2 * $lost_quantity + (1 - $hinting);
  // Check the number of words in the pool
  _text_captcha_word_pool_validate(
      'lost_character_captcha_word_pool',
      $form_state['values'],
      3,
      $min_length,
      'The following words are too short (at least @minimum_length characters needed for the current settings of characters to lose and hinting): <div>@words</div>'
  );
}
