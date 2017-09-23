<?php

/**
 * @file
 * Province field handler.
 */

// @codingStandardsIgnoreStart
class location_handler_field_location_province extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  public function construct() {
    parent::construct();
    $this->additional_fields = array(
      'country' => 'country',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['style'] = array('default' => 'name');

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['style'] = array(
      '#title' => t('Display style'),
      '#type' => 'select',
      '#options' => array('name' => t('Province name'), 'code' => t('Province code')),
      '#default_value' => $this->options['style'],
    );
  }

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    if ($this->options['style'] == 'name') {
      return t(
        check_plain(location_province_name($values->{$this->aliases['country']}, $values->{$this->field_alias}))
      );
    }
    else {
      // If we get a number for the province, there is not a "proper" code, we must us it's name.
      if (is_numeric($values->{$this->field_alias})) {
        return check_plain(location_province_name($values->{$this->aliases['country']}, $values->{$this->field_alias}));
      }
      else {
        return check_plain(strtoupper($values->{$this->field_alias}));
      }
    }
  }
}
// @codingStandardsIgnoreEnd
