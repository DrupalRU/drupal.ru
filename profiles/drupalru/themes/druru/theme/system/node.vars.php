<?php

/**
 * Implements hook_preprocess_node().
 */
function druru_preprocess_node(&$vars) {
  $vars['content_attributes_array']['class'][] = 'content';
  $view_mode = $vars['view_mode'];
  _druru_wrap_terms($vars['content']);

  $date = druru_icon('calendar') . $vars['date'];
  $user = druru_icon('user') . $vars['name'];
  $vars['submitted'] = $date . ', ' . $user;

  if (!$vars['status']) {
    $vars['submitted'] = ' <span class="unpublished-item">';
    $vars['submitted'] .= druru_get_icon_by_title(t('Unpublished'));
    $vars['submitted'] .= '</span>';
  }

  if (isset($vars['content']['datetime']['#markup']) && $view_mode == 'teaser') {
    $event_time = $vars['node']->event->time_from;
    $vars['content']['datetime']['#markup'] = format_date($event_time, 'medium');
    $vars['title_suffix'] = array(
      '#type'   => 'markup',
      '#markup' => format_date($event_time, 'short'),
      '#prefix' => '<small class="event-date">',
      '#suffix' => '</small>',
    );
  }
  if ($vars['view_mode'] == 'frontpage') {
    $vars['name'] = $vars['node']->name;
  }

  $vars['attributes_array']['data-node-id'] = $vars['node']->nid;
}

/**
 * Implements hook_process_node().
 */
function druru_process_node(&$vars) {
  $node = isset($vars['node']) ? $vars['node'] : NULL;
  // If display is page then menu will create in hook_process_page.
  if ($vars['view_mode'] != 'full') {
    if (!empty($node->nid)) {
      $links = menu_contextual_links('node', 'node', array($node->nid));
      $new_links = array();
      _druru_fetch_links($links, $new_links);
      if ($new_links) {
        druru_construct_contextual_menu($new_links, $vars);
      }
    }
  }
  if ('frontpage' == $vars['view_mode']) {
    $vars['time'] = format_interval(time() - $vars['revision_timestamp'], 1);
  }
}

/**
 * @param $elements
 */
function _druru_wrap_terms(&$elements) {
  // List of terms which should be wrapped into "well".
  $list = array(
    'taxonomy_vocabulary_2',
    'taxonomy_vocabulary_7',
    'taxonomy_vocabulary_8',
    'taxonomy_vocabulary_10',
    'taxonomyextra',
    'taxonomy_forums',
  );

  if (array_intersect($list, array_keys($elements))) {
    $elements['terms_wrappepr'] = array(
      '#type'       => 'container',
      '#weight'     => NULL,
      '#attributes' => array(
        'class' => array('terms-wrapper', 'well'),
      ),
    );
    $terms = &$elements['terms_wrappepr'];
    foreach ($list as $term_name) {
      if (isset($elements[$term_name])) {
        if (!isset($terms['#weight'])
          || $elements[$term_name]['#weight'] > $terms['#weight']
        ) {
          $terms['#weight'] = $elements[$term_name]['#weight'];
        }
        $terms[$term_name] = $elements[$term_name];
        unset($elements[$term_name]);
      }
    }
  }
}
