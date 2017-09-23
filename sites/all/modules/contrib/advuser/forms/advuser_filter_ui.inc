<?php
// $Id$
/**
 * @file
 *
 * This provides a multipart form for the filtering options.
 *
 */

/**
 * The phase controller.
 */
function advuser_filter_ui($form_state) {
  $advuser = &$_SESSION['advuser'];
  $phase = &$advuser['phase'];
  $filters = &$advuser['filters'];
  $accounts = &$advuser['accounts'];
  $selectall = &$advuser['selectall'];

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only users'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#theme' => 'exposed_filters__user',
  );

  if (empty($phase)) {
    $phase['id'] = 0;
  }

  if ($phase['id'] == 0 && (count($accounts) || $selectall)) {
    $form['filters']['#collapsed'] = TRUE;
  }

  if (empty($filters)) {
    $filters = array();
  }

  $filter_fields = advuser_filters();
  $i = 0;
  $filtersCnt = count($filters);

  foreach ($filters as $filter) {
    list($field, $conjunction, $operations, $data) = array_values($filter);
    $conjunction = ($conjunction == ') OR (') ? 'OR' : $conjunction;
    if ($i == 0) {
      $oparen = '(';
    }
    elseif ($filtersCnt > 1) {
      $oparen = '';
    }
    if ($i == $filtersCnt - 1) {
      $cparen = ')';
    }
    if (($i + 1) < $filtersCnt) {
      if ($filters[$i + 1]['conjunction'] == ') OR (') {
        $cparen = ')';
      }
      else {
        $cparen = '';
      }
    }
    if ($conjunction == 'OR') {
      $oparen = '(';
    }
    $params = array(
      '%property' => $filter_fields[$field]['title'],
      '%value' => _advuser_filter_ui_data_display($field, $data),
    );
    $form['filters']['current'][] = array(
      '#markup' => t(
                    (!empty($conjunction) ? "<em>{$conjunction}</em> " : NULL) .
                    $oparen .
                    'where <strong>%property</strong> ' .
                    _advuser_filter_ui_symbols($operations) .
                    ' <strong>%value</strong>',
                    $params
                   ) .
                   $cparen,
    );
    $oparen = '';
    $i++;
  }

  $function = "_advuser_filter_ui_$phase[id]";
  $function($form, $form_state);
  drupal_add_library('system', 'drupal.form');
  return $form;
}

function advuser_filter_ui_submit(&$form, &$form_state) {
  $phase = &$form_state['storage']['phase'];
  $function = "_advuser_filter_ui_$phase[id]_submit";
  return $function($form, $form_state);
}

/**
 * The field selection phase.
 */
function _advuser_filter_ui_0(&$form, &$form_state) {
  $advuser = &$_SESSION['advuser'];
  $filters = &$advuser['filters'];
  $accounts = &$advuser['accounts'];
  $selectall = &$advuser['selectall'];
  $deselected = &$advuser['deselected'];

  $form['filters']['conjunction'] = array(
    '#type' => 'select',
    '#title' => t('Filter conjunction'),
    '#options' => array('AND' => t('and'), ') OR (' => t('or')),
  );

  if (!count($filters)) {
    $form['filters']['conjunction']['#type'] = 'hidden';
    $form['filters']['conjunction']['#default_value'] = 'AND';
  }

  $form['filters']['field'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#description' => t('Select the field for this filter'),
    '#options' => _advuser_filter_ui_fields(),
  );

  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue'),
    '#submit' => array('_advuser_filter_ui_0_submit'),
  );
  if (count($filters)) {
    $form['filters']['undo'] = array(
      '#type' => 'submit',
      '#value' => t('Undo'),
      '#submit' => array('_advuser_filter_ui_0_undo'),
    );
    $form['filters']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#submit' => array('_advuser_filter_ui_0_reset'),
    );
  }
  if (count($accounts) || $selectall) {
    $form['filters']['accountsselected'] = array(
      '#value' => t('You will lose your selections by changing the filters.'),
      '#prefix' => '<span class="error">',
      '#suffix' => '</span>',
    );
  }
}

/**
 * Submit controller for the Continue button.
 */
function _advuser_filter_ui_0_submit(&$form, &$form_state) {
  $advuser = &$_SESSION['advuser'];
  $phase = &$advuser['phase'];
  $filters = &$advuser['filters'];
  $phase['values']['field'] = $form_state['values']['field'];
  if (count($filters)) {
    $phase['values']['conjunction'] = $form_state['values']['conjunction'];
  }
  else {
    $phase['values']['conjunction'] = '';
  }
  $phase['id'] = 1;
}

/**
 * Submit controller for the Undo button.
 */
function _advuser_filter_ui_0_undo(&$form, &$form_state) {
  $advuser = &$_SESSION['advuser'];
  $filters = &$advuser['filters'];
  $accounts = &$advuser['accounts'];
  $selectall = &$advuser['selectall'];
  array_pop($filters);
  $accounts = array();
  $selectall = FALSE;
}

/**
 * Submit controller for the Reset button.
 */
function _advuser_filter_ui_0_reset(&$form, &$form_state) {
  $advuser = &$_SESSION['advuser'];
  $filters = &$advuser['filters'];
  $accounts = &$advuser['accounts'];
  $selectall = &$advuser['selectall'];
  $filters = array();
  $accounts = array();
  $selectall = FALSE;
}

/**
 * The operator and data value entry
 */
function _advuser_filter_ui_1(&$form, &$form_state) {
  $advuser = &$_SESSION['advuser'];
  $phase = &$advuser['phase'];
  $field = &$phase['values']['field'];
  $conjunction = &$phase['values']['conjunction'];
  $filters = &$advuser['filters'];

  $form['filters']['conjunction'] = array(
    '#type' => 'select',
    '#title' => t('Filter conjunction'),
    '#options' => array('AND' => t('and'), ') OR (' => t('or')),
    '#disabled' => TRUE,
    '#value' => $conjunction,
    '#default_value' => $conjunction,
  );
  if (!count($filters)) {
    $form['filters']['conjunction']['#type'] = 'hidden';
    $form['filters']['conjunction']['#default_value'] = 'AND';
    $form['filters']['conjunction']['#value'] = 'AND';
  }

  $form['filters']['field'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#description' => t('Select the field for this filter'),
    '#options' => _advuser_filter_ui_fields(),
    '#disabled' => TRUE,
    '#value' => $field,
    '#default_value' => $field,
  );

  $form['filters']['operations'] = array(
    '#type' => 'select',
    '#title' => t('Operator'),
    '#options' => _advuser_filter_ui_operations($field),
  );

  $form['filters']['data'] = array(
    '#type' => _advuser_filter_ui_type($field),
    '#title' => t('Data'),
    '#description' => _advuser_filter_ui_desc($field),
  );

  if ($form['filters']['data']['#type'] == 'select') {
    $form['filters']['data']['#options'] = _advuser_filter_ui_options($field);
  }

  if ($field == 'created' || $field == 'access') {
    $form['filters']['message'] = array(
      '#value' => t('Note, to select dates created today use an operator of "is greater than or equal to" with a data value of "today". Use a combination of "is greater than or equal to" "yesterday" AND "less than" "today" to select dates created yesterday.'),
      '#prefix' => '<div class="advuser_message">',
      '#suffix' => '</div><br/>',
    );
  }
  elseif ($field == 'user_roles') {
    $form['filters']['message'] = array(
      '#value' => t('Note, you will not be able to select is equal to ROLE1 and is equal to ROLE2.'),
      '#prefix' => '<div class="advuser_message">',
      '#suffix' => '</div><br/>',
    );
  }

  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => (count($filters) ? t('Refine') : t('Filter')),
    '#submit' => array('_advuser_filter_ui_1_submit'),
  );
  $form['filters']['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#submit' => array('_advuser_filter_ui_1_back'),
  );
}

/**
 * Submit controller for the Filter and Refine button.
 */
function _advuser_filter_ui_1_submit(&$form, &$form_state) {
  $advuser = &$_SESSION['advuser'];
  $phase = &$advuser['phase'];
  $filters = &$advuser['filters'];
  $accounts = &$advuser['accounts'];
  $selectall = &$advuser['selectall'];

  $phase['values']['operations'] = $form_state['values']['operations'];
  $phase['values']['data'] = $form_state['values']['data'];
  $filters[] = $phase['values'];
  $phase['values'] = array();
  $phase['id'] = 0;
  $accounts = array();
  $selectall = FALSE;
}

/**
 * Submit controller for the Back button.
 */
function _advuser_filter_ui_1_back(&$form, &$form_state) {
  $advuser = &$_SESSION['advuser'];
  $phase = &$advuser['phase'];
  $phase['values'] = array();
  $phase['id'] = 0;
}

/**
 * Get a list of operators based on field.
 *
 * @param $field
 * @return array of operators.
 */
function _advuser_filter_ui_operations($field) {
  $symbols = array();
  switch ($field) {
    case 'user_roles':
    case 'permission': {
      $symbols += array('=' => _advuser_filter_ui_symbols('='));
      $symbols += array('!=' => _advuser_filter_ui_symbols('!='));
    } break;
    case 'status': {
      $symbols += array('=' => _advuser_filter_ui_symbols('='));
    } break;
    case 'uid':
    case 'last_access':
    case 'created': {
      $symbols += array('=' => _advuser_filter_ui_symbols('='));
      $symbols += array('!=' => _advuser_filter_ui_symbols('!='));
      $symbols += array('<' => _advuser_filter_ui_symbols('<'));
      $symbols += array('<=' => _advuser_filter_ui_symbols('<='));
      $symbols += array('>' => _advuser_filter_ui_symbols('>'));
      $symbols += array('>=' => _advuser_filter_ui_symbols('>='));
    } break;
    default: {
      $symbols = _advuser_filter_ui_symbols();
    }
  }

  return $symbols;
}

/**
 * Get a list of fields to select.
 */
function _advuser_filter_ui_fields() {
  $fields = advuser_filters();
  foreach ($fields as $key => $filter) {
    $filter_fields[$key] = $filter['title'];
  }
  return $filter_fields;
}

/**
 * Determine the type of field based on the field name.
 *
 * @param $field
 * @return field type.
 */
function _advuser_filter_ui_type($field) {
  switch ($field) {
    case 'user_roles':
    case 'permission':
    case 'status': {
      $ret = 'select';
    } break;
    // Yea, these are date fields but we want textfield input so that we can
    // use phrases like 3 weeks ago.
    case 'created':
    case 'last_access': {
      $ret = 'textfield';
    } break;
    default: {
      $ret = 'textfield';
    } break;
  }
  return $ret;
}

/**
 * Get the description for the data to enter based on the field name.
 */
function _advuser_filter_ui_desc($field) {
  switch ($field) {
    default: {
      $ret = "Enter the data value for this filter.";
    }
  }
  return t($ret);
}

/**
 * Get the option selectable text.
 */
function _advuser_filter_ui_options($field) {
  $options = array();
  switch ($field) {
    case 'permission': {
      foreach (module_implements('permission') as $module) {
        $function = $module . '_permission';
        if ($permissions = $function('permission')) {
          asort($permissions);
          foreach ($permissions as $permission => $description) {
            $options[t('@module module', array('@module' => $module))][$permission] = t($permission);
          }
        }
      }
    } break;
    case 'status': {
      $options = array(1 => t('active'), 0 => t('blocked'));
    } break;
    case 'user_roles': {
      $result = db_select('role', 'r')
        ->fields('r', array('rid', 'name'))
        ->condition('r.rid', array(0, 1, 2), 'NOT IN')
        ->execute();
      $options[NULL] = t('No role assigned');
      foreach ($result as $role) {
        $options[$role->rid] = $role->name;
      }
    } break;
  }
  return $options;
}

/**
 * Helper function for translating operation symbols to language
 */
function _advuser_filter_ui_symbols($symbol = NULL) {
  static $symbols = NULL;
  if (!isset($symbols)) {
    $symbols = array(
      '=' => t('is equal to'),
      '!=' => t('is not equal to'),
      '<' => t('is less than'),
      '>' => t('is greater than'),
      '<=' => t('is less than or equal to'),
      '>=' => t('is greater than or equal to'),
      'LIKE' => t('contains'),
      'NOT LIKE' => t('does not contain'),
      'BEGINS WITH' => t('begins with'),
      'ENDS WITH' => t('ends with'),
    );
  }
  return isset($symbol) ? (isset($symbols[$symbol]) ? $symbols[$symbol] : $symbol) : $symbols;
};

/**
 * Translate data values to the user based on field name.
 * @param $field
 * @param $data
 * @return the translated text.
 */
function _advuser_filter_ui_data_display($field, $data) {
  switch ($field) {
    case 'status': {
      $ret = t($data ? 'active' : 'blocked');
    } break;
    case 'user_roles': {
      switch ($data) {
        case NULL: {
          $ret = t('No role assigned');
        } break;
        default: {
          $ret = db_select('role', 'r')
            ->fields('r', array('name'))
            ->condition('r.rid', $data, '=')
            ->execute()
            ->fetchField();
        } break;
      }
    } break;
    default: {
      $ret = $data;
    } break;
  }
  return $ret;
}

// vim:ft=php:sts=2:sw=2:ts=2:et:ai:sta:ff=unix
