<?php

/**
 * @file
 * Functionality and helper functions for WORD LIST CAPTCHA administration.
 */

/**
 * Function for the settings form
 */
function word_list_captcha_settings_form() {
  drupal_set_message(t('WARNING: this module is not completely ported to Drupal 6 and does not work yet.'), 'warning');
  $form = array();
  // Form element for the number of words in the word list
  $form['word_list_captcha_list_size'] = array(
    '#type' => 'select',
    '#title' => t('Number of words in word list'),
    '#default_value' => variable_get('word_list_captcha_list_size', 5),
    '#options' => drupal_map_assoc(array(4, 5, 6, 7, 8, 9, 10)),
  );
  // Form elements for the word pools
  _text_captcha_word_pool_form_items($form,
    'word_list_captcha_word_pool_1',
    t('Word pool @num', array('@num' => 1)),
    'Enter a bunch of related words (space separated, no punctuation). Make sure they are not related in the same way to the words of the other word pool.',
    WORD_LIST_CAPTCHA_WORD_POOL1,
    2
  );
  _text_captcha_word_pool_form_items($form,
    'word_list_captcha_word_pool_2',
    t('Word pool @num', array('@num' => 2)),
    'Enter a bunch of related words (space separated, no punctuation). Make sure they are not related in the same way to the words of the other word pool.',
    WORD_LIST_CAPTCHA_WORD_POOL2,
    2
  );
  // Add additional validation handler
  $form['#validate'][] = 'word_list_captcha_settings_form_validate';
  // Add buttons and return
  return system_settings_form($form);
}

/**
 * Validation function for the settings form
 */
function word_list_captcha_settings_form_validate($form, &$form_state) {
  // Check the number of words in the pools
  $list_size = (int) $form_state['values']['word_list_captcha_list_size'];
  _text_captcha_word_pool_validate('word_list_captcha_word_pool_1', $form_state['values'], $list_size, 0, '');
  _text_captcha_word_pool_validate('word_list_captcha_word_pool_2', $form_state['values'], $list_size, 0, '');
}
