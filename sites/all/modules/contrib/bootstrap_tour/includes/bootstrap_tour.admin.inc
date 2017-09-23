<?php
/**
 * @file
 * The file for admin forms and functionality for the bootstrap_tour entity
 */

/**
 * Implements hook_form().
 */
function bootstrap_tour_form($form, &$form_state, $tour = NULL) {
  $form = array();
  drupal_add_css(drupal_get_path('module', 'bootstrap_tour') . '/css/bootstrap_tour_admin.css');

  if (empty($tour)) {
    $tour = new stdClass();
  }
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('A human readable name for this site tour.'),
    '#required' => TRUE,
    '#default_value' => !empty($tour->title) ? $tour->title : '',
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#required' => TRUE,
    '#size' => 50,
    '#maxlength' => 255,
    '#default_value' => !empty($tour->name) ? $tour->name : '',
    '#machine_name' => array(
      'source' => array('title'),
      'exists' => '_bootstrap_tour_name_exists',
    ),
  );
  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Indicate whether or not this tour is enabled or disabled.'),
    '#default_value' => (!empty($tour->enabled) || arg(3) == 'add') ? 1 : 0,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('Used for administrative purposes only.'),
    '#default_value' => !empty($tour->description) ? $tour->description : '',
  );
  $form['roles'] = array(
    '#type' => 'select',
    '#title' => t('Roles'),
    '#description' => t('Which roles can access this tour? Leave blank for all.'),
    '#multiple' => TRUE,
    '#options' => user_roles(),
    '#default_value' => !empty($tour->roles) ? explode(',', $tour->roles) : '',
  );
  $form['autorun'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically run'),
    '#description' => t('Should this tour start automatically the first time a user visits the path of the first step? If this is checked, the first step must have the "Path" field filled out.'),
    '#default_value' => (!empty($tour->autorun) || arg(3) == 'add') ? 1 : 0,
  );
  $form['start_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Start Path'),
    '#description' => t("The path where the tour should start if 'Automatically Run' is checked. The path setting of the first step will be ignored. Enter &lt;front&gt; for the front page."),
    '#default_value' => !empty($tour->start_path) ? $tour->start_path : '',
    '#states' => array(
      'visible' => array(
        'input[name="autorun"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name=autorun]' => array('checked' => TRUE),
      ),
    ),
  );

  field_attach_form('bootstrap_tour', $tour, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Tour'),
    '#weight' => 20,
    '#suffix' => ' ' . l('Cancel', 'admin/structure/tours'),
  );
  return $form;
}


/**
 * Implements hook_form_validate().
 */
function bootstrap_tour_form_validate($form, &$form_state) {
  if ($form_state['values']['autorun'] && empty($form_state['values']['start_path'])) {
    form_set_error('start_path', "You must provide a path if the 'Automatically Run' option is checked.");
  }
}


/**
 * Implements hook_form_submit().
 */
function bootstrap_tour_form_submit($form, &$form_state) {
  $tour = entity_ui_form_submit_build_entity($form, $form_state);
  $tour->roles = implode(',', $form_state['values']['roles']);
  $tour->save();
  drupal_set_message(t('Bootstrap tour @name has been saved.', array('@name' => $tour->name)));
  $form_state['redirect'] = 'admin/structure/tours';
}

/**
/* Function to check if the machine name already exists.
 */
function _bootstrap_tour_name_exists($name, $element, &$form_state) {
  $result = db_query('SELECT bootstrap_tour_id from {bootstrap_tour_tour} WHERE name = :name LIMIT 1', array(':name' => $name));
  return ($result->rowCount()==0 ? FALSE : TRUE);
}
