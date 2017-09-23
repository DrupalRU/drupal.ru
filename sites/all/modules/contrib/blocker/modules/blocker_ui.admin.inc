<?php

/**
 * Wrapper to add blocker
 */
function blocker_ui_add_form($form, $form_state, $entity_type, $entity_id, $region) {
  $blocker = new stdClass();
  $blocker->entity_type = $entity_type;
  $blocker->entity_id = $entity_id;
  $blocker->theme = variable_get('theme_default', 'bartik');
  $blocker->region = $region;
  return blocker_ui_edit_form($form, $form_state, $blocker);
}

/**
 * Submit handler for blocker_add_form
 */
function blocker_ui_add_form_submit($form, $form_state) {
  $blocker = $form_state['values'];
  $parts = explode(':', $form_state['values']['block']);
  $blocker['module'] = $parts[0];
  $blocker['delta'] = $parts[1];
  drupal_write_record('blocker', $blocker);
}

/**
 * Form for creating and editing blocker
 */
function blocker_ui_edit_form($form, $form_state, $blocker) {
  $is_new = TRUE;
  if (isset($blocker->brid)) {
    $form['brid'] = array(
      '#type' => 'value',
      '#value' => $blocker->brid,
    );
    $is_new = FALSE;
  }
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $blocker->entity_type,
  );
  $form['entity_id'] = array(
    '#type' => 'value',
    '#value' => $blocker->entity_id,
  );
  $form['theme'] = array(
    '#type' => 'value',
    '#value' => $blocker->theme,
  );
  $form['region'] = array(
    '#type' => 'value',
    '#value' => $blocker->region,
  );
  
  $blocks = blocker_blocks();
  $enabled_blocks = array_filter(variable_get('blocker_blocks', array()));
  
  if ($enabled_blocks && !empty($enabled_blocks)) {
    foreach($blocks as $key => $block) {
      if ($key && !isset($enabled_blocks[$key])) {
        unset($blocks[$key]);
      }
    }
  }

  $form['block'] = array(
    '#type' => 'select',
    '#title' => t('Block'),
    '#required' => TRUE,
    '#options' => $blocks,
    '#default_value' => (isset($blocker->module)?$blocker->module . ':' . $blocker->delta:''),
  );
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Block title override'),
    '#maxlength' => 64,
    '#description' => t('Override the default title for the block. Use <em>!placeholder</em> to display no title, or leave blank to use the default block title.', array('!placeholder' => '&lt;none&gt;')),
    '#default_value' => isset($blocker->title) ? $blocker->title : '',
  );
  
  if (module_exists('block_class')) {
    $form['block_class'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['block_class']['css_class'] = array(
      '#type' => 'textfield',
      '#title' => t('CSS class(es)'),
      '#default_value' => isset($blocker->css_class) ? $blocker->css_class : '',
      '#description' => t('Customize the styling of this block by adding CSS classes. Separate multiple classes by spaces.'),
      '#maxlength' => 255,
    );
    $form['block_class']['block_class_check'] = array(
      '#type' => 'checkbox',
      '#title' => t('Append existing CSS class(es)'),
      '#default_value' => isset($blocker->block_class_check) ? $blocker->block_class_check : 0,
      '#description' => t('If this block has existing CSS classes, you can choose to have them appended.'),
    );
  }
  
  if (module_exists('block_row')) {
    $form['row'] = array(
      '#type' => 'select',
      '#title' => t('Row'),
      '#options' => array(
        '' => '',
        'row1' => 'Row 1',
        'row2' => 'Row 2',
        'row3' => 'Row 3',
        'row4' => 'Row 4',
        'row5' => 'Row 5',
      ),
      '#default_value' => (isset($blocker->row)?$blocker->row:''),
      '#description' => t('Select the row for this block.'),
    );

    $form['row_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Row Class(es)'),
      '#default_value' => (isset($blocker->row_class)?$blocker->row_class:""),
      '#description' => t('Enter CSS classes to add to the row classes.'),
      '#maxlength' => 255,
    );    
  }
  
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => (isset($blocker->weight) ? $blocker->weight : 0),
    '#delta' => 20,
    '#description' => t('The heavier items will sink and the lighter items will be positioned nearer the top.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => ($is_new ? t('Place block') : t('Save block')),
  );

  return $form;
}

/**
 * Submit handler for blocker_edit_form
 */
function blocker_ui_edit_form_submit($form, $form_state) {
  $blocker = $form_state['values'];
  $parts = explode(':', $form_state['values']['block']);
  $blocker['module'] = $parts[0];
  $blocker['delta'] = $parts[1];
  drupal_write_record('blocker', $blocker, array('brid'));
}

/**
 * Form to delete blocks.
 * 
 * @param type $form
 * @param type $form_state
 * @param type $blocker
 * @return type
 */
function blocker_ui_block_delete($form, &$form_state, $blocker) {
  $form['brid'] = array('#type' => 'hidden', '#value' => $blocker->brid);
  $entity = array_values(entity_load($blocker->entity_type, array($blocker->entity_id)));
  $question = t('Are you sure you want to remove the block %name from this entity?', array('%name' => $blocker->title));
  $path = '<front>';
  $desc_options = array('@block' => $blocker->module.':'.$blocker->delta, '@region' => $blocker->region, '@title' => $entity[0]->title);
  $description = t('This will remove the <em>@block</em> block in <em>@region</em> from <em>@title</em>.', $desc_options);
  return confirm_form($form, $question, $path, $description, t('Remove'), t('Cancel'));
}

/**
 * Submit handler for deleting blocks.
 * 
 * @param type $form
 * @param type $form_state
 */
function blocker_ui_block_delete_submit($form, &$form_state) {
  db_delete('blocker')
    ->condition('brid', $form_state['values']['brid'])
    ->execute();
}
