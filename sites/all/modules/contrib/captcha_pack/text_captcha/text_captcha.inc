<?php

/**
 * Helper function for splitting a string on white spaces.
 * Using explode(' ', $string) is not enough because it returns empty elements
 * if $string contains consecutive spaces.
 */
function _text_captcha_whitespace_explode($string) {
  return preg_split('/\s+/', $string, -1, PREG_SPLIT_NO_EMPTY);
}

/**
 * Helper function for splitting an utf8 string correctly in characters.
 * Assumes the given utf8 string is well formed.
 * See http://en.wikipedia.org/wiki/Utf8 for more info
 */
function _text_captcha_utf8_split($str) {
  $characters = array();
  $len = drupal_strlen($str);
  for ($i = 0; $i < $len;) {
    $chr = ord($str[$i]);
    if (($chr & 0x80) == 0x00) { // one byte character (0zzzzzzz)
      $width = 1;
    }
    else {
      if (($chr & 0xE0) == 0xC0) { // two byte character (first byte: 110yyyyy)
        $width = 2;
      }
      elseif (($chr & 0xF0) == 0xE0) { // three byte character (first byte: 1110xxxx)
        $width = 3;
      }
      elseif (($chr & 0xF8) == 0xF0) { // four byte character (first byte: 11110www)
        $width = 4;
      }
      else {
        watchdog('captcha', t('Encountered an illegal byte while splitting an utf8 string in characters.'), WATCHDOG_ERROR);
        return $characters;
      }
    }
    $characters[] = drupal_substr($str, $i, $width);
    $i += $width;
  }
  return $characters;
}


/**
 * Helper function for getting the content of a word pool
 * Locale dependent
 */
function _text_captcha_word_pool_get_content($name_base, $lang_code, $default_value, $explode = FALSE) {
  if (module_exists('locale')) {
    if (!$lang_code) {
      global $language;
      $lang_code = $language->language;
    }
    $content = variable_get("{$name_base}_{$lang_code}", t($default_value));
  }
  else {
    $content = variable_get($name_base, t($default_value));
  }
  if ($explode) {
    $content = _text_captcha_whitespace_explode($content);
  }
  return $content;
}

/**
 * Helper function for setting word pool form items
 * Locale dependent
 */
function _text_captcha_word_pool_form_items(&$form, $name_base, $title, $description, $default_value, $rows = 3) {
  if (module_exists('locale')) {
    // Locale available
    $langs = locale_language_list();
    $form[$name_base] = array(
      '#type' => 'fieldset',
      '#title' => t($title),
      '#description' => t($description),
      '#collapsible' => TRUE,
    );
    foreach ($langs as $lang_code => $lang_name) {
      $form[$name_base]["{$name_base}_{$lang_code}"] = array(
        '#type' => 'textarea',
        '#title' => t('For language %lang_name (code %lang_code)', array('%lang_name'=>$lang_name, '%lang_code'=>$lang_code)),
        '#default_value' => _text_captcha_word_pool_get_content($name_base, $lang_code, $default_value),
        '#rows' => $rows,
      );
    }
  }
  else {
    // No locale available
    $form[$name_base] = array(
      '#type' => 'textarea',
      '#title' => t($title),
      '#description' => t($description),
      '#default_value' => _text_captcha_word_pool_get_content($name_base, NULL, $default_value),
      '#rows' => $rows,
    );
  }
}


function _text_captcha_word_pool_validate_word_length($name, $words, $minimum_length, $too_short_message) {
  $too_short = array();
  foreach($words as $word) {
    if (count(_text_captcha_utf8_split($word)) < $minimum_length) {
      $too_short[] = $word;
    }
  }
  if (count($too_short)) {
    form_set_error($name, t($too_short_message, array('@minimum_length' => $minimum_length, '@words' => implode(', ',$too_short))));
  }
}

/**
 * Helper function for validating the word pool
 * Locale dependent
 */
function _text_captcha_word_pool_validate($name_base, $form_values, $minimum_count, $minimum_length = NULL, $too_short_message = '') {
  if (module_exists('locale')) {
    $langs = locale_language_list();
    foreach ($langs as $lang_code => $lang_name) {
      $words = _text_captcha_whitespace_explode($form_values["{$name_base}_{$lang_code}"]);
      if (count($words) < $minimum_count) {
        form_set_error("{$name_base}_{$lang_code}", t('You should provide at least @num words', array('@num' => $minimum_count)));
      }
      // Check the minimum word length (if needed)
      if ($minimum_length) {
        _text_captcha_word_pool_validate_word_length("{$name_base}_{$lang_code}", $words, $minimum_length, $too_short_message);
      }
    }
  }
  else {
    $words = _text_captcha_whitespace_explode($form_values[$name_base]);
    if (count($words) < $minimum_count) {
      form_set_error($name_base, t('You should provide at least @num words', array('@num' => $minimum_count)));
    }
    if ($minimum_length) {
      _text_captcha_word_pool_validate_word_length($name_base, $words, $minimum_length, $too_short_message);
    }
  }
}
