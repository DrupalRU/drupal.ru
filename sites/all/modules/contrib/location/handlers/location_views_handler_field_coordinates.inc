<?php

/**
 * @file
 * Coordinates field handler.
 */

// @codingStandardsIgnoreStart
class location_views_handler_field_coordinates extends location_views_handler_field_latitude {
  /**
   * {@inheritdoc}
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['longitude'] = 'longitude';
  }

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    if ($this->options['style'] == 'dms') {
      return theme('location_latitude_dms', array('latitude' => $values->{$this->field_alias})) . ', ' . theme(
        'location_longitude_dms',
        array('longitude' => $values->{$this->aliases['longitude']})
      );
    }
    else {
      return check_plain($values->{$this->field_alias}) . ', ' . check_plain($values->{$this->aliases['longitude']});
    }
  }
}
// @codingStandardsIgnoreEnd
