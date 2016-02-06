<?php

/**
 * @file
 * Implementation of pages used to search using Sphinx search module.
 */

/**
 * Menu callback; presents the search form and/or search results.
 *
 * Search form submits with POST but redirects to GET. This way we can keep
 * the search query URL clean as a whistle:
 * http://www.example.com/search-path?keys=... (including keys and options)
 * @see advanced_sphinx_get_query_string()
 * group=atribute fields for grouping
 * fil-atribute=znach,znach2,znach+n - fields for filter
 * sor-atribute=asc - sort
 * sor-atribute2=desc - sort
 * keys=keyword - key-words
 * view=v - view without keys
 */
function advanced_sphinx_search_page() {
  // Parse request and build search options structure.
  $search_options = advanced_sphinx_parse_request($_GET);

  // Set keys only if sphinx log enable(dirty-way).
  if (variable_get('advanced_sphinx_logs', FALSE)) {
    variable_set('advanced_sphinx_keys', isset($_GET['keys']) ? $_GET['keys'] : NULL);
  }

  // Execute search query and collect the results.
  if (isset($search_options['keys']) || (!isset($search_options['keys']) && isset($search_options['view']))) {
    $search_results = advanced_sphinx_execute_query($search_options);
    $output = array(
      '#theme' => 'advanced_sphinx_search_results',
      '#search_options' => $search_options,
      '#search_results' => $search_results,
    );
  }
  else {
    $output = drupal_get_form('advanced_sphinx_search_form', $search_options);
  }

  return $output;
}

/**
 * Render a search form.
 *
 * @param array $search_options
 *   The search string and options entered by the user.
 *
 * @param bool $advanced_options_collapsed
 *   TRUE to collapse Advanced search options fieldset.
 *
 * @return array
 *   The search form.
 */
function advanced_sphinx_search_form($form, &$form_state, $search_options = array(), $advanced_options_collapsed = TRUE) {
  $form = array(
    '#action' => url(variable_get('advanced_sphinx_search_path', 'search-content')),
    '#attributes' => array(
      'class' => 'search-form',
    ),
  );

  $form['basic'] = array(
    '#type' => 'item',
    '#title' => t('Enter your keywords'),
  );

  $form['basic']['inline'] = array(
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $form['basic']['inline']['keys'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => isset($search_options['keys']) ? $search_options['keys'] : '',
    '#size' => 50,
    '#maxlength' => 255,
  );

  $form['basic']['inline']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * Validate a search form submission.
 */
function advanced_sphinx_search_form_validate($form, &$form_state) {
  if (empty($form_state['values']['keys'])) {
    form_set_error('keys', t('Enter search request in search field!'));
  }
}

/**
 * Process a search form submission.
 */
function advanced_sphinx_search_form_submit($form, &$form_state) {
  $search_options = advanced_sphinx_parse_request($form_state['values']);
  $query = advanced_sphinx_get_query_string($search_options);

  if (empty($query)) {
    form_set_error('keys', t('Please enter some keywords and/or other search options.'));
  }

  // Transform POST into a GET request.
  $form_state['redirect'] = advanced_sphinx_goto_search($query);
}

/**
 * Implements theme_THEMEID().
 */
function theme_advanced_sphinx_search_results($variables) {
  // Display information about query execution.
  $output = '';

  // Params.
  $search_results = $variables['search_results'];
  $search_options = $variables['search_options'];

  // Display list of formatted search results.
  if ($search_results['error_message']) {
    drupal_set_message($search_results['error_message'], 'error');
  }

  if ($search_results['warnings']) {
    drupal_set_message($search_results['warnings'], 'warnings');
  }

  if (!isset($result_wrapper['result'])) {
    $result_wrapper = advanced_sphinx_get_items_result_main($search_options, $search_results);
  }

  $result_wrapper['stat'] = t('Found results: @count on @seconds seconds.', array(
      '@count' => $search_results['total_found'],
      '@seconds' => round($search_results['time'], 3),
    )
  );

  if (!empty($search_results['words'])) {
    $words = array();
    foreach ($search_results['words'] as $word => $word_data) {
      $words[] = '<em>' . check_plain($word) . '</em> (' . t('results: @docs, repeats: @hits', array(
          '@docs' => (int) $word_data['docs'],
          '@hits' => (int) $word_data['hits'],
        )) . ')';
    }

    $result_wrapper['word_stat'] = implode('; ', $words) . '.';
    $result_wrapper['sorted'] = theme('advanced_sphinx_sorted_links', array(
      'query' => advanced_sphinx_get_query_string($search_options),
    ));
  }

  $result_wrapper['search_form'] = drupal_get_form('advanced_sphinx_search_form', $search_options);
  $output .= theme('advanced_sphinx_result_wrapper', array('result' => $result_wrapper));

  // Display pager.
  $output .= advanced_sphinx_pager($search_results['total_available'], $search_options['results_per_page']);
  return $output;
}

function theme_advanced_sphinx_sorted_links($variables) {
  $query = $variables['query'];

  $sorts = array(
    'sor-relevance' => t('Relevance'),
    'sor-created' => t('Date'),
    'sor-changed' => t('Update'),
  );

  $url = array();
  $search_path = variable_get('advanced_sphinx_search_path', 'search-content');

  // Get current sort in query.
  $current_sort = advanced_sphinx_get_current_sort($query);
  // Delete sort-parameters from query.
  $query = advanced_sphinx_query_delete_sorts($query, array_keys($sorts));

  foreach ($sorts as $key => $value) {
    $class = '';

    if (in_array($key, array_keys($current_sort))) {
      $class = 'sortnow';
    }

    $url[$key] = array(
      'title' => $value,
      'href' => $search_path,
      'attributes' => array('id' => $key, 'class' => array($class)),
      'query' => array_merge($query, array($key => 'DESC')),
    );
  }

  return theme('links', array('links' => $url));
}

function advanced_sphinx_get_items_result_main($search_options, $search_results) {
  $items_result = array();
  drupal_set_message('<pre>' . print_r($search_results, true). '</pre>');
  $result_wrapper = array('result' => '');
  $page = isset($_GET['page']) ? (int) $_GET['page'] : 0;

  foreach ($search_results['nodes'] as $item_id => $node) {
    $items_result['number'] = $item_id + 1 + $page * $search_options['results_per_page'];
    $title = (isset($search_results['titles'][$item_id]) ? $search_results['titles'][$item_id] : check_plain($node->title));

    if (variable_get('advanced_sphinx_logs', FALSE)) {
      $items_result['title'] = l($title, variable_get('advanced_sphinx_search_path', 'search-content') . '/service/' . $node->nid, array('html' => TRUE));
    }
    else {
      $items_result['title'] = l($title, 'node/' . $node->nid, array('html' => TRUE));
    }

    $items_result['excerpts'] = isset($search_results['excerpts'][$item_id]) ? $search_results['excerpts'][$item_id] : '';
    $items_result['username'] = theme('username', array('account' => $node));
    $items_result['date'] = format_date($node->created, 'short');

    if (property_exists($node, 'comment_count')) {
      $items_result['comment_count'] = $node->comment_count;
    }

    $result_wrapper['result'] .= theme('advanced_sphinx_items_result_main', array('result' => $items_result));
  }

  return $result_wrapper;
}
