<?php

/**
 * @file
 * Functionality and helper functions for MATH CAPTCHA administration.
 */

/**
 * Math CAPTCHA settings form
 */
function math_captcha_settings_form() {
  $form = array();
  $enabled_challenges = _math_captcha_enabled_challenges();
  $form['math_captcha_enabled_challenges'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled math challenges'),
    '#options' => array(
      'addition' => t('Addition: x + y = z'),
      'subtraction' => t('Subtraction: x - y = z'),
      'multiplication' => t('Multiplication: x * y = z'),
    ),
    '#default_value' => $enabled_challenges,
    '#description' => t('Select the math challenges you want to enable.'),
  );
  $form['math_captcha_textual_numbers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Textual representation of numbers'),
    '#default_value' => variable_get('math_captcha_textual_numbers', TRUE),
    '#description' => t('When enabled, the numbers in the challenge will get a textual representation if available. E.g. "four" instead of "4".'),
  );
  $form['math_captcha_textual_operators'] = array(
    '#type' => 'checkbox',
    '#title' => t('Textual representation of operators'),
    '#default_value' => variable_get('math_captcha_textual_operators', FALSE),
    '#description' => t('When enabled, the operators in the challenge will get a textual representation if available. E.g. "plus" instead of "+".'),
  );
  // Addition challenge
  $form['math_captcha_addition'] = array(
    '#type' => 'fieldset',
    '#title' => t('Addition challenge: x + y = z'),
  );
  $form['math_captcha_addition']['math_captcha_addition_argmax'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum value for x and y'),
    '#default_value' => variable_get('math_captcha_addition_argmax', 10),
    '#maxlength' => 3,
    '#size' => 3,
  );
  $form['math_captcha_addition']['math_captcha_addition_allow_negative'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow negative values.'),
    '#default_value' => variable_get('math_captcha_addition_allow_negative', FALSE),
  );
  // Subtraction challenge
  $form['math_captcha_subtraction'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subtraction challenge: x - y = z'),
  );
  $form['math_captcha_subtraction']['math_captcha_subtraction_argmax'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum value for x and y'),
    '#default_value' => variable_get('math_captcha_subtraction_argmax', 10),
    '#maxlength' => 3,
    '#size' => 3,
  );
  $form['math_captcha_subtraction']['math_captcha_subtraction_allow_negative'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow negative values.'),
    '#default_value' => variable_get('math_captcha_subtraction_allow_negative', FALSE),
  );
  // Multiplication challenge
  $form['math_captcha_multiplication'] = array(
    '#type' => 'fieldset',
    '#title' => t('Multiplication challenge: x * y = z'),
  );
  $form['math_captcha_multiplication']['math_captcha_multiplication_argmax'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum value for x and y'),
    '#default_value' => variable_get('math_captcha_multiplication_argmax', 5),
    '#maxlength' => 3,
    '#size' => 3,
  );
  $form['math_captcha_multiplication']['math_captcha_multiplication_allow_negative'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow negative values.'),
    '#default_value' => variable_get('math_captcha_multiplication_allow_negative', FALSE),
  );

  // Add validation function
  $form['#validate'][] = 'math_captcha_settings_form_validate';
  return system_settings_form($form);
}

/**
 * Validation function for Math CAPTCHA settings form
 */
function math_captcha_settings_form_validate($form, &$form_state) {
  // Check enabled challenges
  if (count(array_filter($form_state['values']['math_captcha_enabled_challenges'])) < 1) {
    form_set_error('math_captcha_enabled_challenges', t('You should select at least one type of math challenges.'));
  }
  // Check argmax's
  $argmaxs = array('math_captcha_addition_argmax', 'math_captcha_subtraction_argmax', 'math_captcha_multiplication_argmax');
  foreach ($argmaxs as $argmax) {
    if (!ctype_digit($form_state['values'][$argmax])) {
      form_set_error($argmax, t('Maximum value should be an integer.'));
    }
    else {
      $form_state['values'][$argmax] = intval($form_state['values'][$argmax]);
      if ($form_state['values'][$argmax] < 2) {
        form_set_error($argmax, t('Maximum value should be an integer and at least 2'));
      }
    }
  }
}
