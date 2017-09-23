<?php

/**
 * @file
 * Admin page callback file for the Login Destination module.
 */

/**
 * Form for editing an entire login destination at once.
 *
 * Shows list of all login destination.
 */
function login_destination_overview_form($form, &$form_state) {
  // Get all login destination rules from the database.
  $result = db_select('login_destination', 'l')
    ->fields('l', array(
      'id',
      'triggers',
      'roles',
      'pages_type',
      'pages',
      'destination',
      'weight',
      'enabled',
    ))
    ->orderBy('weight')
    ->execute()
    ->fetchAll();

  $form['#tree'] = TRUE;

  // Loop through the categories and add them to the table.
  foreach ($result as $data) {
    $triggers = array_map('check_plain', unserialize($data->triggers));
    if (empty($triggers)) {
      $triggers = array();
    }

    $roles = array_map('check_plain', unserialize($data->roles));
    if (empty($roles)) {
      $roles = array();
    }

    $form[$data->id]['destination']['#markup'] = theme('login_destination_destination', array('destination' => $data->destination));
    $form[$data->id]['triggers']['#markup'] = theme('login_destination_triggers', array('items' => $triggers));
    $form[$data->id]['pages']['#markup'] = theme('login_destination_pages', array(
      'pages' => $data->pages,
      'pages_type' => $data->pages_type,
    ));
    $form[$data->id]['roles']['#markup'] = theme('login_destination_roles', array('items' => $roles));

    $form[$data->id]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#delta' => 50,
      '#default_value' => $data->weight,
      '#title_display' => 'invisible',
    );

    $form[$data->id]['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => $data->enabled,
      '#title_display' => 'invisible',
    );

    // Build a list of operations.
    $operations = array();
    $operations['edit'] = array(
      '#type' => 'link',
      '#title' => t('edit'),
      '#href' => 'admin/config/people/login-destination/edit/' . $data->id,
    );

    $operations['delete'] = array(
      '#type' => 'link',
      '#title' => t('delete'),
      '#href' => 'admin/config/people/login-destination/delete/' . $data->id,
    );
    $form[$data->id]['operations'] = $operations;
  }

  if (element_children($form)) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $form['#empty_text'] = t('There is no Login Destination Rule.');
  }

  return $form;
}

/**
 * Returns HTML for a login destination list.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 *
 * @return string
 */
function theme_login_destination_overview_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('login-destination-overview', 'order', 'sibling', 'login-destination-weight');

  $header = array(
    t('Destination'),
    t('Triggers'),
    t('Pages'),
    t('Roles'),
    array('data' => t('Enabled'), 'class' => array('checkbox')),
    t('Weight'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );

  $rows = array();
  foreach (element_children($form) as $ldid) {
    if (!isset($form[$ldid]['enabled'])) {
      continue;
    }
    $element = &$form[$ldid];
    $operations = array();

    foreach (element_children($element['operations']) as $op) {
      $operations[] = array(
        'data' => drupal_render($element['operations'][$op]),
        'class' => array('login-destination-operations'),
      );
    }
    while (count($operations) < 2) {
      $operations[] = '';
    }

    $row = array();
    $row[] = drupal_render($element['destination']);
    $row[] = drupal_render($element['triggers']);
    $row[] = drupal_render($element['pages']);
    $row[] = drupal_render($element['roles']);
    $row[] = array(
      'data' => drupal_render($element['enabled']),
      'class' => array(
        'checkbox',
        'login-destination-enabled',
      ),
    );

    $form[$ldid]['weight']['#attributes']['class'] = array('login-destination-weight');

    $row[] = drupal_render($element['weight']);
    $row = array_merge($row, $operations);
    $row = array_merge(array('data' => $row), array());
    $row['class'][] = 'draggable';
    $rows[] = $row;
  }

  $output = '';
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => $form['#empty_text'],
        'colspan' => '7',
      ),
    );
  }

  $table_arguments = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'login-destination-overview',
    ),
  );

  $output .= theme('table', $table_arguments);
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Submit handler for the login destination overview form.
 *
 * This function update the login destination rule attribute
 * like rules are enabled/disabled or its weight.
 *
 * @see login_destination_overview_form()
 */
function login_destination_overview_form_submit($form, &$form_state) {
  $element = &$form_state['values'];
  foreach (element_children($element) as $ldid) {
    if (isset($form[$ldid]['enabled'])) {
      $login_destination_rules[$ldid] = $element[$ldid];
      $login_destination_rules[$ldid]['ldid'] = $ldid;
    }
  }

  foreach ($login_destination_rules as $ldid => $login_destination_rule) {
    _login_destination_update_rules($login_destination_rule);
  }

  drupal_set_message(t('Your configuration has been saved.'), 'status');
}

/**
 * Save all our changed items to the database.
 *
 * @param array $login_destination_rule
 *   An associative array representing a login destination item:
 *   - enabled: (required) can contain 0 or 1, if rule is enabled then
 *   it should be 1 else 0.
 *   - weight: (required) can contain any integer value.
 *
 * @return bool
 *   The ldid of the saved login destination rule, or FALSE
 *   if the login destination rule could not be saved.
 */
function _login_destination_update_rules($login_destination_rule) {

  if (!(isset($login_destination_rule['enabled']) &&
    isset($login_destination_rule['weight']) &&
    isset($login_destination_rule['ldid']))
  ) {
    return FALSE;
  }

  if ($login_destination_rule['enabled'] != 0 &&
    $login_destination_rule['enabled'] != 1
  ) {
    return FALSE;
  }

  $login_destination_rule['weight'] = (int) $login_destination_rule['weight'];

  if (!is_int($login_destination_rule['weight'])) {
    return FALSE;
  }

  db_update('login_destination')
    ->fields(array(
      'enabled' => $login_destination_rule['enabled'],
      'weight' => $login_destination_rule['weight'],
    ))
    ->condition('id', $login_destination_rule['ldid'])
    ->execute();

  return $login_destination_rule['ldid'];
}

/**
 * Render a destination of login destination rule.
 */
function theme_login_destination_destination($variables) {
  $output = nl2br(check_plain($variables['destination']));

  if (empty($output)) {
    $output = '<i>' . t('Empty') . '</i>';
  }

  return $output;
}

/**
 * Render a trigger of login destination rule.
 */
function theme_login_destination_triggers($variables) {
  $items = array_map('check_plain', $variables['items']);

  if (empty($items)) {
    return '<i>' . t('All triggers') . '</i>';
  }

  $output = '';
  foreach ($items as &$item) {
    switch ($item) {
      case 'login':
        $item = t('Login');
        break;

      case 'logout':
        $item = t('Logout');
        break;
    }
    $output .= $item . "<br/>";
  }

  return $output;
}

/**
 * Render a page type of login destination rule.
 */
function theme_login_destination_pages($variables) {
  $type = $variables['pages_type'];

  if ($type == LOGIN_DESTINATION_REDIRECT_PHP) {
    return nl2br(check_plain($variables['pages']));
  }

  $pages = trim($variables['pages']);

  if (empty($pages)) {
    if ($type == LOGIN_DESTINATION_REDIRECT_NOTLISTED) {
      return '<i>' . t('All pages') . '</i>';
    }
    else {
      return '<i>' . t('No pages') . '</i>';
    }
  }

  $pages = explode("\n", preg_replace('/\r/', '', check_plain($variables['pages'])));

  $output = '';
  foreach ($pages as &$page) {
    if ($type == LOGIN_DESTINATION_REDIRECT_NOTLISTED) {
      $output .= "~ ";
    }
    $output .= $page . "<br/>";
  }

  return $output;
}

/**
 * Render a roles of login destination rule.
 */
function theme_login_destination_roles($variables) {
  $items = array_values(array_intersect_key(_login_destination_role_options(), $variables['items']));

  if (empty($items)) {
    return '<i>' . t('All roles') . '</i>';
  }

  return theme('item_list', array('items' => $items));
}

/**
 * Category edit page.
 */
function login_destination_edit_form($form, &$form_state, array $rule = array()) {
  // Default values.
  $rule += array(
    'triggers' => array(),
    'roles' => array(),
    'pages_type' => LOGIN_DESTINATION_REDIRECT_NOTLISTED,
    'pages' => '',
    'destination_type' => LOGIN_DESTINATION_STATIC,
    'destination' => '<front>',
    'id' => NULL,
    'weight' => 0,
  );

  $access = user_access('use PHP for settings');

  $type = $rule['destination_type'];

  if ($type == LOGIN_DESTINATION_SNIPPET && !$access) {
    $form['destination_type'] = array(
      '#type' => 'value',
      '#value' => LOGIN_DESTINATION_SNIPPET,
    );
    $form['destination'] = array(
      '#type' => 'value',
      '#value' => $rule['destination'],
    );
  }
  else {
    $options = array(
      LOGIN_DESTINATION_STATIC => t('Internal page or external URL'),
    );
    $description = t("Specify page by using its path. Example path is %blog for the blog page. %front is the front page. %current is the current page. Precede with http:// for an external URL. Leave empty to redirect to a default page.", array(
      '%blog' => 'blog',
      '%front' => '<front>',
      '%current' => '<current>',
    ));

    if ($access && module_exists('php')) {
      $options += array(LOGIN_DESTINATION_SNIPPET => t('Page returned by this PHP code (experts only)'));
      $description .= ' ' .
        t('If the PHP option is chosen, enter PHP code between %php. It should return either a string value or an array of params that the %function function will understand, for example. %example. For more information, see the online API entry for <a href="@url">url function</a>. Note that executing incorrect PHP code can break your Drupal site.', array(
          '%php' => '<?php ?>',
          '%function' => 'url($path = \'\', array $options = array())',
          '%example' => '<?php return array(\'blog\', array(\'fragment\' => \'overlay=admin/config\', ), ); ?>',
          '@url' => 'http://api.drupal.org/api/drupal/includes--common.inc/function/url/7',
        ));
    }

    $form['destination_type'] = array(
      '#type' => 'radios',
      '#title' => t('Redirect to page'),
      '#default_value' => $type,
      '#options' => $options,
    );
    $form['destination'] = array(
      '#type' => 'textarea',
      '#default_value' => $rule['destination'],
      '#description' => $description,
    );
  }

  $triggers = array_map('check_plain', $rule['triggers']);
  if (empty($triggers)) {
    $triggers = array();
  }

  $form['triggers'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Redirect upon triggers'),
    '#options' => array(
      'login' => t('Login, registration, one-time login link'),
      'logout' => t('Logout'),
    ),
    '#default_value' => $triggers,
    '#description' => t('Redirect only upon selected trigger(s). If you select no triggers, all of them will be used.'),
  );

  $type = $rule['pages_type'];

  if ($type == LOGIN_DESTINATION_REDIRECT_PHP && !$access) {
    $form['pages_type'] = array(
      '#type' => 'value',
      '#value' => LOGIN_DESTINATION_REDIRECT_PHP,
    );
    $form['pages'] = array(
      '#type' => 'value',
      '#value' => $rule['destination'],
    );
  }
  else {
    $options = array(
      LOGIN_DESTINATION_REDIRECT_NOTLISTED => t('All pages except those listed'),
      LOGIN_DESTINATION_REDIRECT_LISTED => t('Only the listed pages'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page. %login is the login form. %register is the registration form. %reset is the one-time login (e-mail validation).", array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>',
      '%login' => 'user',
      '%register' => 'user/register',
      '%reset' => 'user/*/edit',
    ));

    if ($access && module_exists('php')) {
      $options += array(LOGIN_DESTINATION_REDIRECT_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
      $description .= ' ' .
        t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }

    $form['pages_type'] = array(
      '#type' => 'radios',
      '#title' => t('Redirect from specific pages'),
      '#default_value' => $type,
      '#options' => $options,
    );
    $form['pages'] = array(
      '#type' => 'textarea',
      '#default_value' => $rule['pages'],
      '#description' => $description,
    );
  }

  $default_role_options = array_map('check_plain', $rule['roles']);
  if (empty($default_role_options)) {
    $default_role_options = array();
  }

  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Redirect users with roles'),
    '#options' => _login_destination_role_options(),
    '#default_value' => $default_role_options,
    '#description' => t('Redirect only the selected role(s). If you select no roles, all users will be redirected.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  if ($rule['id']) {
    $form['id'] = array(
      '#type' => 'hidden',
      '#value' => $rule['id'],
    );
  }

  return $form;
}

/**
 * Validate the contact category edit page form submission.
 */
function login_destination_edit_form_validate($form, &$form_state) {
  $destination = $form_state['values']['destination'];
  $destination_type = $form_state['values']['destination_type'];

  // Check user has enter any path.
  $available_urls = array('<current>', '<front>');
  if (empty($destination) || $destination_type != 0 || in_array($destination, $available_urls)) {
    return;
  }
  $destination = preg_replace("/\?.+/", "", $destination);
  if (url_is_external($destination)) {
    return;
  }
  // Get source path if an alias entered.
  $source_path = drupal_lookup_path('source', $destination);
  if (!empty($source_path)) {
    $destination = $source_path;
  }
  if (!drupal_valid_path($destination)) {
    form_set_error('destination', t('Incorrect path, please enter a valid path.'));
  }
}

/**
 * Process the contact category edit page form submission.
 */
function login_destination_edit_form_submit($form, &$form_state) {
  $form_state['values']['triggers'] = serialize(array_filter($form_state['values']['triggers']));
  $form_state['values']['roles'] = serialize(array_filter($form_state['values']['roles']));

  if (empty($form_state['values']['id'])) {
    drupal_write_record('login_destination', $form_state['values']);
  }
  else {
    drupal_write_record('login_destination', $form_state['values'], array('id'));
  }

  drupal_set_message(t('Login destination to %destination has been saved.', array('%destination' => $form_state['values']['destination'])));

  $form_state['redirect'] = 'admin/config/people/login-destination';
}

/**
 * Form builder for deleting a login destination.
 */
function login_destination_delete_form($form, &$form_state, array $rule) {
  $form['login_destination'] = array(
    '#type' => 'value',
    '#value' => $rule,
  );

  return confirm_form($form, t('Are you sure you want to delete the login destination %destination ?', array('%destination' => $rule['destination'])), 'admin/config/people/login-destination', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Submit handler for the confirm delete login destination form.
 */
function login_destination_delete_form_submit($form, &$form_state) {
  $rule = $form['login_destination']['#value'];

  db_delete('login_destination')
    ->condition('id', $rule['id'])
    ->execute();

  drupal_set_message(t('The login destination %destination has been deleted.', array('%destination' => $rule['destination'])));

  $form_state['redirect'] = 'admin/config/people/login-destination';
}

/**
 * Settings page.
 */
function login_destination_settings() {
  $form = array();

  $form['settings']['login_destination_preserve_destination'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('login_destination_preserve_destination', FALSE),
    '#title' => t('Preserve the destination parameter'),
    '#description' => t("The 'destination' GET parameter will have priority over the settings of this module. With this setting enabled, redirect from the user login block will not work."),
  );
  $form['settings']['login_destination_immediate_redirect'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('login_destination_immediate_redirect', FALSE),
    '#title' => t('Redirect immediately after using one-time login link'),
    '#description' => t("User will be redirected before given the possibility to change their password."),
  );

  return system_settings_form($form);
}
