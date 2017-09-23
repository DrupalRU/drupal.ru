<?php

/**
 * @file
 * GMap Views support.
 */

/**
 * Gmap Extended View.
 *
 * @param array $vars
 *   Array of used variables.
 *
 * @return mixed
 *   Rendered HTML.
 */
function theme_gmap_views_view_gmapextended(&$vars) {
  $markers = array();
  foreach ($vars['markers'] as $offset => $data) {
    if (empty($data['latitude']) || empty($data['longitude'])) {
      continue;
    }
    $markers[] = $data;
  }
  // @@@ Move to preprocess.
  $map = gmap_parse_macro($vars['options']['macro']);

  // If center lon/lat are not empty they are used to center map.
  if (!empty($vars['center']['longitude']) && !empty($vars['center']['latitude'])) {
    $map['longitude'] = $vars['center']['longitude'];
    $map['latitude'] = $vars['center']['latitude'];
  }

  if (!empty($vars['options']['iwq'])) {
    $map['iwq'] = $vars['options']['iwq'];
  }
  $map['markers'] = $markers;
  $elem = array(
    '#type' => 'gmap',
    '#gmap_settings' => $map,
  );
  return drupal_render($elem);
}

/**
 * Implements template_preprocess_views_view_VIEW_NAME().
 *
 * @param array $vars
 *   Array of used variables.
 */
function template_preprocess_gmap_views_view_gmapextended(&$vars) {
  $view = $vars['view'];

  // We need the raw data for this
  // grouping, which is passed in as $vars['rows'].
  // However, the template also needs to use for the rendered fields.  We
  // therefore swap the raw data out to a new variable and reset $vars['rows']
  // so that it can get rebuilt.
  // Store rows so that they may be used by further preprocess functions.
  $result = $vars['result'] = $vars['rows'];
  $vars['rows'] = array();

  $options = $view->style_plugin->options;
  $handler = $view->style_plugin;

  $fields = & $view->field;

  if ($view->style_plugin->options['center_on_proximityfilter'] && $view->style_plugin->options['datasource'] == 'location' && module_exists('location')) {
    $vars['center'] = location_views_proximity_get_reference_location($view, array(
        'origin' => 'tied',
        'relationship' => $view->style_plugin->options['center_on_proximityfilter_rel'],
      )
    );
  }

  // Render fields.
  $renders = $handler->render_fields($result);

  $markers = array();
  // File away fields into the marker data.
  foreach ($renders as $i => $row) {
    $markers[$i] = $options['fallback_values'];
    foreach ($options['field_purposes'] as $field => $purpose) {
      if (!isset($markers[$i][$purpose])) {
        $markers[$i][$purpose] = '';
      }
      if ($purpose){
        $markers[$i][$purpose] .= $row[$field];
      }
    }
  }
  /*
  foreach ($options['field_purposes'] as $field => $purpose) {
  foreach ($renders as $i => $row) {
  if (!isset($markers[$i][$purpose])) {
  $markers[$i][$purpose] = '';
  }
  $markers[$i][$purpose] .= $row[$field];
  }
  }
  */

  foreach ($markers as $offset => $marker) {
    // Set up 'opts' array to make tooltips work.
    $markers[$offset]['opts'] = array('title' => isset($marker['title']) ? decode_entities($marker['title']) : NULL);

    switch ($options['clickmode']) {
      case 'render':
        $markers[$offset]['text'] = $vars['result'][$offset];
        // Fallthrough.
      case 'text':
        unset ($markers[$offset]['link']);
        unset ($markers[$offset]['rmt']);
        unset ($markers[$offset]['iwq']);
        unset ($markers[$offset]['iwo']);
        break;

      case 'rmt':
        unset ($markers[$offset]['text']);
        unset ($markers[$offset]['link']);
        unset ($markers[$offset]['iwq']);
        unset ($markers[$offset]['iwo']);
        break;

      case 'link':
        unset ($markers[$offset]['text']);
        unset ($markers[$offset]['rmt']);
        unset ($markers[$offset]['iwq']);
        unset ($markers[$offset]['iwo']);
        break;

      case 'iwq':
        // @@@ May need to parse entities for stuff like .foo>.bar.
        $markers[$offset]['iwq'] = trim($markers[$offset]['iwq']);
        $markers[$offset]['iwo'] = (int) trim($markers[$offset]['iwo']);
        if (empty($markers[$offset]['iwq']) || $markers[$offset]['iwq'] == $options['iwq']) {
          unset($markers[$offset]['iwq']);
        }

        unset ($markers[$offset]['text']);
        unset ($markers[$offset]['link']);
        unset ($markers[$offset]['rmt']);
        break;
    }
  }

  // @@@ add rest of handling
  /*
  if ($options['markers'] == 'static') {
  foreach ($markers as $offset => $marker) {
  $markers[$offset]['markername'] = $options['markertype'];
  }
  }
  */

  // Marker cleanup.
  foreach ($markers as $offset => $marker) {
    $marker['latitude'] = isset($marker['latitude']) ? (float) $marker['latitude'] : NULL;
    $marker['longitude'] = isset($marker['longitude']) ? (float) $marker['longitude'] : NULL;
  }

  $vars['markers'] = $markers;
}
