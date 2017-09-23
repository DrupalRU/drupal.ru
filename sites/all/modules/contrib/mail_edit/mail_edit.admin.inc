<?php

/**
 * @file
 * Administrative interface for the Mail Editor module.
 */

/**
 * Form builder for the template list.
 *
 * @return array
 *   The $form array.
 */
function mail_edit_list_form() {
  $form['mail_templates'] = mail_edit_list();
  return $form;
}

/**
 * Page callback: List mail templates.
 *
 * @return array
 *   The $form array.
 */
function mail_edit_list() {
  $language_list = mail_edit_language_list();
  $languages_selected = !empty($_SESSION['mail_edit']['languages_selected']) ? $_SESSION['mail_edit']['languages_selected'] : $language_list;
  $module = isset($_SESSION['mail_edit']['module']) ? $_SESSION['mail_edit']['module'] : '-enabled';

  $form['filter'] = mail_edit_list_filter($languages_selected, $module, $language_list);
  $form['filtered_form'] = mail_edit_list_filtered_form($languages_selected, $module, count($language_list) > 1);
  drupal_add_css(drupal_get_path('module', 'mail_edit') . '/mail_edit.admin.css');
  $form['#submit'][] = 'mail_edit_list_submit';
  return $form;
}

/**
 * Filter the mail_edit table.
 *
 * @param array $languages_selected
 *   The list of selected langauges.
 * @param string $module
 *   The selected module option.
 * @param array $language_list
 *   The list of known languages.
 *
 * @return array
 *   The $form array.
 */
function mail_edit_list_filter(array $languages_selected, $module, array $language_list) {
  if (count($language_list) > 1) {
    $form['languages_selected'] = array(
      '#type'          => 'select',
      '#title'         => t('Languages'),
      '#description'   => t('Select the languages to display.'),
      '#options'       => $language_list,
      '#default_value' => array_keys($languages_selected),
      '#multiple'      => TRUE,
    );
  }

  $module_list = _mail_edit_module_list(TRUE);
  $options['-enabled'] = t('- Display enabled modules only -');
  $options['-all'] = t('- Display all modules -');
  _mail_edit_key_registry_rebuild();
  $query = db_select('mail_edit_registry', 'mer')
    ->fields('mer', array('module'))
    ->distinct()
    ->orderBy('mer.module');
  foreach ($query->execute()->fetchCol() as $mod) {
    $options[$mod] = isset($module_list[$mod]) ? $module_list[$mod] : check_plain($mod);
  }

  $form['module'] = array(
    '#type'          => 'select',
    '#title'         => t('Module'),
    '#description'   => t('Select a module.'),
    '#options'       => $options,
    '#default_value' => $module,
  );

  $form['submit'] = array(
    '#type'  => 'item',
    '#value' => '',
  );
  $form['submit']['filter'] = array(
    '#type'  => 'submit',
    '#value' => t('Filter'),
  );
  $form['language_list'] = array(
    '#type'  => 'hidden',
    '#value' => serialize($language_list),
  );

  $form['#theme'] = 'mail_edit_list_filter';
  return $form;
}

/**
 * Create $form array with filtered list.
 *
 * @param array $languages_selected
 *   The list of selected languages.
 * @param string $module
 *   The module option to restrict the list with.
 * @param bool $display_language_names
 *   Whether to display the language names.
 *
 * @return array
 *   The $form array.
 */
function mail_edit_list_filtered_form($languages_selected, $module, $display_language_names) {
  _mail_edit_key_registry_rebuild();

  $header = _mail_edit_list_get_header($languages_selected, $display_language_names);
  _mail_edit_key_registry_rebuild();
  $query = db_select('mail_edit_registry', 'mer')->extend('PagerDefault')->limit(50)
    ->fields('mer', array('id', 'module', 'mailkey', 'description'))
    ->orderBy('mer.module')
    ->orderBy('mer.mailkey');

  switch ($module) {
    case '-all':
      break;

    case '-enabled':
      $enabled_modules = array_keys(_mail_edit_module_list());
      $query->condition('mer.module', $enabled_modules, 'IN');
      break;

    default:
      $query->condition('mer.module', $module);
      break;
  }

  $form['templates']['#tree'] = TRUE;
  $form['#cache'] = TRUE;

  $form['header'] = array(
    '#type'  => 'value',
    '#value' => $header,
  );

  $form['templates'] = array();
  foreach ($query->execute() as $template) {
    // Add all translated languages to the template.
    $languages = db_select('mail_edit', 'me')
      ->fields('me', array('language'))
      ->condition('me.id', $template->id)
      ->execute()
      ->fetchCol();
    foreach ($languages as $language) {
      $template->languages[$language] = TRUE;
    }
    $form['templates'][$template->id] = _mail_edit_list_row($template, $languages_selected, $display_language_names);
  }

  $form['languages'] = array(
    '#type'  => 'value',
    '#value' => $languages_selected,
  );

  $form['#theme'] = 'mail_edit_table';
  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

/**
 * Build the header for the mail edit table.
 *
 * @param array $languages_selected
 *   The languages to include.
 * @param bool $display_language_names
 *   Whether to display the name of the langauge.
 * @return array
 */
function _mail_edit_list_get_header($languages_selected, $display_language_names) {
  $header = array(
    array('data' => t('Module')),
    array('data' => t('Mailkey')),
  );

  if ($display_language_names) {
    foreach ($languages_selected as $lang_name) {
      $header[] = array('data' => $lang_name);
    }
    $header[] = array('data' => t('Purge'));
  }
  else {
    $header[] = array('data' => t('Operations'), 'colspan' => 2);
  }
  return $header;
}

/**
 * Build one row of the mail edit table.
 *
 * @param $template
 *   The mail template for this row.
 * @param $languages array
 *   The languages to include.
 * @param $display_language_names bool
 *   Whether to display the language names
 *
 * @return array
 */
function _mail_edit_list_row($template, array $languages, $display_language_names) {
  $module_list = _mail_edit_module_list(TRUE);
  $form['module'] = array(
    '#markup' => isset($module_list[$template->module]) ? $module_list[$template->module] : check_plain($template->module),
  );

  $form['id'] = array(
    '#markup' => check_plain($template->id),
  );
  $form['mailkey'] = array(
    '#markup' => check_plain($template->mailkey),
  );
  $form['description'] = array(
    '#markup' => filter_xss_admin($template->description),
  );
  $count = 0;
  foreach ($languages as $lang_code => $lang_name) {
    $variable = array('@language' => $lang_code);
    if (isset($template->languages[$lang_code])) {
      $link_text = t($display_language_names ? 'Edit @language' : 'Edit', $variable);
      $count++;
    }
    else {
      $link_text = t($display_language_names ? 'Add @language' : 'Add', $variable);
    }
    $form['operations'] = array(
        '#markup' => l(($count > 0 ? t('Purge All') : t('Purge')), 'admin/config/system/mail-edit/purge/' . $template->module . '_' . $template->mailkey),
    );
    $form[$lang_code] = array(
      '#markup' => l($link_text, 'admin/config/system/mail-edit/' . $template->id . '/' . $lang_code),
    );
  }
  return $form;
}

/**
 * The submit handler for mail_edit_list_form().
 *
 * @param $form
 * @param $form_state
 */
function mail_edit_list_form_submit($form, &$form_state) {
  $language_list = unserialize($form_state['values']['language_list']);
  $_SESSION['mail_edit']['module'] = $form_state['values']['module'];
  if (isset($form_state['values']['languages_selected'])) {
    $_SESSION['mail_edit']['languages_selected'] = array_intersect_key($language_list, $form_state['values']['languages_selected']);
  }
}

/**
 * The theme function for the basic filter form.
 *
 * @param $variables
 * @return string
 *
 * @ingroup themeable
 */
function theme_mail_edit_list_filter($variables) {
  $form = $variables['form'];
  $output = '<div id="mail-edit-filter">';
  foreach (element_children($form) as $key) {
    $attributes = drupal_attributes(array(
      'id'    => 'mail-edit-' . str_replace('_', '-', $key) . '-filter',
      'class' => 'mail-edit-filter',
    ));
    $output .= "<div $attributes>";
    $output .= drupal_render($form[$key]);
    $output .= '</div>';
  }
  $output .= '</div>';

  return $output;
}

/**
 * The theme function for the mail_edit table.
 *
 * @param array $variables
 * @return string
 *
 * @ingroup themeable
 */
function theme_mail_edit_table($variables) {
  $form = $variables['form'];
  $header = $form['header']['#value'];
  $languages = $form['languages']['#value'];

  foreach (element_children($form['templates']) as $key) {
    // Build the table row.
    $row = array();
    $row['data'][] = array('data' => drupal_render($form['templates'][$key]['module']));
    $row['data'][] = array('data' => drupal_render($form['templates'][$key]['mailkey']) . '<div class="description">' . drupal_render($form['templates'][$key]['description']) . '</div>');
    foreach ($languages as $lang_code => $lang_name) {
      $row['data'][] = array('data' => drupal_render($form['templates'][$key][$lang_code]), 'class' => 'mail-edit-table-' . $lang_code);
    };
    $row['data'][] = array('data' => drupal_render($form['templates'][$key]['operations']));
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array(
      'data'    => t('No templates available for override.'),
      'colspan' => count($header),
    ));
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'mail-edit-table')));
  if ($form['pager']) {
    $output .= drupal_render($form['pager']);
  }
  return $output;
}

/**
 * Builds the list of modules that expose mailkeys.
 *
 * @param bool $all
 *   Return all modules if TRUE, or only enabled ones otherwise.
 *
 * @return array
 */
function _mail_edit_module_list($all = FALSE) {
  static $modules;

  if (isset($modules[$all])) {
    return $modules[$all];
  }

  $modules[$all] = array();
  $query = db_select('system', 's')
    ->fields('s', array('name', 'info'))
    ->condition('s.type', 'module')
    ->orderBy('s.name');
  if (!$all) {
    $query->condition('s.status', 1);
  }
  foreach ($query->execute() as $module) {
    $info = unserialize($module->info);
    if (isset($info['name'])) {
      $modules[$all][$module->name] = $info['name'];
    }
  }
  return $modules[$all];
}

/**
 * Form builder function to prepare the template edit form.
 *
 * @param array $form
 * @param array $form_state
 * @param string $id
 * @param string $lang
 *
 * @return array
 */
function mail_edit_template_form(array $form, array $form_state, $id, $lang) {
  _mail_edit_include();
  _mail_edit_module_load_include('alter.inc');
  $template = _mail_edit_load($id, $lang, TRUE);
  if ($template === FALSE) {
    drupal_not_found();
    drupal_exit();
  }
  $new = !empty($template['default']);

  $language_list = mail_edit_language_list();
  $variables = array('%mailkey' => $template['type']['mailkey'], '%language' => $language_list[$lang]);
  if (count($language_list) > 1) {
    $message = t(($new
      ? 'Create new %mailkey template for %language'
      : 'Update %mailkey template for %language'), $variables);
  }
  else {
    $message = t(($new
      ? 'Create new %mailkey template'
      : 'Update %mailkey template'), $variables);
  }
  $dst = 'drupal_set_title';  // Keep Coder happy.
  $dst($message, PASS_THROUGH);

  $form = array();
  $form['update'] = array(
    '#type'  => 'hidden',
    '#value' => !$new,
  );
  $form['language'] = array(
    '#type'  => 'hidden',
    '#value' => $lang,
  );
  $form['id'] = array(
    '#type'  => 'hidden',
    '#value' => $id,
  );
  $form['description'] = array(
    '#title'         => t('Description'),
    '#type'          => 'textfield',
    '#default_value' => isset($template['description']) ? $template['description'] : (isset($template['type']['description']) ? filter_xss($template['type']['description'], array()) : ''),
    '#size'          => 100,
    '#weight'        => -10,
  );

  $form['mail'] = mail_edit_template_subform($template);

  $form['op'] = array(
    '#type'   => 'submit',
    '#value'  => $new ? t('Save new template') : t('Update template'),
    '#submit' => array('mail_edit_template_form_save'),
    '#weight' => 10,
  );
  if (!$new) {
    $form['remove'] = array(
      '#type'   => 'submit',
      '#value'  => (count($language_list) > 1 ? t('Remove @language template', array('@language' => $language_list[$lang])) : t('Remove template')),
      '#submit' => array('mail_edit_template_form_remove'),
      '#weight' => 20,
    );
  }
  $extra_func = $template['type']['module'] . '_mail_edit_form_extra';
  if (function_exists($extra_func)) {
    $extra_func($form, $form_state, $template['type']['mailkey'], $template);
  }
  return $form;
}

/**
 * Prepare a template edit subform.
 *
 * @param array $template
 *   An array with optional 'subject' and 'body' keys, containing the current
 *   subject and body strings.
 *
 * @return array
 */
function mail_edit_template_subform($template) {
  //dpm($template, "mail_edit_template_subform");
  $form = array(
    '#type'          => 'fieldset',
    '#title'         => t('Template'),
    '#collapsible'   => FALSE,
  );
  if (isset($template['subject'])) {
    $form['subject'] = array(
      '#title'         => t('Subject'),
      '#type'          => 'textarea',
      '#default_value' => $template['subject'],
      '#rows'          => 1,
      '#description'   => t("Subject must expand into a single line; it's a multiline field for editing convenience only.")
    );
  }
  if (isset($template['body'])) {
    $form['body'] = array(
      '#title'         => t('Body'),
      '#type'          => 'textarea',
      '#default_value' => $template['body'],
      '#rows'          => 15,
    );
  }

  if (module_exists('token')) {
    $token_types = module_invoke($template['type']['module'], 'mail_edit_token_types', $template['type']['id']);
    $form['token_tree'] = array(
      '#theme'           => 'token_tree',
      '#token_types'     => (empty($token_types) ? array() : $token_types),
      '#show_restricted' => ($template['type']['module'] == 'user'),
      '#weight'          => 10,
      '#dialog'          => TRUE,
    );
    drupal_add_css(drupal_get_path('module', 'mail_edit') . '/mail_edit.admin.css');
  }
  else {
    $form['token_tree'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available tokens.', array('@drupal-token' => 'https://www.drupal.org/project/token')) . '</p>',
    );
  }

  $form['dynamic'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Dynamic text information'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#weight'      => 15,
  );
  $form['dynamic'][] = array(
    '#markup'  => t('Dynamic text provides a way to dynamically adapt templates based on comparisons with token values.') . '<br />' .
                  t('Note: Be careful about spaces and line breaks &mdash; they do count!') .
                  '<div>' .
                  '<div style="font-size: 4ex; margin-top: 1em"><b style="color: red">{{</b><u>left</u>&lt;&gt;<u>right</u><b style="color: red">?</b><u>true_text</u><b style="color: red">:</b><u>false_text</u><b style="color: red">}}</b></div><br />' .
                  '<div style="font-size: 4ex"><b style="color: red">{{</b><u>condition</u><b style="color: red">?</b><u>true_text</u><b style="color: red">:</b><u>false_text</u><b style="color: red">}}</b></div><br />' .
                  '<dl style="margin-bottom: 0">' .
                  '<dt>' . t('!left and !right', array('!left' => '<u>left</u>', '!right' => '<u>right</u>')) . '</dt><dd style="margin-bottom: 0">' . t("Any text (including nothing), which may also contain tokens but no question marks (@qm) or comparison operators.", array('@qm' => "'?'")) . '</dd><br />' .
                  '<dt>&lt;&gt;</dt><dd style="margin-bottom: 0">' . t('Any of the common comparison operators (@list).', array('@list' => "'==', '!=', '>', '<', '>=', '<='")) . '</dd><br />' .
                  '<dt><u>condition</u></dt><dd style="margin-bottom: 0">' . t('A token; the @condition is false if the token evaluates to empty or 0, true otherwise. A prepended exclamation mark (!excl) reverses the logic.', array('@condition' => 'condition', '!excl' => "'!'")) . '</dd><br />' .
                  '<dt><u>true_text</u></dt><dd style="margin-bottom: 0">' . t("Any text (including nothing), but no colon (@colon), except inside token names or nested dynamic clauses.<br />Square brackets (@brackets) must match and cannot be nested.", array('@colon' => "':'", '@brackets' => "'[', ']'")) . '</dd><br />' .
                  '<dt><u>false_text</u></dt><dd style="margin-bottom: 0">' . t("Any text (including nothing).") . '</dd><br />' .
                  '</dl>' .
                  t('If the comparison evaluates to true, then !true_text is used, otherwise !false_text.', array('!true_text' => '<u>true_text</u>', '!false_text' => '<u>false_text</u>')) . '<br />' .
                  '</div>' .
                  '<div>' .
                  '<div style="font-size: 4ex; margin-top: 1em"><b style="color: red">{{</b><u>count</u><b style="color: red">#</b><u>repeated_text</u><b style="color: red">}}</b></div><br />' .
                  '<dl style="margin-bottom: 0">' .
                  '<dt><u>count</u></dt><dd style="margin-bottom: 0">' . t('A number or token that evaluates to a number. A prepended exclamation mark (!excl) turns 0 into 1 and anything else into 0; two prepended exclamation marks (!two_excl) keep 0 and turn anything else into 1.', array('!excl' => "'!'", '!two_excl' => "'!!'")) . '</dd><br />' .
                  '<dt><u>repeated_text</u></dt><dd style="margin-bottom: 0">' . t("The text that is to be repeated !count times. The text can include !number0 and !number1 which will be replaced by the 0- or 1-based index of the current iteration; the former can be used for indexing array tokens, i.e. !example, the latter for numbering the items starting at 1, as the end user would expect.", array('!count' => '<u>count</u>', '!number0' => "'#0'", '!number1' => "'#1'", '!example' => "'[<u>array</u><span style='color: red'>:index:#0</span>:<u>whatever</u>]'")) . '<br />' .
                  t('Example: the following code lists the roles and rids (role keys) of the user, one per line:') . '<br />' .
                  '<code>{{[user:roles:count]# &nbsp; #1: [user:roles:index:#0] ([user:roles:keys:value:#0])<br />}}</code>' . '</dd><br />' .
                  '</dl>' .
                  '</div>' .
                  '<div>' .
                  '<div style="font-size: 4ex; margin-top: 0.5em"><b style="color: red">{{</b><u>context</u><b style="color: red">@</b><u>conditional_text</u><b style="color: red">}}</b></div><br />' .
                  '<dl style="margin-bottom: 0">' .
                  '<dt><u>context</u></dt><dd style="margin-bottom: 0">' . t('A string that is passed in to discern the context of applying the template. A prepended exclamation mark (!excl) reverses the logic.', array('!excl' => "'!'")) . '<br />' . t('If the resulting text is used as a mail message, then the context is !MAIL. Client modules can pass in different context strings to get templates filled in for other purposes.', array('!MAIL' => "'MAIL'")) . '</dd><br />' .
                  '<dt><u>conditional_text</u></dt><dd style="margin-bottom: 0">' . t("The text that is to be used in the given context, or in every other context if the exclamation mark (!excl) is used. This construct supports an additional level of nesting.", array('!count' => '<u>count</u>', '!number0' => "'#0'", '!number1' => "'#1'", '!excl' => "'!'")) . '</dd><br />' .
                  '</dl>' .
                  '</div>' .
                  '<p>' . t('The replacement texts can contain line breaks and tokens, and they support four levels of nesting.') . '</p>' .
                  '<p>' . t("As an extension to Drupal's token system, !Mail_Editor adds support for spaces and newlines inside the separator string of !join tokens.", array('!Mail_Editor' => 'Mail Editor', '!join' => '[*:join:?]')) . '</p>',
  );
  return $form;
}

/**
 * Submit handler to remove a template.
 *
 * @param array $form
 * @param array $form_state
 */
function mail_edit_template_form_remove(array $form, array &$form_state) {
  $form_state['redirect'] = array(current_path() . '/remove');
}

/**
 * Asks for confirmation of template removal.
 *
 * @param array $form
 * @param array $form_state
 * @param string $id
 * @param string $lang_code
 *
 * @return array
 */
function mail_edit_template_remove_confirm(array $form, array &$form_state, $id, $lang_code) {
  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $id,
  );
  $form['lang_code'] = array(
    '#type' => 'hidden',
    '#value' => $lang_code,
  );
  $args = explode('/', current_path());
  array_pop($args);
  $path = implode('/', $args);
  return confirm_form($form, t('Are you sure you want to remove the %mailkey template?', array('%mailkey' => "$id/$lang_code")), $path, NULL, t('Remove'));
}

/**
 * Executes the template removal.
 *
 * @param array $form
 * @param array $form_state
 */
function mail_edit_template_remove_confirm_submit(array $form, array &$form_state) {
  $form_values = $form_state['values'];
  $success = db_delete('mail_edit')
    ->condition('id', $id = $form_values['id'])
    ->condition('language', $lang_code = $form_values['lang_code'])
    ->execute();
  $language_list = mail_edit_language_list();
  $variables = array(
    '%id' => $id,
    '%language' => $language_list[$lang_code],
  );
  $dsm = 'drupal_set_message';  // Keep Coder happy.
  if (count($language_list) > 1) {
    $dsm(t($success
      ? '%language translation of %id has been removed.'
      : 'Failed to remove %language translation of %id.', $variables));
  }
  else {
    $dsm(t($success
      ? 'Template %id has been removed.'
      : 'Failed to remove the %id template.', $variables));
  }
  $form_state['redirect'] = 'admin/config/system/mail-edit';
}

/**
 * Submit handler to save the template edit form.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return void
 */
function mail_edit_template_form_save(array $form, array &$form_state) {
  $values = $form_state['values'];
  $success = drupal_write_record('mail_edit', $values, $values['update'] ? array('id', 'language') : array());
  $language_list = mail_edit_language_list();
  $variables = array('%id' => $values['id'], '%language' => $language_list[$values['language']]);
  if ($success) {
    drupal_set_message(t('Saved the %id template for the %language translation.', $variables));
  }
  $form_state['redirect'] = 'admin/config/system/mail-edit';
}

/**
 * Builds the registry of exposed mailkeys, keeps track of which module
 * exposed them.
 *
 * This function uses the cache system to control when it needs actual
 * rebuilding. It returns immediately, of the general cache has not been
 * cleared, so we can call it everytime before reading the registry.
 *
 * @return void
 */
function _mail_edit_key_registry_rebuild() {
  if (cache_get('mail_edit_registry')) {
    return;
  }
  // Fetch exposed mailkeys.
  $hook = 'mailkeys';
  _mail_edit_include();
  module_implements($hook, FALSE, TRUE);
  $mailkeys = array();
  foreach (module_implements($hook) as $module) {
    $function = $module . '_' . $hook;
    $result = $function();
    if (isset($result)) {
      $mailkeys[$module] = $result;
    }
  }
  // Find out if we already have record of the exposed keys.
  $query = db_select('mail_edit_registry', 'mer')
    ->fields('mer');
  foreach ($query->execute() as $row) {
    if (isset($mailkeys[$row->module][$row->mailkey])) {
      unset($mailkeys[$row->module][$row->mailkey]);
    }
  }
  // Insert any new mailkeys into our registry table.
  foreach ($mailkeys as $module => $keys) {
    foreach ($keys as $key => $description) {
      db_insert('mail_edit_registry')
        ->fields(array(
          'id' => $module . '_' . $key,
          'module' => $module,
          'mailkey' => $key,
          'description' => $description,
        ))
        ->execute();
    }
  }
  cache_set('mail_edit_registry', TRUE, 'cache', CACHE_TEMPORARY);
}

/**
 * Return an array of all the languages in the system.
 *
 * @return array
 */
function mail_edit_language_list() {
  $language_list = array();
  if (module_exists('locale')) {
    $language_list = locale_language_list('name', TRUE);
  }
  else {
    foreach (array_keys(language_list()) as $key) {
      $language_list[$key] = $key;
    }
  }
  return $language_list;
}

/**
 * Page callback for purging a mail template.
 *
 * @param array $form
 * @param array $form_state
 * @param string $mailkey
 *
 * @return array
 */
function mail_edit_purge_mailkey(array $form, array $form_state, $mailkey) {
  $form['mailkey'] = array(
    '#type' => 'hidden',
    '#value' => $mailkey,
  );
  return confirm_form($form, t('Are you sure you want to purge the %mailkey template and all translations?', array('%mailkey' => $mailkey)), 'admin/config/system/mail-edit', NULL, t('Purge'));
}

/**
 * Submit handler for purging a mail template.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return void
 */
function mail_edit_purge_mailkey_submit(array $form, array &$form_state) {
  $id = $form_state['values']['mailkey'];
  $count = db_delete('mail_edit')
    ->condition('id', $id)
    ->execute();
  $count += db_delete('mail_edit_registry')
    ->condition('id', $id)
    ->execute();
  if ($count > 0) {
    drupal_set_message(t('The %mailkey template has been purged.', array('%mailkey' => $id)));
    cache_clear_all('mail_edit_registry', 'cache');
  }
  else {
    drupal_set_message(t('The %mailkey template was not found.', array('%mailkey' => $id)));
  }
  unset($_SESSION['mail_edit']['module']);
  $form_state['redirect'] = 'admin/config/system/mail-edit';
}

/**
 * Implements hook_modules_uninstalled().
 *
 * Removes templates when a module is uninstalled.
 *
 * @param array $modules
 */
function _mail_edit_modules_uninstalled(array $modules) {
  _mail_edit_key_registry_rebuild();
  $templates = db_select('mail_edit_registry', 'mer')
    ->fields('mer')
    ->condition('mer.module', $modules, 'IN')
    ->execute();
  $ids = array();
  foreach ($templates as $template) {
    $ids[] = $template->id;
  }
  if (!empty($ids)) {
    db_delete('mail_edit')
      ->condition('id', $ids, 'IN')
      ->execute();
    db_delete('mail_edit_registry')
      ->condition('id', $ids, 'IN')
      ->execute();
  }
}

