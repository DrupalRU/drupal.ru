<?php

/**
 * Implements hook_form_FORM_ID_alter() for search_block_form().
 */
function druru_form_search_block_form_alter(&$form, &$form_state) {
  $form['#attributes']['class'][] = 'form-search';

  $form['search_block_form']['#title'] = '';
  $form['search_block_form']['#attributes']['placeholder'] = t('Search');

  // Hide the default button from display and implement a theme wrapper
  // to add a submit button containing a search icon directly after the
  // input element.
  $form['actions']['submit']['#attributes']['class'][] = 'element-invisible';
  $form['search_block_form']['#theme_wrappers'] = array('bootstrap_search_form_wrapper');

  // Apply a clearfix so the results don't overflow onto the form.
  $form['#attributes']['class'][] = 'content-search';
}
