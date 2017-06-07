<?php

/**
 * Implements hook_form_FORM_ID_alter() for search_form().
 */
function druru_form_search_form_alter(&$form, &$form_state) {
  // Add a clearfix class so the results don't overflow onto the form.
  $form['#attributes']['class'][] = 'clearfix';

  // Remove container-inline from the container classes.
  $form['basic']['#attributes']['class'] = array();

  // Hide the default button from display.
  $form['basic']['submit']['#attributes']['class'][] = 'element-invisible';

  // Implement a theme wrapper to add a submit button containing a search
  // icon directly after the input element.
  $form['basic']['keys']['#theme_wrappers'] = array('bootstrap_search_form_wrapper');
  $form['basic']['keys']['#title'] = '';
  $form['basic']['keys']['#attributes']['placeholder'] = t('Search');
}

/**
 * Implements hook_form_FORM_ID_alter() for search_form().
 */
function druru_form_advanced_sphinx_search_box_alter(&$form, &$form_state) {
  // Add a clearfix class so the results don't overflow onto the form.
  $form['#attributes']['class'][] = 'clearfix';

  $form['inline']['#prefix'] = NULL;
  $form['inline']['#suffix'] = NULL;
  // Remove container-inline from the container classes.
  $form['inline']['#attributes']['class'] = array();

  // Hide the default button from display.
  $form['inline']['submit']['#attributes']['class'][] = 'element-invisible';

  // Implement a theme wrapper to add a submit button containing a search
  // icon directly after the input element.
  $form['inline']['keys']['#theme_wrappers'] = array('bootstrap_search_form_wrapper');
  $form['inline']['keys']['#title'] = ' ';

  $form['inline']['keys']['#attributes']['placeholder'] = t('Search');
}

/**
 * Implements hook_form_FORM_ID_alter() for search_form().
 */
function druru_form_advanced_sphinx_search_form_alter(&$form, &$form_state) {
//  drupal_set_title(t('Search'));
  $form['basic']['#prefix'] = '<span class="input-group">';
  $form['basic']['#suffix'] = '</span>';
  $form['basic']['#theme_wrappers'] = array();

  $form['basic']['inline']['#prefix'] = NULL;
  $form['basic']['inline']['#suffix'] = NULL;

  $form['basic']['inline']['keys']['#attributes']['placeholder'] = t('Search');
  $form['basic']['inline']['keys']['#theme_wrappers'] = array();

  // Hide the default button from display.
  $form['basic']['inline']['submit']['#value'] = druru_icon('search');
  $form['basic']['inline']['submit']['#attributes']['class'][] = 'btn-primary';
  $form['basic']['inline']['submit']['#prefix'] = '<span class="input-group-btn">';
  $form['basic']['inline']['submit']['#suffix'] = '</span>';
}
