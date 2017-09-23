<?php

/**
 * @file
 * Country sort handler.
 */

// @codingStandardsIgnoreStart
class location_handler_sort_location_country extends views_handler_sort {

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['country_sort'] = array('default' => 'name');

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function has_extra_options() {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function extra_options_form(&$form, &$form_state) {
    $form['country_sort'] = array(
      '#type' => 'radios',
      '#title' => t('Name or Code'),
      '#options' => array(
        'name' => t("Use the County Name (as specified by the database)."),
        'code' => t("Use the ISO3166 two letter country code ."),
      ),
      '#description' => t("This sets how country will be sorted, either by Name or the ISO 3166 2 letter code"),
      '#default_value' => $this->options['country_sort'],
    );
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    // Make sure the table_alias work.  This sets up some of the list pointers.
    $this->ensure_my_table();

    // This is needed otherwise the the add_relationship will not work.
    $this->query->ensure_path($this->table_alias);

    if ($this->options['country_sort'] == 'name') {
      $join = new views_join();
      $join->definition = array(
        'table' => 'location_country',
        'field' => 'code',
        'left_table' => $this->table_alias,
        'left_field' => 'country',
      );
      if (!empty($this->options['required'])) {
        $join->definition['type'] = 'INNER';
      }
      $join->construct();

      $this->query->add_relationship('country_name', $join, isset($this->definition['base']) ? $this->definition['base'] : NULL, NULL);
      $this->query->add_orderby('country_name', 'name', $this->options['order'], 'country_sort');
    }
    elseif ($this->options['country_sort'] == 'code') {
      $this->query->add_orderby($this->table_alias, 'country', $this->options['order'], 'country_code_sort');
    }
  }
}
// @codingStandardsIgnoreEnd
