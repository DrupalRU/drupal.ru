<?php

/**
 * @file
 * Additional field handler.
 */

// @codingStandardsIgnoreStart
class location_handler_field_location_additional extends views_handler_field {

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
  public function render($values) {
    return check_plain(trim($values->{$this->aliases['additional']}));
  }
}
// @codingStandardsIgnoreEnd
