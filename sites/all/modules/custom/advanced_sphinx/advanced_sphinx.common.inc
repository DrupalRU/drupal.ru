<?php

/**
 * @file
 * Common functions for the Sphinx search module.
 *
 * WARNING: Implementation is not finished yet, and it is subject to change in
 * future versions with no backwards compatibility. I should probably create a
 * sample module before it gets into something really reusable... and useful.
 */

/**
 * List of supported search matching modes.
 */
define('ADVANCED_SPHINX_MATCH_ALL', 0);
define('ADVANCED_SPHINX_MATCH_ANY', 1);
define('ADVANCED_SPHINX_MATCH_PHRASE', 2);

/**
 * Check if specified path is the Sphinx search page.
 *
 * @param string $path
 *   Path to check. Defaults to $_GET['q'].
 *
 * @return bool
 */
function advanced_sphinx_is_search_path($path = NULL) {
  if (!isset($path)) {
    $path = $_GET['q'];
  }

  return (strpos($path, variable_get('advanced_sphinx_search_path', 'search-content')) === 0);
}

/**
 * Redirect to search page with specified query options.
 *
 * @param mixed $query
 *   This argument accepts a query string array or an already escaped query string.
 *
 * @return array
 */
function advanced_sphinx_goto_search($query = NULL) {
  $url = array(
    variable_get('advanced_sphinx_search_path', 'search-content'),
    array('query' => $query),
  );

  return $url;
}

/**
 * Check if user has exceeded flood limit.
 *
 * This function is aimed to hide implementation details.
 *
 * @see advanced_sphinx_search_page()
 * @see advanced_sphinx_block()
 *
 * @param bool $exceeded
 *   TRUE to enable the flag. This argument is optional.
 *
 * @return bool
 *   TRUE is flood limit has been exceeded.
 */
function advanced_sphinx_flood_limit_exceeded($exceeded = NULL) {
  if (!empty($exceeded)) {
    $GLOBALS['advanced_sphinx_flood_limit_exceeded'] = TRUE;
  }

  return isset($GLOBALS['advanced_sphinx_flood_limit_exceeded']);
}

/**
 * Instantiate a Sphinx search client object.
 *
 * Usage:
 *   $advanced_sphinx = &advanced_sphinx_get_client();
 *
 * @return object
 *   Sphinx client instance.
 */
function &advanced_sphinx_get_client() {
  static $advanced_sphinx;

  if (!isset($advanced_sphinx)) {
    require_once drupal_get_path('module', 'advanced_sphinx') . '/lib/sphinxapi.php';

    $advanced_sphinx = new SphinxClient();
    $advanced_sphinx->SetServer(variable_get('advanced_sphinx_searchd_host', 'localhost'), (int) variable_get('advanced_sphinx_searchd_port', '3312'));
    $advanced_sphinx->SetLimits(0, 30, 500);

    // Setup connection timeout?
    if (($advanced_sphinx_searchd_timeout = (int) variable_get('advanced_sphinx_searchd_timeout', 0)) > 0) {
      $advanced_sphinx->SetConnectTimeout($advanced_sphinx_searchd_timeout);
    }

    // Setup max query time?
    if (($advanced_sphinx_searchd_maxquerytime = (int) variable_get('advanced_sphinx_searchd_maxquerytime', 0)) > 0) {
      $advanced_sphinx->SetMaxQueryTime($advanced_sphinx_searchd_maxquerytime * 1000);
    }

    // Setup distributed retries?
    if (($advanced_sphinx_retries_count = (int) variable_get('advanced_sphinx_retries_count', 0)) > 0) {
      $advanced_sphinx_retries_delay = (int) variable_get('advanced_sphinx_retries_delay', 0);
      $advanced_sphinx->SetRetries($advanced_sphinx_retries_count, $advanced_sphinx_retries_delay * 1000);
    }
  }
  return $advanced_sphinx;
}

/**
 * Check connection with Sphinx searchd daemon.
 *
 * @return bool
 */
function advanced_sphinx_check_connection() {
  $advanced_sphinx = &advanced_sphinx_get_client();
  return $advanced_sphinx->_Connect() ? TRUE : FALSE;
}

/**
 * Build search URL data based on the given search options structure.
 *
 * @param array $search_options
 *   Search options structure.
 *
 * @return string
 *   Encoded query string. NULL indicates no search filter has been specified.
 */
function advanced_sphinx_get_query_string($search_options) {
  $query = array();
  foreach ($search_options as $key => $option) {
    if ($key == 'keys') {
      $query[$key] = $option;
    }
    elseif ($key == 'filters') {
      foreach ($search_options['filters'] as $filter_key => $filter_val) {
        $val = implode(',', $filter_val);
        $query['fil-' . $filter_key] = $val;
      }
    }
    elseif ($key == 'view' || $key == 'group') {
      $query[$key] = $option;
    }
    elseif ($key == 'sorted') {
      foreach ($search_options['sorted'] as $sort_key => $sort_val) {
        $query['sor-' . $sort_key] = $sort_val;
      }
    }
  }

  return $query;
}

/**
 * Parse search request and build search options structure.
 *
 * @param array $request_options
 *   Requested search options.
 *
 * @return array
 *   Search options structure.
 */
function advanced_sphinx_parse_request($request_options = array()) {
  $search_options = array(
    'matchmode' => ADVANCED_SPHINX_MATCH_ALL,
    'results_per_page' => (int) variable_get('advanced_sphinx_results_per_page', 25),
    'excerpts_limit' => (int) variable_get('advanced_sphinx_excerpts_limit', 256),
    'excerpts_around' => (int) variable_get('advanced_sphinx_excerpts_around', 5),
    'excerpts_single_passage' => (int) variable_get('advanced_sphinx_excerpts_single_passage', 0),
    'errors' => array(),
  );

  foreach ($request_options as $key => $value) {
    if ($key == 'keys') {
      $search_options[$key] = preg_replace('#\s+#', ' ', trim($value));
    }
    elseif (strstr($key, 'fil-') != FALSE) {
      $key = str_replace("fil-", "", $key);
      $search_options['filters'][$key] = explode(',', $value);
    }
    elseif ($key == 'view' || $key == 'group') {
      $search_options[$key] = $value;
    }
    elseif (strstr($key, 'sor-') != FALSE) {
      $key = str_replace("sor-", "", $key);
      $search_options['sorted'][$key] = $value;
    }
  }

  return $search_options;
}

/**
 * Execute a search query on the given options.
 *
 * @param array $search_options
 *   Search options structure.
 *
 * @return array
 *   Search results structure.
 */
function advanced_sphinx_execute_query($search_options) {
  $search_results = array(
    'error_message' => '',
    'warnings' => array(),
    'total_found' => 0,
    'total_available' => 0,
    'time' => 0,
    'words' => array(),
    'nodes' => array(),
    'titles' => array(),
    'excerpts' => array(),
    'groups' => array(),
  );

  $sphinx_query_keywords = array($search_options['keys']);

  // Obtain distributed index name, required to resolve search query.
  if (!variable_get('advanced_sphinx_query_index_delta', '')) {
    $advanced_sphinx_query_index = variable_get('advanced_sphinx_query_index', '');
  }
  else {
    $advanced_sphinx_query_index = variable_get('advanced_sphinx_query_index', '') . ',' . variable_get('advanced_sphinx_query_index_delta', '');
  }

  if (empty($advanced_sphinx_query_index)) {
    $search_results['error_message'] = t('Sphinx query index not specified.');
    return $search_results;
  }

  // Obtain excerpts index name, required to build excerpts.
  $advanced_sphinx_excerpts_index = variable_get('advanced_sphinx_excerpts_index', '');
  if (empty($advanced_sphinx_excerpts_index)) {
    $search_results['error_message'] = t('Sphinx excerpts index not specified.');
    return $search_results;
  }

  // Validate results per page option.
  if (!isset($search_options['results_per_page']) || $search_options['results_per_page'] <= 0) {
    $search_options['results_per_page'] = 25;
  }

  // Prepare Sphinx client for search queries.
  $current_page = advanced_sphinx_get_current_page();
  $advanced_sphinx = &advanced_sphinx_get_client();
  $advanced_sphinx->ResetFilters();
  $advanced_sphinx->ResetGroupBy();
  $advanced_sphinx->SetLimits($current_page * $search_options['results_per_page'], $search_options['results_per_page']);
  $advanced_sphinx->SetFieldWeights(array('title' => 20, 'body' => 5));
  // $advanced_sphinx->SetIndexWeights(array('index1'=>20, 'index2'=>5));.
  $sphinx_excerpt_keywords = $sphinx_query_keywords[0];
  $advanced_sphinx->SetMatchMode(SPH_MATCH_EXTENDED2);
  $advanced_sphinx->SetRankingMode(SPH_RANK_MATCHANY);
  // $advanced_sphinx->SetRankingMode(SPH_RANK_PROXIMITY_BM25);
  // $advanced_sphinx->SetSelect('@weight + ( countitl - '.count($search_options['keys']).')*(-0.1) AS new_weight');.
  // Sort options.
  if (isset($search_options['sorted'])) {
    foreach ($search_options['sorted'] as $key => $value) {
      if ($key == 'relevance') {
        $advanced_sphinx->SetSortMode(SPH_SORT_EXPR, '@weight + ( countitl - ' . count($search_options['keys']) . ')*(-0.1)');
      }
      else {
        $advanced_sphinx->SetSortMode(SPH_SORT_EXTENDED, $key . ' ' . $value);
      }
    }
  }
  else {
    $advanced_sphinx->SetSortMode(SPH_SORT_EXPR, '@weight + ( countitl - ' . count($search_options['keys']) . ')*(-0.1)');
  }

  $advanced_sphinx->SetArrayResult(TRUE);

  if (isset($search_options['filters'])) {
    foreach ($search_options['filters'] as $key => $value) {
      $advanced_sphinx->SetFilter($key, $value, FALSE);
    }
  }
  // Grouping options.
  if (isset($search_options['group'])) {
    $advanced_sphinx->SetGroupBy($search_options['group'], SPH_GROUPBY_ATTR, "@group desc");
  }

  // Send query to Sphinx.
  $sphinx_results = $advanced_sphinx->Query(implode(' ', $sphinx_query_keywords), $advanced_sphinx_query_index);

  if (!$sphinx_results) {
    $message = $advanced_sphinx->GetLastError();
    if (!advanced_sphinx_check_connection()) {
      watchdog('advanced_sphinx', $message);
      $search_results['error_message'] = t('Search service is disabled temporarily. Please, try again later.');
      return $search_results;
    }

    $search_results['error_message'] = t('Search failed using index %index. Sphinx error: %message', array(
      '%index' => $advanced_sphinx_query_index,
      '%message' => $message,
    ));

    return $search_results;
  }
  $message = $advanced_sphinx->GetLastWarning();

  if (!empty($message)) {
    $search_results['warnings'][] = t('Search query warning: %message', array(
      '%message' => $message,
    ));
  }

  if (empty($sphinx_results['matches'])) {
    return $search_results;
  }

  // Save Sphinx query results.
  $search_results['total_found'] = (int) $sphinx_results['total_found'];
  $search_results['total_available'] = (int) $sphinx_results['total'];
  $search_results['time'] = $sphinx_results['time'];
  $search_results['words'] = (isset($sphinx_results['words']) && is_array($sphinx_results['words']) ? $sphinx_results['words'] : array());

  // Load nodes referenced by returned results.
  foreach ($sphinx_results['matches'] as $sphinx_match) {
    if (isset($sphinx_match['node_id']) && ($node = advanced_sphinx_get_node_info($sphinx_match['node_id']))) {
      $search_results['nodes'][] = $node;
      $search_results['titles'][] = check_plain($node->title);
      $search_results['excerpts'][] = advanced_sphinx_get_node_text_fast($node);
    }
  }

  // Use Sphinx to build excerpts.
  // Build node titles with highlighted keywords.
  $search_results['titles'] = $advanced_sphinx->BuildExcerpts($search_results['titles'], $advanced_sphinx_excerpts_index, $sphinx_excerpt_keywords, array(
    'before_match' => '<b>',
    'after_match' => '</b>',
    'chunk_separator' => '',
  // We want all text here, so using a high enough number.
    'limit' => 1024,
  // Ignored when single_passage is TRUE.
    'around' => 200,
    'exact_phrase' => ($search_options['matchmode'] == ADVANCED_SPHINX_MATCH_PHRASE),
    'single_passage' => TRUE,
  ));

  if (!$search_results['titles']) {
    $search_results['titles'] = array();
    $search_results['warnings'][] = t('Unable to build excerpts for content titles. Sphinx error: %message', array('%message' => $advanced_sphinx->GetLastError()));
  }

  // Build node excerpts with highlighted keywords.
  $search_results['excerpts'] = $advanced_sphinx->BuildExcerpts($search_results['excerpts'], $advanced_sphinx_excerpts_index, $sphinx_excerpt_keywords, array(
    'before_match' => '<b>',
    'after_match' => '</b>',
    'chunk_separator' => '<span class="search-chunk-separator"> ... </span>',
    'limit' => $search_options['excerpts_limit'],
    'around' => $search_options['excerpts_around'],
    'exact_phrase' => ($search_options['matchmode'] == ADVANCED_SPHINX_MATCH_PHRASE),
    'single_passage' => $search_options['excerpts_single_passage'],
  ));

  if (!$search_results['excerpts']) {
    $search_results['excerpts'] = array();
    $search_results['warnings'][] = t('Unable to build excerpts for content snippets. Sphinx error: %message', array('%message' => $advanced_sphinx->GetLastError()));
  }

  return $search_results;
}

/**
 * Obtain current page from search results navigation.
 *
 * @param int $pager_element
 *   An optional integer to distinguish between multiple pagers on one page.
 *
 * @return int
 */
function advanced_sphinx_get_current_page($pager_element = 0) {
  $pager_page_array = (isset($_GET['page']) ? explode(',', $_GET['page']) : array());
  return (isset($pager_page_array[$pager_element]) ? (int) $pager_page_array[$pager_element] : 0);
}

/**
 * Compute pager options and invoke theme pager.
 *
 * @param int $total_results
 *   The total number of returned search results.
 *
 * @param int $results_per_page
 *   The number of query results to display per page.
 *
 * @param int $pager_element
 *   An optional integer to distinguish between multiple pagers on one page.
 *
 * @return string
 */
function advanced_sphinx_pager($total_results, $results_per_page, $pager_element = 0) {
  $page = isset($_GET['page']) ? (int) $_GET['page'] : NULL;

  $GLOBALS['pager_page_array'] = explode(',', $page);
  $GLOBALS['pager_total_items'][$pager_element] = $total_results;
  $GLOBALS['pager_total'][$pager_element] = ceil($total_results / $results_per_page);
  $GLOBALS['pager_page_array'][$pager_element] = max(0, min((int) $GLOBALS['pager_page_array'][$pager_element], ((int) $GLOBALS['pager_total'][$pager_element]) - 1));

  return theme('pager');
}

/**
 * Delete sort parameters from query.
 */
function advanced_sphinx_query_delete_sorts($query, $delete) {
  foreach ($query as $key => $value) {
    if (in_array($key, $delete)) {
      unset($query[$key]);
    }
  }

  return $query;
}

/**
 * Get current sort parameter.
 */
function advanced_sphinx_get_current_sort($query) {
  $result = array();
  foreach ($query as $key => $value) {
    if (strstr($key, 'sor-') != FALSE) {
      $result[$key] = $value;
    }
  }

  if (empty($result)) {
    $result['sor-relevance'] = 'DESC';
  }

  return $result;
}
