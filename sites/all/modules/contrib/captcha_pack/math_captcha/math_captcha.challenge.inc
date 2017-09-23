<?php

/**
 * @file
 * Provides challenge functions for MATH CAPTCHA administration.
 */

/**
 * Function for addition challenges
 */
function _math_captcha_addition_challenge() {
  $argmax = intval(variable_get('math_captcha_addition_argmax', 10));
  if (variable_get('math_captcha_addition_allow_negative', FALSE)) {
    $x = mt_rand(-$argmax, $argmax);
    $y = mt_rand(-$argmax, $argmax);
  }
  else {
    $x = mt_rand(0, $argmax);
    $y = mt_rand(0, $argmax);
  }
  $solution = $x + $y;
  $maxlength = strlen(strval($argmax + $argmax)) + intval(variable_get('math_captcha_addition_allow_negative', FALSE));
  return _math_captcha_build_captcha($x, $y, '+', $solution, $maxlength);
}

/**
 * Function for subtraction challenges
 */
function _math_captcha_subtraction_challenge() {
  $argmax = intval(variable_get('math_captcha_subtraction_argmax', 10));
  if (variable_get('math_captcha_subtraction_allow_negative', FALSE)) {
    $x = mt_rand(-$argmax, $argmax);
    $y = mt_rand(-$argmax, $argmax);
  }
  else {
    $y = mt_rand(0, $argmax);
    $x = mt_rand($y, $argmax);
  }
  $solution = $x - $y;
  $maxlength = strlen(strval($argmax + $argmax)) + intval(variable_get('math_captcha_subtraction_allow_negative', FALSE));
  return _math_captcha_build_captcha($x, $y, '-', $solution, $maxlength);
}

/**
 * Function for multiplication challenges
 */
function _math_captcha_multiplication_challenge() {
  $argmax = intval(variable_get('math_captcha_multiplication_argmax', 5));
  $x = mt_rand(1, $argmax);
  $y = mt_rand(1, $argmax);
  if (variable_get('math_captcha_multiplication_allow_negative', FALSE)) {
    $x = $x * (mt_rand(0, 1) * 2 - 1);
    $y = $y * (mt_rand(0, 1) * 2 - 1);
  }
  $solution = $x * $y;
  $maxlength = strlen(strval($argmax * $argmax)) + intval(variable_get('math_captcha_multiplication_allow_negative', FALSE));
  return _math_captcha_build_captcha($x, $y, '*', $solution, $maxlength);
}

/**
 * Helper function to build a math CAPTCHA form item
 */
function _math_captcha_build_captcha($x, $y, $operator, $result, $maxlength = 3) {
  $form_item = array();
  $form_item['form']['captcha_response'] = array(
    '#type' => 'textfield',
    '#title' => t('Math question'),
    '#required' => TRUE,
    '#size' => $maxlength + 2,
    '#maxlength' => $maxlength,
    '#description' => t('Solve this math question and enter the solution with digits. E.g. for "two plus four = ?" enter "6".'),
  );
  switch (mt_rand(0, 2)) {
    case 0: // question like "x + y = ?"
      $form_item['solution'] = "$result";
      $form_item['form']['captcha_response']['#field_prefix'] = _math_captcha_repr($x, TRUE) . ' ' . _math_captcha_repr_op($operator) . ' ' . _math_captcha_repr($y, TRUE) . ' ' . _math_captcha_repr_op('=');
      break;
    case 1: // question like "x + ? = z"
      $form_item['solution'] = "$y";
      $form_item['form']['captcha_response']['#field_prefix'] = _math_captcha_repr($x, TRUE) . ' ' . _math_captcha_repr_op($operator) . ' ';
      $form_item['form']['captcha_response']['#field_suffix'] = ' ' . _math_captcha_repr_op('=') . ' ' . _math_captcha_repr($result);
      break;
    case 2: // question like "? + y = z"
      $form_item['solution'] = "$x";
      $form_item['form']['captcha_response']['#field_suffix'] = ' ' . _math_captcha_repr_op($operator) . ' ' . _math_captcha_repr($y, TRUE) . ' ' . _math_captcha_repr_op('=') . ' ' . _math_captcha_repr($result);
      break;
  }
  return $form_item;
}

/**
 * Helper function for transforming a number to a textual representation
 */
function _math_captcha_repr($n, $add_paratheses_when_negative = FALSE) {
  // Start with no textual representation
  $t = "$n";
  // If enabled and available: do textual representation
  if (variable_get('math_captcha_textual_numbers', TRUE)) {
    $repr_map = array(
      0 => t('zero'), 1 => t('one'), 2 => t('two'), 3 => t('three'),
      4 => t('four'), 5 => t('five'), 6 => t('six'), 7 => t('seven'), 8 => t('eight'),
      9 => t('nine'), 10 => t('ten'), 11 => t('eleven'), 12 => t('twelve'),
      13 => t('thirteen'), 14 => t('fourteen'), 15 => t('fifteen'),
    );
    if (array_key_exists(abs($n), $repr_map)) {
      $t = $repr_map[abs($n)];
      if ($n < 0) {
        $t = t('minus !number', array('!number' => $t));
      }
    }
  }
  if ($add_paratheses_when_negative && $n < 0) {
    $t = "($t)";
  }
  return $t;
}

/**
 * Helper function for transforming a operator to a textual representation
 */
function _math_captcha_repr_op($op) {
  // Start with no textual representation
  $t = "$op";
  // If enabled and available: do textual representation
  if (variable_get('math_captcha_textual_operators', FALSE)) {
    $repr_map = array('+' => t('plus'), '-' => t('minus'), '*' => t('times'), '=' => t('equals'));
    if (array_key_exists($op, $repr_map)) {
      $t = $repr_map[$op];
    }
  }
  return $t;
}
