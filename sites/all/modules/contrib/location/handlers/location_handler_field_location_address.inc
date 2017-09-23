<?php

/**
 * @file
 * Field handler to display a complete address.
 */

// @codingStandardsIgnoreStart
class location_handler_field_location_address extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['hide'] = array('default' => array());
    $options['exclude_cck'] = array('default' => FALSE);

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['hide'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Hide fields from display'),
      '#options' => location_field_names(TRUE),
      '#default_value' => $this->options['hide'],
    );
    $form['exclude_cck'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude Content locations'),
      '#description' => t('Filter Content (CCK) locations from this field. You can use Content views fields to display them seperately.'),
      '#default_value' => $this->options['exclude_cck'],
    );
  }

  function query() {
    if (empty($this->options['exclude_cck'])) {
      parent::query();
      return;
    }
    $genid_field = $this->query->ensure_table('location_instance', $this->relationship) . ".genid";
    $this->query->add_where(0, "$genid_field NOT LIKE '%cck%' OR $genid_field IS NULL");
  }

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    if ($values->{$this->field_alias}) {
      $location = location_load_location($values->{$this->field_alias});
      if ($location['lid']) {
        return theme('location', array('location' => $location, 'hide' => $this->options['hide']));
      }
    }
  }
}
// @codingStandardsIgnoreEnd