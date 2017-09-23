<?php

/**
 * @file
 * Longitude field handler.
 */

// @codingStandardsIgnoreStart
class location_views_handler_field_longitude extends location_views_handler_field_latitude {

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    if ($this->options['style'] == 'dms') {
      return theme('location_longitude_dms', array('longitude' => $values->{$this->field_alias}));
    }

    return parent::render($values);
  }
}
// @codingStandardsIgnoreEnd
