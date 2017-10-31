<?php

/**
 * @file
 * File with API from this module with examples.
 */

/**
 * Implements hook_fz152_info().
 * With this hook, every module can define form_id's with checkbox weights which
 * will be passed to core code and will add checkbox to the form if it match.
 *
 * @return array
 *   Information about module forms and their settings. Array key will be used
 *   as path for settings. /admin/config/system/fz152/[key]. Possible values:
 *   - "title": String with title for tab and page with settings.
 *   - "weight": (optional) Int value with weight of tab with settings.
 *   - "form callback": Function to invoke with all forms which must be altered.
 *     array('form_id' => 'form_*_name', 'weight' => 0). Form id is supporting
 *     for wildcard and weight is used for checkbox position in that form.
 *   - "page callback": (optional) Function to invoke which return page for your
 *     module in main tabs. If you don't need it, leave it empty.
 *   - "page arguments": (optional) Array of arguments which will be passed to
 *     page callback function if provided.
 *
 * @see fz152_fz152_info()
 */
function hook_fz152_info() {
  $info = [
    'my-forms' => [
      'title' => 'My forms settings',
      'weight' => 0,
      'form callback' => 'my_forms_forms',
      'page callback' => 'my_forms_settings',
      'page arguments' => array('arg1', 'arg2'),
    ]
  ];

  return $info;
}

/**
 * Implements hook_fz152_info_alter().
 *
 * With this hook you can modify form_id's and their checkboxes weight, or
 * completely remove some of them from execution order.
 */
function hook_fz152_info_alter(&$forms) {
  foreach ($forms as $k => &$v) {
    if ($v['form_id'] == 'my_*_form') {
      $v['weight'] = 10;
    }
  }
}
