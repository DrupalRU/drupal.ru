<?php

/**
 * @file
 * Views 3 support for Location Email.
 */

/**
 * Implements hook_views_data().
 */
function location_email_views_data() {
  // location_email table -- basic table information.
  // Define the base group of this table.
  // Fields that don't have a group defined will go into this field by default.
  $data['location_email']['table']['group'] = t('Location', array(), array('context' => 'geolocation'));

  // Join location_email to all the tables that would be useful.
  $data['location_email']['table']['join'] = array(
    // location_email links to location via lid.
    'location' => array(
      'left_field' => 'lid',
      'field' => 'lid',
    ),
    // location_email links to node through location_instance via lid.
    'node' => array(
      'left_table' => 'location_instance',
      'left_field' => 'lid',
      'field' => 'lid',
    ),
    // location_email links to node_revision through location_instance via lid.
    'node_revision' => array(
      'left_table' => 'location_instance',
      'left_field' => 'lid',
      'field' => 'lid',
    ),
    // location_email links to users through location_instance via lid.
    'users' => array(
      'left_table' => 'location_instance',
      'left_field' => 'lid',
      'field' => 'lid',
    ),
  );

  // location_email table -- fields.
  $data['location_email']['email'] = array(
    'title' => t('Email address'),
    'help' => t('The email address of the selected location.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'empty field name' => t('None'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
  );

  return $data;
}
