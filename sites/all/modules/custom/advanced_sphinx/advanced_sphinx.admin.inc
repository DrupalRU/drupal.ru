<?php

/**
 * @file
 * Administration interface for the advanced_sphinx module.
 */

/**
 * Module settings form.
 */
function advanced_sphinx_settings() {
  $form = array();

  if (advanced_sphinx_get_memory_limit() < 0) {
    drupal_set_message(t('Function <em>memory_get_usage</em> not found. Please, make sure your PHP installation is compiled with --enable-memory-limit option.'), 'error');
  }

  $form['server'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sphinx server and config settings'),
    '#collapsible' => FALSE,
    '#description' => t('Use this section to setup options related to your Sphinx server installation.'),
  );

  $form['server']['advanced_sphinx_generate_config'] = array(
    '#type' => 'checkbox',
    '#title' => t('Generate configuration file'),
    '#default_value' => variable_get('advanced_sphinx_generate_config', FALSE),
  );

  $form['server']['advanced_sphinx_path_config'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to confige file'),
    '#default_value' => variable_get('advanced_sphinx_path_config', conf_path() . '/sphinx'),
    '#description' => t('Default "sites/default/sphinx".'),
    '#required' => TRUE,
  );

  $form['server']['advanced_sphinx_connections_type'] = array(
    '#type' => 'radios',
    '#title' => t('Type of connection to Sphinx daemon'),
    '#default_value' => variable_get('advanced_sphinx_connections_type', 'socket'),
    '#required' => TRUE,
    '#options' => array(
      'socket' => t('Socket'),
      'port' => t('Port'),
    ),
  );

  $form['server']['advanced_sphinx_searchd_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Sphinx searchd host name or path to socket'),
    '#default_value' => variable_get('advanced_sphinx_searchd_host', realpath(conf_path()) . '/sphinx/sphinx.s'),
    '#description' => t('Host name where your <strong>Sphinx searchd daemon</strong> is running, i.e. <code>localhost</code>, <code>sphinx.example.com</code> or <code>192.168.0.1</code>.'),
    '#required' => TRUE,
  );

  $form['server']['advanced_sphinx_searchd_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Sphinx searchd port'),
    '#default_value' => variable_get('advanced_sphinx_searchd_port', '3312'),
    '#description' => t('Port on which your <strong>Sphinx searchd daemon</strong> listens. Default Sphinx port is 3312.'),
    '#required' => TRUE,
  );

  $form['server']['advanced_sphinx_searchd_timeout'] = array(
    '#type' => 'select',
    '#title' => t('Sphinx searchd connection timeout'),
    '#default_value' => variable_get('advanced_sphinx_searchd_timeout', 6),
    '#options' => range(0, 20),
    '#description' => t('Timeout in seconds used for connections to your <strong>Sphinx searchd daemon</strong>. Leave 0 for PHP defaults for <a href="@fsockopen" target="_blank">fsockopen()</a>.', array('@fsockopen' => 'http://www.php.net/fsockopen')),
  );

  $form['index'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sphinx index settings'),
    '#collapsible' => FALSE,
    '#description' => t('Use this section to setup options related to your Sphinx indexes. Note that there a few options that will require you to rebuild your Sphinx indexes if you change them.'),
  );

  $form['index']['advanced_sphinx_query_index'] = array(
    '#type' => 'textfield',
    '#title' => t('Index name for queries'),
    '#default_value' => variable_get('advanced_sphinx_query_index', 'index_main'),
    '#description' => t('Name of the Sphinx index used for queries. If you have more than one index (ie. main+delta), you may configure Sphinx with a distributed index that joins all of them (local and/or remote) to provide a single interface to resolve queries.'),
  );

  $form['index']['advanced_sphinx_query_index_delta'] = array(
    '#type' => 'textfield',
    '#title' => t('Index name for queries delta'),
    '#default_value' => variable_get('advanced_sphinx_query_index_delta', 'index_delta'),
    '#description' => t('Name of the Sphinx index used for queries delta.'),
  );

  $form['index']['advanced_sphinx_excerpts_index'] = array(
    '#type' => 'textfield',
    '#title' => t('Index name for building excerpts'),
    '#default_value' => variable_get('advanced_sphinx_excerpts_index', 'index_main'),
    '#description' => t('Regardless of the index used for queries, Sphinx needs an index with tokenizing settings defined to build excerpts. Sphinx may be unable to build excertps from distributed indexes. It is recommended to use the name of your first main index for this purpose.'),
  );

  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search page settings'),
    '#collapsible' => FALSE,
    '#description' => t('Use these options to customize the behaviour of your search page.'),
  );

  $search_path_description = t('Use this option to setup the path for the main search page.');

  if (module_exists('search')) {
    $search_path_description .= ' ' . t("Note that Drupal search module is installed. You cannot take over the 'search' path.");
  }
  else {
    $search_path_description .= ' ' . t("Note that Drupal search module is not installed. You can take over the 'search' path.");
  }

  $form['search']['advanced_sphinx_search_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Search path'),
    '#default_value' => variable_get('advanced_sphinx_search_path', 'search-content'),
    '#description' => $search_path_description,
    '#required' => TRUE,
  );

  $form['search']['advanced_sphinx_results_per_page'] = array(
    '#type' => 'select',
    '#title' => t('Results per page'),
    '#default_value' => variable_get('advanced_sphinx_results_per_page', 25),
    '#options' => drupal_map_assoc(array(
      5,
      10,
      15,
      20,
      25,
      30,
      35,
      40,
      45,
      50,
      60,
      70,
      80,
      90,
      100,
    )),
    '#description' => t('How many items will be shown on search results page.'),
  );

  $form['server']['advanced_sphinx_logs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable log for search requests'),
    '#default_value' => variable_get('advanced_sphinx_logs', FALSE),
  );

  $form['search']['excerpts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Excerpts builder settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Use these options to customize how to build excerpts for search results.'),
  );

  $form['search']['excerpts']['advanced_sphinx_excerpts_limit'] = array(
    '#type' => 'select',
    '#title' => t('Excerpts size limit'),
    '#default_value' => variable_get('advanced_sphinx_excerpts_limit', 256),
    '#options' => drupal_map_assoc(array(
      128,
      192,
      256,
      320,
      384,
      448,
      512,
      640,
      768,
      896,
      1024,
    )),
    '#description' => t('Maximum snippet size (in Sphinx symbols) used to build content excerpts for search results.'),
  );

  $form['search']['excerpts']['advanced_sphinx_excerpts_around'] = array(
    '#type' => 'select',
    '#title' => t('Words around matches'),
    '#default_value' => variable_get('advanced_sphinx_excerpts_around', 5),
    '#options' => range(1, 10),
    '#description' => t('How much words to pick around each matching keywords block when building excerpts. Note that the actual number of words around matches also depends on Sphinx index configuration options such as <code>min_word_len</code>, <code>charset_table</code>, etc.'),
  );

  $form['search']['excerpts']['advanced_sphinx_excerpts_single_passage'] = array(
    '#type' => 'radios',
    '#title' => t('Single passage'),
    '#default_value' => variable_get('advanced_sphinx_excerpts_single_passage', 0),
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
    '#description' => t('Whether to extract single best passage only. When this option is enabled, <em>Words around matches</em> setting is ignored and excerpts are build with a single passage up to <em>Excerpts size limit</em> long.'),
  );

  // Let default settings form do its own job, and then
  // inject our own submit handler.
  $form = system_settings_form($form);
  $form['#submit'][] = 'advanced_sphinx_settings_submit';
  return $form;
}

/**
 * Validate module settings form.
 */
function advanced_sphinx_settings_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['advanced_sphinx_searchd_port']) || (int) $form_state['values']['advanced_sphinx_searchd_port'] < 0) {
    form_set_error('advanced_sphinx_searchd_port', t('Sphinx port must be a numeric positive value.'));
  }

  if (strpos($form_state['values']['advanced_sphinx_search_path'], ' ') !== FALSE) {
    form_set_error('advanced_sphinx_search_path', t('Search path cannot contain whitespaces.'));
  }
  elseif ($form_state['values']['advanced_sphinx_search_path'] == 'search' && module_exists('search')) {
    form_set_error('advanced_sphinx_search_path', t("You should first uninstall the Drupal search module to be able to take over the <code>'search'</code> path."));
  }
}

/**
 * Submit handler for module settings form.
 */
function advanced_sphinx_settings_submit($form, &$form_state) {
  advanced_sphinx_create_config_dir();

  // Rebuild menus when search page path has been changed.
  if ($form_state['values']['advanced_sphinx_search_path'] != $form['search']['advanced_sphinx_search_path']['#default_value']) {
    menu_rebuild();
  }
}

/**
 * Menu callback; Diagnose Sphinx searchd daemon connection.
 */
function advanced_sphinx_check_connection_page() {
  global $databases;

  $steps = array();

  $advanced_sphinx = &advanced_sphinx_get_client();

  if (!variable_get('advanced_sphinx_query_index_delta', '')) {
    $advanced_sphinx_query_index = variable_get('advanced_sphinx_query_index', '');
  }
  else {
    $advanced_sphinx_query_index = variable_get('advanced_sphinx_query_index', '') . ',' . variable_get('advanced_sphinx_query_index_delta', '');
  }

  $advanced_sphinx_excerpts_index = variable_get('advanced_sphinx_excerpts_index', '');

  // Check connection.
  $steps['connection'] = array(
    'title' => t('Testing Sphinx searchd daemon connection.'),
    'messages' => array(),
  );

  if (advanced_sphinx_check_connection()) {
    $steps['connection']['messages'][] = t('OK') . ' - ' . t('Your Sphinx searchd daemon is up and running.');
  }
  else {
    $steps['connection']['messages'][] = t('ERROR') . ' - ' . t('Unable to connect to your Sphinx searchd daemon.') . theme('item_list', array(
      'items' => array(
        t('searchd host: %host', array('%host' => variable_get('advanced_sphinx_searchd_host', 'localhost'))),
        t('searchd port: %port', array('%port' => (int) variable_get('advanced_sphinx_searchd_port', '3306'))),
        t('Sphinx message: %message', array('%message' => $advanced_sphinx->GetLastError())),
      ),
    ));
  }

  // Check distributed index.
  $steps['query'] = array(
    'title' => t('Testing search query using index %index.', array('%index' => $advanced_sphinx_query_index)),
    'messages' => array(
      t('Query asks for a list of all indexed documents to retrieve the last indexed node.'),
      t('MySQL equivalent would look something like:<br /><code>SELECT nid FROM {node} ORDER BY nid DESC LIMIT 1;</code>'),
    ),
  );

  if (empty($advanced_sphinx_query_index)) {
    $steps['query']['messages'][] = t('ERROR') . ' - ' . t('Sphinx query index not specified. Please, check module settings to match your Sphinx server configuration.');
  }
  else {
    $advanced_sphinx->SetLimits(0, 1);
    $advanced_sphinx->SetSortMode(SPH_SORT_EXTENDED, 'node_id DESC');
    $sphinx_results = $advanced_sphinx->Query('', $advanced_sphinx_query_index);
    if (!$sphinx_results) {
      $steps['query']['messages'][] = t('ERROR') . ' - ' . t('Search query failed.') . theme('item_list', array(
        'items' => array(t('Sphinx message: %message', array('%message' => $advanced_sphinx->GetLastError()))),
      ));
    }
    else {
      if (!empty($sphinx_results['matches'])) {
        $sphinx_match = array_shift($sphinx_results['matches']);
        $nid = $sphinx_match['node_id'];
        $updated = '(' . format_date($sphinx_match['changed'], 'custom', 'd.m.Y H:i:s') . ')';
      }
      else {
        $nid = t('N/A - Oops! It apears that your Sphinx indexes are empty.');
        $updated = '';
      }

      $steps['query']['messages'][] = t('OK') . ' - ' . t('Test passed successfully. Sphinx results summary follow:') . theme('item_list', array(
        'items' => array(
          t('Query execution time: @time seconds', array('@time' => $sphinx_results['time'])),
          t('Total results found: @total', array('@total' => $sphinx_results['total_found'])),
          t('Total results available: @total', array('@total' => $sphinx_results['total'])),
          t('Last indexed node: @nid @updated', array(
            '@nid' => $nid,
            '@updated' => $updated,
          )),
        ),
      ));
    }

    $warning = $advanced_sphinx->GetLastWarning();

    if (!empty($warning)) {
      $steps['query']['messages'][] = t('WARNING') . ' - ' . t('There was a problem while trying to resolve the query.') . theme('item_list', array(
        'items' => array(t('Sphinx message: %message', array('%message' => $warning))),
      ));
    }
  }

  // Check excerpts index.
  $steps['excerpts'] = array(
    'title' => t('Testing excerpts builder using index %index.', array('%index' => $advanced_sphinx_excerpts_index)),
    'messages' => array(),
  );

  if (empty($advanced_sphinx_excerpts_index)) {
    $steps['excerpts']['messages'][] = t('ERROR') . ' - ' . t('Sphinx excerpts index not specified. Please, check module settings to match your Sphinx server configuration.');
  }
  else {
    $excerpt_samples = array(t('The quick brown fox jumps over a lazy dog.'));
    $excerpt_keywords = t('brown lazy');
    $excerpt_results = $advanced_sphinx->BuildExcerpts($excerpt_samples, $advanced_sphinx_excerpts_index, $excerpt_keywords);

    if (!$excerpt_results) {
      $steps['excerpts']['messages'][] = t('ERROR') . ' - ' . t('Unable to build excerpts.') . theme('item_list', array(
        'items' => array(t('Sphinx message: %message', array('%message' => $advanced_sphinx->GetLastError()))),
      ));
    }
    else {
      $steps['excerpts']['messages'][] = t('OK') . ' - ' . t('Test passed successfully. Sphinx results summary follow:') . theme('item_list', array(
        'items' => array(
          t('Sample text: <code>@sample</code>', array('@sample' => $excerpt_samples[0])),
          t('Keywords: <code>@keywords</code>', array('@keywords' => $excerpt_keywords)),
          t('Result: <code>@result</code>', array('@result' => $excerpt_results[0])),
        ),
      ));
    }
  }

  $prefix = $databases['default']['default']['prefix'];

  $steps['view'] = array(
    'title' => t('View in the database'),
    'messages' => array(),
  );

  $view_name = $prefix . 'sphinxmain';
  $exist_view = db_table_exists($view_name);

  $steps['view']['messages'][] = $exist_view ? (t('OK') . ' - ' . t('View "@name" exist.', array('@name' => $view_name))) : (t('ERROR') . ' - ' . t('View "@name" does not exist.', array('@name' => $view_name)));

  // Format HTML report.
  $items = array();
  foreach ($steps as $step) {
    $items[] = theme('item_list', array(
      'items' => $step['messages'],
      'title' => $step['title'],
    ));
  }

  return theme('item_list', array('items' => $items));
}
