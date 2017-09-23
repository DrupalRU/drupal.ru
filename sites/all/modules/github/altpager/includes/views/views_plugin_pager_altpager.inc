<?php

/**
 * @file
 * The plugin to handle Altpager.
 *
 * @ingroup views_pager_plugins
 */

class views_plugin_pager_altpager extends views_plugin_pager_mini {

  /**
   * Summary title overwrite.
   */
  function summary_title() {
    return t('Without settings.');
  }

  /**
   * Query overwrite.
   */
  function query() {
    // Override query.
  }

  /**
   * Pre execute overwrite.
   */
  function pre_execute(&$query) {
    // Count all entries.
    $max_count = $query
      ->countQuery()
      ->execute()
      ->fetchField();

    // Save data count all entries.
    altpager_count_all_items($max_count);

    // Count viewed.
    $count_items = altpager_count_items_viewed();

    // Apply limit.
    $query->range(0, $count_items);
  }

  /**
   * Options definition overwrite.
   */
  function option_definition() {
    return array();
  }

  /**
   * Options form overwrite.
   */
  function options_form(&$form, &$form_state) {
    $form['empty'] = array(
      '#markup' => t('Altpager not have settings.'),
    );
  }

  /**
   * Options form validate overwrite.
   */
  function options_validate(&$form, &$form_state) {
    // Disable validate.
  }

  /**
   * Render overwrite.
   */
  function render($input) {
    $pager_theme = views_theme_functions('altpager', $this->view, $this->display);

    return theme($pager_theme, array(
      'element' => $this->options,
    ));
  }

}
