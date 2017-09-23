<?php

/**
 * @file
 * Creates a location context.
 */

/**
 * Implement hook_[context_name]_ctools_contexts().
 */
function location_location_ctools_contexts() {
  return array(
    'title' => t('Location', array(), array('context' => 'geolocation')),
    'description' => t('Location', array(), array('context' => 'geolocation')),
    'context' => 'location_context_create_location',
    'context name' => 'location',
    'settings form' => 'location_context_location_settings_form',
    'keyword' => 'location',
    'context name' => 'location',
    'convert list' => 'location_context_location_convert_list',
    'convert' => 'location_context_location_convert',
  );
}

/**
 * Create a context, either from configuration or an argument on the URL.
 *
 * @param bool $empty
 *   If true, just return an empty context.
 *
 * @param array $data
 *   If from settings form, a form values array. If from argument, a string.
 *
 * @param bool $conf
 *   TRUE if the $data is coming from admin configuration, FALSE if it's
 *   from a URL arg.
 *
 * @return object
 *   a Context object
 */
function location_context_create_location($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('location');
  $context->plugin = 'location';

  if ($empty) {
    return $context;
  }

  if ($conf) {
    $lid = is_array($data) && isset($data['lid']) ? $data['lid'] : (is_object($data) ? $data->lid : 0);
    if (is_array($data) || !empty($reload)) {
      $nid = isset($data['nid']) ? $data['nid'] : NULL;
      $data = location_load_location($lid);
      $data['nid'] = is_null($nid) ? FALSE : $nid;
    }
  }

  if (!empty($data)) {
    $context->data = $data;
    $context->title = isset($data['city']) ? $data['city'] : '';
    $context->argument = $data['lid'];

    return $context;
  }
}

/**
 * Settings form.
 */
function location_settings_form($conf, $external = FALSE) {
  return array();
}

/**
 * Provide a list of ways that this context can be converted to a string.
 */
function location_context_location_convert_list() {
  $fields = location_field_names();
  $fields['country_name'] = t('Country name');
  $fields['province_name'] = t('State/Province name');

  return $fields;
}

/**
 * Convert a context into a string.
 */
function location_context_location_convert($context, $type) {
  $fields = location_context_location_convert_list();
  if (isset($fields[$type]) && isset($context->data[$type])) {
    return check_plain($context->data[$type]);
  }

  return t('Unknown location keyword');
}
