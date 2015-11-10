<?php
/**
 * @file
 *   Wysiwyg module's plugin file.
 */

/**
 * Implements hook_wysiwyg_plugin().
 */
function quote_quote_plugin() {
  $plugins = array();

  $plugins['quote'] = array(
    'title' => t('Quote'),
    'vendor url' => 'http://drupal.org/project/quote',
    'icon file' => 'quote.png',
    'icon title' => t('Add selected text between quote tags.'),
    'css path' => NULL,
    'css file' => NULL,
    'settings' => array()
  );

  return $plugins;
}
