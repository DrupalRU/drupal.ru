<?php
// $Id$
/**
 * @file
 *
 * The form for the admin/people/advuser page.
 *
 */

// We've broken the form into parts.
require_once 'advuser_filter_ui.inc';
require_once 'advuser_admin_account.inc';

/**
 * Callback form controller.
 *
 * This form is cannot be called by drupal_get_form but returns forms that are.
 * Because it is not loaded by drupal_get_form then $form_state isn't passed
 * to it.
 */
function advuser_admin() {

  $output = array();
  $output['advuser_filter_ui'] = drupal_get_form('advuser_filter_ui');
  $output['advuser_admin_account'] = drupal_get_form('advuser_admin_account');

  return $output;
}

// vim:ft=php:sts=2:sw=2:ts=2:et:ai:sta:ff=unix
