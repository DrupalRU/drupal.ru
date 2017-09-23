<?php

/**
 * @file
 * The birthdays_field_views_handler_field class.
 */

/**
 * A Views field handler that provides click sorting of birthday fields.
 */
class birthdays_field_views_handler_field extends views_handler_field_field {

  /**
   * Overrides views_handler_field_field::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['click_sort_mode'] = array(
      'default' => 'year',
    );

    return $options;
  }

  /**
   * Overrides views_handler_field_field::options_form().
   */
  function options_form(&$form, &$form_state) {
    // Adjust the parent form.
    parent::options_form($form, $form_state);
    unset($form['click_sort_column']);

    // Add a sorting option.
    $form['click_sort_mode'] = array(
      '#type' => 'radios',
      '#title' => 'Click sort mode',
      '#options' => array(
        'all' => t('Sort by year, month and day.'),
        'year' => t('Sort by month and day.'),
        'now' => t('Sort by time to next birthday.'),
      ),
      '#description' => t('The way this column should beclick sorted.'),
      '#fieldset' => 'more',
      '#default_value' => $this->options['click_sort_mode'],
    );
  }

  /**
   * Overrides views_handler_field_field::click_sort().
   */
  function click_sort($order) {
    // Ensure the table exists.
    $this->ensure_my_table();

    // Look up the real column names.
    $columns = array('day' => NULL, 'month' => NULL, 'year' => NULL);
    foreach ($columns as $delta => $column) {
      $column = _field_sql_storage_columnname($this->definition['field_name'], $delta);
      if (!isset($this->aliases[$column])) {
        $this->aliases[$column] = $this->table_alias . '.' . $column;
      }
      $columns[$delta] = $this->aliases[$column];
    }

    // Add sorting to the query.
    $mode = $this->options['click_sort_mode'];
    if ($mode == 'all') {
      $this->query->add_orderby(NULL, NULL, $order, $columns['year']);
    }
    if ($mode == 'all' || $mode == 'year') {
      $this->query->add_orderby(NULL, NULL, $order, $columns['month']);
      $this->query->add_orderby(NULL, NULL, $order, $columns['day']);
    }
    else {
      birthdays_field_views_handler_sort::addOrderBy($this->query, $order, $columns, $this->definition['field_name'] . '_sort');
    }
  }
}
