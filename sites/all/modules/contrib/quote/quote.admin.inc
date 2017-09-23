<?php

/**
 * @file
 * Admin page callbacks for the quote module.
 */

/**
 * Menu callback: quote module settings form.
 */
function quote_settings_form() {
  $form = array();
  $form['quote'] = array(
    '#type' => 'fieldset',
    '#title' => t('Quote module settings'),
    '#tree' => TRUE
  );
  $form['quote']['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node associations'),
    '#description' => t('Select the node types to associate with the quote filter.'),
    '#options' => _quote_get_node_types(),
    '#default_value' => _quote_variable_get('node_types')
  );
  $form['quote']['node_link_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the quote link for nodes'),
    '#description' => t('While the quote link is always displayed for comments, it can also be displayed for nodes.'),
    '#default_value' => _quote_variable_get('node_link_display')
  );

  // List all formats regardless of permission issues.
  $formats = filter_formats();
  $options = array('0' => t('None'));
  foreach ($formats as $format) {
    $options[$format->format] = $format->name;
  }

  $form['quote']['format'] = array(
    '#type' => 'select',
    '#title' => t('Text format'),
    '#description' => t('Select the text format that the quote should be filtered through prior to display in a text field.
    This is useful in situations where the raw quote might potentially contain sensitive content/code. It is recommended
    that a dedicated format be used for this purpose containing the appropriate filters.'),
    '#options' => $options,
    '#default_value' => _quote_variable_get('format')
  );

  $form['quote']['nest'] = array(
    '#type' => 'select',
    '#title' => t('Level of nesting'),
    '#description' => t('Recursively nested quotes can lead to unsightly pages. This can be minimised by limiting the
    order of nesting. 0 will display all levels. The default and recommended value is 2.'),
    '#options' => range(0, 10),
    '#default_value' => _quote_variable_get('nest')
  );

  return system_settings_form($form);
}

/**
 * Validate quote settings form submission.
 */
function quote_settings_form_validate($form, &$form_state) {
  // Run the node type checkboxes through array_filter to strip unselected
  // items.
  form_set_value($form['quote']['node_types'], array_filter($form_state['values']['quote']['node_types']), $form_state);
}
