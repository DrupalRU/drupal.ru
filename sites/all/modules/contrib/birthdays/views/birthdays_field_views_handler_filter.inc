<?php

/**
 * @file
 * The birthdays_field_views_handler_filter class.
 */

/**
 * Handler that allows to filter by birthday fields.
 */
class birthdays_field_views_handler_filter extends views_handler_filter_date {

  /**
   * Overrides views_handler_filter_date::operators().
   */
  function operators() {
    // Remove NULL form the parent operators, e.g. "Is empty (NULL)".
    $operators = parent::operators();
    $operators['empty']['title'] = t('Is empty');
    $operators['not empty']['title'] = t('Is not empty');
    return $operators;
  }

  /**
   * Overrides views_handler_filter_date::value_form().
   */
  function value_form(&$form, &$form_state) {
    // Adjust the parent option descriptions.
    parent::value_form($form, $form_state);
    if (empty($form_state['exposed'])) {
      $form['value']['type']['#options']['date'] = t('A specific date.');
      $form['value']['type']['#options']['offset'] = t('An offset from the current time in days, such as <em>-1</em> or <em>+7</em>.');
    }
  }

  /**
   * Overrides views_handler_filter_date::options_validate().
   */
  function options_validate(&$form, &$form_state) {
    // Determine the validation callback.
    if ($form_state['values']['options']['value']['type'] == 'date') {
      $validate = 'birthdays_validate_date';
    }
    else {
      $validate = 'birthdays_validate_offset';
    }

    // Infos about the operator.
    $operator = $form_state['values']['options']['operator'];
    $operators = $this->operators();

    if ($operators[$operator]['values'] == 1) {
      $this->{$validate}($form['value']['value'], $form_state['values']['options']['value']['value'], $form_state);
    }
    elseif ($operators[$operator]['values'] == 2) {
      $this->{$validate}($form['value']['min'], $form_state['values']['options']['value']['min'], $form_state);
      $this->{$validate}($form['value']['max'], $form_state['values']['options']['value']['max'], $form_state);
    }
  }

  /**
   * Validate a date.
   */
  function birthdays_validate_date($element, $value, &$form_state) {
    try {
      $birthday = BirthdaysBirthday::fromString($value, TRUE);
      form_set_value($element, $birthday->toString(), $form_state);
    }
    catch (InvalidArgumentException $e) {
      form_error($element, t('That is not a valid date.'));
    }
  }

  /**
   * Validate offset.
   */
  function birthdays_validate_offset($element, $value, &$form_state) {
    $value = strval(intval($value));
    if ($value && drupal_substr($value, 0, 1) != '-') {
      $value = '+' . $value;
    }
    form_set_value($element, $value, $form_state);
  }

  /**
   * Overrides views_handler_filter_date::op_simple().
   */
  function op_simple($field) {
    $this->birthdays_add_where($field, $this->operator, $this->value['type'], $this->value['value']);
  }

  /**
   * Overrides views_handler_filter_date::op_between().
   */
  function op_between($field) {
    $this->birthdays_add_where($field, '>=', $this->value['type'], $this->value['min']);
    $this->birthdays_add_where($field, '<=', $this->value['type'], $this->value['max']);
  }

  /**
   * Adds a where clause on a birtdhay value.
   */
  function birthdays_add_where($field, $operator, $type, $value) {
    // Get the absolute value.
    if ($type == 'offset') {
      $birthday = BirthdaysBirthday::fromOffset($value);
    }
    else {
      $birthday = BirthdaysBirthday::fromString($value, TRUE);
      if ($birthday->isEmpty()) {
        return;
      }
    }

    // Add the WHERE expression.
    $value = $birthday->getYear() * 500 + $birthday->getMonth() * 32 +  $birthday->getDay();
    if ($birthday->getYear()) {
      $this->query->add_where_expression($this->options['group'], "{$field}_year * 500 + {$field}_month * 32 + {$field}_day $operator $value");
    }
    else {
      $this->query->add_where_expression($this->options['group'], "{$field}_month * 32 + {$field}_day $operator $value");
    }
  }

  /**
   * Overrides views_handler_filter_date::op_empty().
   */
  function op_empty($field) {
    if ($this->operator == 'empty') {
      $prefix = 'NOT ';
    }
    else {
      $prefix = '';
    }

    $this->query->add_where_expression($this->options['group'], $prefix . 'COALESCE(' . $field . '_month, FALSE)');
    $this->query->add_where_expression($this->options['group'], $prefix . 'COALESCE(' . $field . '_day, FALSE)');
  }
}
