<?php
/**
 * @file
 * Provides the views plugin information.
 */

/**
 * Implements hook_views_plugins().
 */
function altpager_views_plugins() {
  return array(
    'module' => 'altpager',
    'pager' => array(
      'altpager' => array(
        'title' => t('Altpager'),
        'help' => t('Views altpager'),
        'help topic' => '',
        'handler' => 'views_plugin_pager_altpager',
        'uses options' => TRUE,
        'parent' => 'mini',
      ),
    ),
  );
}
