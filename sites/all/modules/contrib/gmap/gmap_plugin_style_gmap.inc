<?php

/**
 * @file
 * GMap style plugin.
 */

/**
 * Style plugin to render a map.
 *
 * @ingroup views_style_plugins
 */
// @codingStandardsIgnoreStart
class gmap_plugin_style_gmap extends views_plugin_style {
  /**
   * Set default options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['macro'] = array(
      'default' => '[gmap ]',
    );

    $options['datasource'] = array(
      'default' => 'location',
    );

    $options['markers'] = array('default' => 'static');
    $options['markertype'] = array('default' => 'drupal');

    $options['latfield'] = array('default' => '');
    $options['lonfield'] = array('default' => '');
    $options['markerfield'] = array('default' => '');
    $options['geofield'] = array('default' => '');

    $options['enablermt'] = array('default' => FALSE);
    $options['rmtfield'] = array('default' => '');
    $options['rmtfieldraw'] = array('default' => TRUE);
    $options['rmtcallback'] = array('default' => '');

    $options['center_on_nodearg'] = array('default' => 0);
    $options['center_on_nodearg_arg'] = array('default' => '');

    $options['center_on_proximityarg'] = array('default' => 0);

    $options['animation'] = array('default' => 0);

    $options['highlight_nodearg'] = array('default' => 0);
    $options['highlight_nodearg_arg'] = array('default' => '');
    $options['highlight_nodearg_color'] = array('default' => '#FF0000');

    $options['autoclick_on_nodearg'] = array('default' => 0);
    $options['autoclick_on_nodearg_arg'] = array('default' => '');

    $options['tooltipenabled'] = array('default' => 0);
    $options['tooltipfield'] = array('default' => '');

    $options['bubbletextenabled'] = array('default' => 0);
    $options['bubbletextfield'] = array('default' => '');

    return $options;
  }

  /**
   * Query method.
   */
  public function query() {
    parent::query();

    if ($this->options['datasource'] == 'location') {
      $table = $this->view->query->ensure_table('location');
      $this->view->query->add_field($table, 'latitude', 'gmap_lat');
      $this->view->query->add_field($table, 'longitude', 'gmap_lon');
    }

    if ($this->options['markers'] == 'nodetype') {
      $this->view->query->add_field('node', 'type', 'gmap_node_type');
    }
    elseif ($this->options['markers'] == 'taxonomy') {
      $this->view->query->add_field('gmap_taxonomy_node', 'marker', 'gmap_node_marker');
    }
    elseif ($this->options['markers'] == 'userrole') {
      $this->view->query->add_field('users_roles', 'rid', 'gmap_role_marker');
    }

    if (isset($this->row_plugin)) {
      $this->row_plugin->query();
    }
  }

  /**
   * Render the display in this style.
   */
  public function render() {

    if (isset($this->view->live_preview) && $this->view->live_preview) {
      return t('GMap views are not compatible with live preview.');
    }

    if (empty($this->row_plugin)) {
      vpr('gmap_plugin_style_gmap: Missing row plugin');
      return;
    }

    $defaults = gmap_defaults();

    if ($this->options['datasource'] == 'location') {
      $lat_field = 'gmap_lat';
      $lon_field = 'gmap_lon';
    }
    else {
      if ($this->options['datasource'] == 'geofield') {
        // "[0]['raw']['lat']";
        $lat_field = 'field_' . $this->options['geofield'];
        // "[0]['raw']['lon']";
        $lon_field = 'field_' . $this->options['geofield'];
      }
      else {
        if ($this->options['datasource'] == 'geolocation') {
          // "[0]['raw']['lat']";
          $lat_field = 'field_' . $this->options['geolocation'];
          // "[0]['raw']['lng']";
          $lon_field = 'field_' . $this->options['geolocation'];
        }
        // Determine fieldname for latitude and longitude fields.
        else {
          if ($this->options['datasource'] == 'fields') {
            // "[0]['raw']['value']";
            $lat_field = 'field_' . $this->options['latfield'];
            // "[0]['raw']['value']";
            $lon_field = 'field_' . $this->options['lonfield'];
          }
        }
      }
    }

    // Determine fieldname for marker field.
    if ($this->options['markers'] == 'field') {
      $marker_field_obj = $this->view->display_handler->get_handler('field', $this->options['markerfield']);
      $marker_field = $marker_field_obj->field_info['type'] == 'gmap_marker' ?
        "field_$marker_field_obj->field" : $marker_field_obj->field_alias;
    }

    // Determine rmt field.
    if ($this->options['enablermt']) {
      $rmt_field_obj = $this->view->display_handler->get_handler('field', $this->options['rmtfield']);
      $rmt_field = $rmt_field_obj->field_alias;
    }

    $markertypes = variable_get('gmap_node_markers', array());
    if ($this->options['markers'] == 'nodetype') {
      $markertypes = variable_get('gmap_node_markers', array());
    }
    elseif ($this->options['markers'] == 'userrole') {
      $markertypes = variable_get('gmap_role_markers', array(DRUPAL_AUTHENTICATED_RID => 'drupal'));
    }

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    // Let's make sure the individual fields are rendered.
    $this->render_fields($this->view->result);

    // Render each group separately and concatenate.
    // Plugins may override this method if they wish
    // some other way of handling grouping.
    $output = '';
    foreach ($sets as $title => $records) {
      $markername = isset($this->options['markertype']) ? $this->options['markertype'] : 'drupal';
      $markers = array();
      $offsets = array();
      $center_lat = NULL;
      $center_lon = NULL;
      $center_nid = NULL;
      $highlight_nid = NULL;
      $autoclick_nid = NULL;

      // We search nid argument used to center map, autoclick or highlight.
      if ($this->options['center_on_nodearg'] && $nodehandler = $this->view->display_handler->get_handler('argument', $this->options['center_on_nodearg_arg'])) {
        $center_nid = $nodehandler->get_value();
      }
      // Use the distance contextual filter argument to center the map.
      if ($this->options['center_on_proximityarg'] && $this->options['datasource'] == 'location' && module_exists('location')) {
        // Just grab this straight from the view.
        $distance_contextual_filter = $this->view->display_handler->get_handler('argument', 'distance');

        $center_lat = $distance_contextual_filter->value['latitude'];
        $center_lon = $distance_contextual_filter->value['longitude'];
      }
      if ($this->options['highlight_nodearg'] && $nodehandler = $this->view->display_handler->get_handler('argument', $this->options['highlight_nodearg_arg'])) {
        $highlight_nid = $nodehandler->get_value();
      }
      if ($this->options['autoclick_on_nodearg'] && $nodehandler = $this->view->display_handler->get_handler('argument', $this->options['autoclick_on_nodearg_arg'])) {
        $autoclick_nid = $nodehandler->get_value();
      }

      foreach ($records as $row_index => $row) {
        $this->view->row_index = $row_index;

        if ($this->options['datasource'] == 'location') {
          $lat = (float) $row->{$lat_field};
          $lon = (float) $row->{$lon_field};
        }
        elseif ($this->options['datasource'] == 'geofield') {
          $lat = isset($row->{$lat_field}[0]['raw']['lat']) ? (float) $row->{$lat_field}[0]['raw']['lat'] : NULL;
          $lon = isset($row->{$lon_field}[0]['raw']['lon']) ? (float) $row->{$lon_field}[0]['raw']['lon'] : NULL;
        }
        elseif ($this->options['datasource'] == 'geolocation') {
          $lat = isset($row->{$lat_field}[0]['raw']['lat']) ? (float) $row->{$lat_field}[0]['raw']['lat'] : NULL;
          $lon = isset($row->{$lon_field}[0]['raw']['lng']) ? (float) $row->{$lon_field}[0]['raw']['lng'] : NULL;
        }
        elseif ($this->options['datasource'] == 'fields') {
          $lat = isset($row->{$lat_field}[0]['raw']['value']) ? (float) $row->{$lat_field}[0]['raw']['value'] : NULL;
          $lon = isset($row->{$lon_field}[0]['raw']['value']) ? (float) $row->{$lon_field}[0]['raw']['value'] : NULL;

          // Field is Location CCK.
          if (!$lat || !$lon) {
            $lat = isset($row->{$lat_field}[0]['raw']['latitude']) ? $row->{$lat_field}[0]['raw']['latitude'] : NULL;
            $lon = isset($row->{$lon_field}[0]['raw']['longitude']) ? $row->{$lon_field}[0]['raw']['longitude'] : NULL;
          }
        }

        // $row->nid is present in node views, views without node
        // as the base table must include the nid field,
        // which will be in $row->node_nid if present.
        // If nid for a row is required use $row_nid.
        $row_nid = isset($row->nid) ? $row->nid : (isset($row->node_nid) ? $row->node_nid : NULL);

        // If this row will be used as center map then we keep its lon/lat.
        // If there are multiple points on a single node take the first match.
        if (!empty($center_nid) && !empty($row_nid) && $center_nid == $row_nid && ($center_lon === NULL || $center_lat === NULL)) {
          $center_lon = $lon;
          $center_lat = $lat;
        }

        if ((!empty($lat) && !empty($lon))
          && ($lat != 0 && $lon != 0)
          && ($lat != 180 && $lon != 360)
        ) {
          if ($this->options['markers'] == 'nodetype') {
            if (isset($markertypes[$row->gmap_node_type])) {
              $markername = $markertypes[$row->gmap_node_type];
            }
          }
          elseif ($this->options['markers'] == 'taxonomy') {
            if (!empty($row->gmap_node_marker)) {
              $markername = $row->gmap_node_marker;
            }
          }
          elseif ($this->options['markers'] == 'userrole') {
            if (!empty($row->gmap_role_marker)) {
              $markername = $markertypes[DRUPAL_AUTHENTICATED_RID];
              if (isset($markertypes[$row->gmap_role_marker])) {
                $markername = $markertypes[$row->gmap_role_marker];
              }
            }
          }
          elseif ($this->options['markers'] == 'field' && !empty($row->{$marker_field})) {
            if ($marker_field_obj->field_info['type'] == 'image') {
              $image = $row->{$marker_field}[0]['rendered'];
              $marker_icon = image_style_url($image['#image_style'], $image['#item']['uri']);
            }
            elseif (!empty($row->{$marker_field}[0]['raw']['value'])) {
              $markername = $row->{$marker_field}[0]['raw']['value'];
            }
          }
          if (!isset($offsets[$markername])) {
            $offsets[$markername] = 0;
          }

          $tooltip = '';
          if ($this->options['tooltipenabled'] && !empty($this->rendered_fields[$row_index][$this->options['tooltipfield']])) {
            $tooltip = $this->rendered_fields[$row_index][$this->options['tooltipfield']];
          }

          $bubbletext = NULL;
          if ($this->options['bubbletextenabled'] && !empty($this->options['bubbletextfield']) && !empty($this->rendered_fields[$row_index][$this->options['bubbletextfield']])) {
            $bubbletext = $this->rendered_fields[$row_index][$this->options['bubbletextfield']];
          }

          $marker = array(
            'latitude' => $lat,
            'longitude' => $lon,
            'markername' => $markername,
            'offset' => $offsets[$markername],
            'text' => $bubbletext,
            'autoclick' => (!empty($autoclick_nid) && !empty($row_nid) && $autoclick_nid == $row_nid) ? 1 : 0,
            'opts' => array(
              'title' => $tooltip,
              'highlight' => (!empty($highlight_nid) && !empty($row_nid) && $highlight_nid == $row_nid) ? 1 : 0,
              'highlightcolor' => $this->options['highlight_nodearg_color'],
              'animation' => $this->options['animation'],
            ),
          );

          if ($this->options['markers'] == 'field' && isset($marker_icon)) {
            $marker['opts']['icon'] = $marker_icon;
            unset($marker_icon);
          }

          // RMT mode.
          if ($this->options['enablermt']) {
            if ($this->options['rmtfieldraw']) {
              $marker['rmt'] = $row->{$rmt_field};
            }
            else {
              $marker['rmt'] = $this->rendered_fields[$row_index][$rmt_field_obj->field];
            }
          }
          elseif (!empty($defaults['markermode'])) {
            // Marker mode: popup.
            if ($defaults['markermode'] == 1) {
              $marker['text'] = $this->row_plugin->render($row);
            }
            // Marker mode: link.
            elseif ($defaults['markermode'] == 2) {
              $marker['link'] = url('node/' . $row_nid);
            }
          }
          $markers[] = $marker;

          $offsets[$markername]++;
        }
      }
      // Don't draw empty maps.
      if (!empty($markers)) {
        $map = gmap_parse_macro($this->options['macro']);
        if ($this->options['enablermt']) {
          $map['rmtcallback'] = $this->options['rmtcallback'];
        }

        // If center lon/lat are not empty they are used to center map.
        if (!empty($center_lon) && !empty($center_lat)) {
          $map['longitude'] = $center_lon;
          $map['latitude'] = $center_lat;
        }

        if ($this->options['markers'] == 'field') {
          $map['behavior']['customicons'] = TRUE;
        }
        $map['markers'] = $markers;
        $output .= theme($this->theme_functions(), array(
          'view' => $this->view,
          'options' => $this->options,
          'rows' => $map,
          'title' => $title,
        ));
      }
    }

    unset($this->view->row_index);
    return $output;
  }

  /**
   * Render the given style.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $field_options = array();
    $fields = $this->display->handler->get_handlers('field');
    foreach ($fields as $handler) {
      $field_options[$handler->options['id']] = (!empty($handler->options['label'])) ? $handler->options['label'] : $handler->options['id'];
    }

    $argument_options = array();
    $arguments = $this->display->handler->get_handlers('argument');
    foreach ($arguments as $id => $handler) {
      $argument_options[$id] = $handler->ui_name(FALSE);
    }

    $form['macro'] = array(
      '#type' => 'textarea',
      '#title' => t('Macro'),
      '#rows' => 3,
      '#default_value' => $this->options['macro'],
      '#description' => t('Enter a macro. If left unchanged, it takes the default settings from <a href="!link">/admin/config/services/gmap</a>. See the <a href="http://drupal.org/documentation/modules/gmap/macros">documentation on GMap macros</a>.', array('!link' => url('admin/config/services/gmap'))),
    );

    $form['datasource'] = array(
      '#type' => 'select',
      '#title' => t('Data Source'),
      '#options' => array(
        'location' => t('Location.module'),
        'geofield' => t('Geofield.module'),
        'geolocation' => t('Geolocation.module'),
        'fields' => t('Choose latitude and longitude fields'),
        // 'geocode' => t('Just-in-time geocoding on field named "address"'),
      ),
      '#default_value' => $this->options['datasource'],
      '#multiple' => FALSE,
    );

    $form['latfield'] = array(
      '#title' => t('Latitude field'),
      '#description' => t('Format must be degrees decimal.'),
      '#type' => 'select',
      '#options' => $field_options,
      '#default_value' => $this->options['latfield'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-datasource' => array('fields')),
    );

    $form['lonfield'] = array(
      '#title' => t('Longitude field'),
      '#description' => t('Format must be degrees decimal.'),
      '#type' => 'select',
      '#options' => $field_options,
      '#default_value' => $this->options['lonfield'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-datasource' => array('fields')),
    );

    $form['geofield'] = array(
      '#title' => t('Geofield field'),
      '#description' => t('Select the Geofield source field.'),
      '#type' => 'select',
      '#options' => $field_options,
      '#default_value' => isset($this->options['geofield']) ? $this->options['geofield'] : NULL,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-datasource' => array('geofield')),
    );

    $form['geolocation'] = array(
      '#title' => t('Geolocation field'),
      '#description' => t('Select the Geolocation source field.'),
      '#type' => 'select',
      '#options' => $field_options,
      '#default_value' => isset($this->options['geolocation']) ? $this->options['geolocation'] : NULL,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-datasource' => array('geolocation')),
    );

    $form['markers'] = array(
      '#type' => 'select',
      '#title' => t('Marker handling'),
      // @@@ Detect view type automatically?
      '#options' => array(
        'nodetype' => t('By content type (for node views)'),
        'taxonomy' => t('By term (for node views)'),
        'userrole' => t('By user role (for user views)'),
        'field' => t('Use marker field'),
        'static' => t('Use single marker type'),
      ),
      '#default_value' => $this->options['markers'],
    );

    $form['markerfield'] = array(
      '#type' => 'select',
      '#title' => t('Marker field'),
      '#description' => t('You can use a views field to set the <em>markername</em> property of the markers.'),
      '#options' => $field_options,
      '#default_value' => $this->options['markerfield'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-markers' => array('field')),
    );

    $form['enablermt'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable GMap RMT for markers'),
      '#description' => t('You can pull the bodies of the markers from a callback instead of defining them inline. This is a performance feature for advanced users. <strong>NOTE:</strong> This feature is <strong>not compatible</strong> with the popup bubble feature below.  Enable only one of them.'),
      '#default_value' => $this->options['enablermt'],
    );
    $form['rmtfield'] = array(
      '#type' => 'select',
      '#title' => t('RMT field'),
      '#description' => t('You can use a views field to define the "tail" of the path called back.'),
      '#options' => $field_options,
      '#default_value' => $this->options['rmtfield'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-enablermt' => array(TRUE)),
    );
    $form['rmtfieldraw'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use raw value of RMT field'),
      '#description' => t('You can use a raw value of the RMT field or a displayed value, the displayed value is different from the raw value if the field output is rewrited in the field settings.'),
      '#default_value' => $this->options['rmtfieldraw'],
      '#dependency' => array('edit-style-options-enablermt' => array(TRUE)),
    );
    $form['rmtcallback'] = array(
      '#type' => 'textfield',
      '#title' => t('RMT callback path'),
      '#description' => t('Define the base path to the callback here. The value of the rmt field will be appended.'),
      '#default_value' => $this->options['rmtcallback'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-enablermt' => array(TRUE)),
    );

    // Hide the taxonomy handling if gmap_taxonomy.module isn't installed.
    if (!module_exists('gmap_taxonomy')) {
      unset($form['markers']['#options']['taxonomy']);
    }

    $form['markertype'] = array(
      '#type' => 'gmap_markerchooser',
      '#title' => t('Marker / fallback marker to use'),
      '#default_value' => $this->options['markertype'],
    );

    $form['center_on_nodearg'] = array(
      '#type' => 'checkbox',
      '#title' => t('Center on node argument'),
      '#default_value' => $this->options['center_on_nodearg'],
      '#description' => ($this->view->base_table == 'node') ? t('Note: The view must contain an argument whose value is a node ID.') : t('Note: The view must contain an argument whose value is a node ID.') . '<br />' . t("The view must contain 'Node: nid' as one of its fields because the view type is not 'Node'."),
    );
    $form['center_on_nodearg_arg'] = array(
      '#title' => t('Argument'),
      '#description' => empty($argument_options) ? t("The value of the selected argument must be a number that matches a node ID.  Use the 'Global: Null' argument if you don't want to also restrict results to that node ID.  You must have added arguments to the view to use this option.") : t("The selected argument must be a number that matches a node ID.  Use the 'Global: Null' argument if you don't want to also restrict results to that node ID."),
      '#type' => 'select',
      '#options' => $argument_options,
      '#default_value' => $this->options['center_on_nodearg_arg'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-center-on-nodearg' => array(TRUE)),
    );

    if (module_exists('location')) {
      $form['center_on_proximityarg'] = array(
        '#type' => 'checkbox',
        '#title' => t('Center on proximity contextual filter'),
        '#default_value' => $this->options['center_on_proximityarg'],
        '#description' => t('Note: The view must contain a "Location: Distance / Proximity" contextual filter'),
      );
    }

    // Use the values from google.maps.Animation.
    // See https://developers.google.com/maps/documentation/javascript/reference
    $form['animation'] = array(
      '#type' => 'select',
      '#options' => array(0 => 'None', 1 => 'Bounce', 2 => 'Drop'),
      '#title' => t('Animation'),
      '#default_value' => $this->options['animation'],
      '#description' => t("Add animation to the markers."),
    );
    $form['highlight_nodearg'] = array(
      '#type' => 'checkbox',
      '#title' => t('Highlight marker for node argument'),
      '#default_value' => $this->options['highlight_nodearg'],
      '#description' => ($this->view->base_table == 'node') ? t('Note: The view must contain an argument whose value is a node ID.') : t('Note: The view must contain an argument whose value is a node ID.') . '<br />' . t("The view must contain 'Node: nid' as one of its fields because the view type is not 'Node'."),
    );
    $form['highlight_nodearg_arg'] = array(
      '#title' => t('Argument'),
      '#description' => empty($argument_options) ? t("The value of the selected argument must be a number that matches a node ID.  Use the 'Global: Null' argument if you don't want to also restrict results to that node ID.  You must have added arguments to the view to use this option.") : t("The value of the selected argument must be a number that matches a node ID.  Use the 'Global: Null' argument if you don't want to also restrict results to that node ID."),
      '#type' => 'select',
      '#options' => $argument_options,
      '#default_value' => $this->options['highlight_nodearg_arg'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-highlight-nodearg' => array(TRUE)),
    );
    $form['highlight_nodearg_color'] = array(
      '#title' => t('Highlight color'),
      '#description' => t("A 6 digit hex color value to use for the highlight. Include preceding hash. Example #FF0000"),
      '#type' => 'textfield',
      '#size' => 7,
      '#maxlength' => 7,
      '#default_value' => $this->options['highlight_nodearg_color'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-highlight-nodearg' => array(TRUE)),
    );

    $form['autoclick_on_nodearg'] = array(
      '#type' => 'checkbox',
      '#title' => t('Autoclick on node argument'),
      '#default_value' => $this->options['autoclick_on_nodearg'],
      '#description' => ($this->view->base_table == 'node') ? t('Note: The view must contain an argument whose value is a node ID.') : t('Note: The view must contain an argument whose value is a node ID.') . '<br />' . t("The view must contain 'Node: nid' as one of its fields because the view type is not 'Node'."),
    );
    $form['autoclick_on_nodearg_arg'] = array(
      '#title' => t('Argument'),
      '#description' => empty($argument_options) ? t("The value of the selected argument must be a number that matches a node ID.  Use the 'Global: Null' argument if you don't want to also restrict results to that node ID.  You must have added arguments to the view to use this option.") : t("The selected argument must be a number that matches a node ID.  Use the 'Global: Null' argument if you don't want to also restrict results to that node ID."),
      '#type' => 'select',
      '#options' => $argument_options,
      '#default_value' => $this->options['autoclick_on_nodearg_arg'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-autoclick-on-nodearg' => array(TRUE)),
    );

    $form['tooltipenabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display a tooltip when hovering over markers'),
      '#default_value' => $this->options['tooltipenabled'],
    );
    $form['tooltipfield'] = array(
      '#title' => t('Tooltip field'),
      '#description' => empty($field_options) ? t("The field's format must be text.  You must be using the fields row style and have added fields to the view to use this option.") : t("The field's format must be text."),
      '#type' => 'select',
      '#options' => $field_options,
      '#default_value' => $this->options['tooltipfield'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-tooltipenabled' => array(TRUE)),
    );
    $form['bubbletextenabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display a popup bubble with additional information when a marker is clicked.'),
      '#default_value' => $this->options['bubbletextenabled'],
      '#description' => t('<strong>NOTE:</strong> This feauture is <strong>not compatible</strong> with the RMT option offered above.  Do not use them together.'),
    );
    $form['bubbletextfield'] = array(
      '#title' => t('Bubble pop-up field'),
      '#description' => empty($field_options) ? t("The field's format must be HTML or text.  You must be using the fields row style and have added fields to the view to use this option.") : t("The field's format must be HTML or text."),
      '#type' => 'select',
      '#options' => $field_options,
      '#default_value' => $this->options['bubbletextfield'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-bubbletextenabled' => array(TRUE)),
    );

  }

  /**
   * Validate the options form.
   */
  public function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);
    // Check if highlight color is a valid hex color.
    if (!preg_match('/^#[a-f0-9]{6}$/i', $form_state['values']['style_options']['highlight_nodearg_color'])) {
      form_error($form['highlight_nodearg_color'], t('Highlight colour must be a valid hex code in the form #FF0000.'));
    }
  }
}
// @codingStandardsIgnoreEnd
