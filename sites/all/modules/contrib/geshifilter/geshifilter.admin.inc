<?php

/**
 * @file
 * Implementation of administration functionality.
 */

require_once drupal_get_path('module', 'geshifilter') .'/geshifilter.inc';

define('GESHIFILTER_FLUSH_LANGUAGE_DEFINITION_CACHE_TEXT', 'Flush the GeSHi language definition cache');

/**
 * Form (items) for filter settings.
 */
function _geshifilter_filter_settings($form, &$form_state, $filter, $format, $defaults, $filters) {
  if (geshifilter_use_format_specific_options()) {
    // tags and attributes
    $settings['general_tags'] = _geshifilter_general_highlight_tags_settings($format);
    $settings['#validate'][] = '_geshifilter_tag_styles_validate';
    // per language tags
    $settings['per_language_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Per language tags'),
      '#collapsible' => TRUE,
      'table' => geshifilter_per_language_settings($format->format, 'enabled', FALSE, TRUE),
    );
  }
  else {
    $settings['info'] = array(
      '#markup' => '<p>'. t('GeSHi filter is configured to use global tag settings. For separate settings per text format, enable this option in the <a href="!geshi_admin_url">general GeSHi filter settings</a>.', array('!geshi_admin_url' => url('admin/config/content/formats/geshifilter'))) .'</p>'
    );
  }
  $settings['#validate'][] = 'geshifilter_per_language_settings_validate';

  return $settings;
}

/**
 * Validation handler for the tag styles form element.
 * Covers usage in the general form of geshifilter_admin_general_settings()
 * and the format specific (sub)form of _geshifilter_filter_settings().
 */
function _geshifilter_tag_styles_validate($form, &$form_state) {
  // If we're coming from the _geshifilter_filter_settings (sub)form, we should
  // take the text format into account.
  $f = isset($form_state['values']['format']) ? '_'. $form_state['values']['format'] : '';

  // Check that at least one tag style is selected.
  if (0 == count(array_filter($form_state['values']["geshifilter_tag_styles{$f}"]))) {
    form_set_error("geshifilter_tag_styles{$f}", t('At least one tag style should be selected.'));
  }
}

/**
 * General settings form for the GeSHi filter.
 */
function geshifilter_admin_general_settings($form, &$form_state) {
  $form = array();

  // Try to load GeSHi library and get version if successful.
  $geshi_library = libraries_load('geshi');

  // GeSHi library settings (constant GESHI_VERSION is defined in GeSHi library)
  $form['geshifilter_library'] = array(
    '#type' => 'fieldset',
    '#title' => defined('GESHI_VERSION') ? t('GeSHi library version @version detected', array('@version' => GESHI_VERSION)) : t('GeSHi library'),
    '#description' => t('The GeSHi filter requires the GeSHi library (which needs to be <a href="!geshi">downloaded</a> and installed seperately).',
      array('!geshi' => url('http://qbnz.com/highlighter/'))),
    '#collapsible' => TRUE,
    '#collapsed' => $geshi_library['loaded'],
  );

  // If the GeSHi library is loaded, show all the options and settings.
  if ($geshi_library['loaded']) {

    // Option for flushing the GeSHi language definition cache.
    $form['geshifilter_library']['geshi_language_definition_caching'] = array(
      '#type' => 'item',
      '#title' => t('GeSHi language definition caching'),
      '#description' => t('The GeSHi library uses languages definition files to define the properties and highlight rules of the supported languages. In most scenarios these language definition files do not change and a lot of derivative data, such as the list of available languages or the CSS style sheet, can be cached for efficiency reasons. Sometimes however, this cache needs to be flushed and the languages definition files need to be reparsed, for example after an upgrade of the GeSHi library or after adding/editing some language definition files manually.'),
    );
    // Non-submitting button for flushing the GeSHi language definition file cache.
    $form['geshifilter_library']['geshi_language_definition_caching']['flush_geshi_language_definition_cache'] = array(
      '#type' => 'button',
      '#value' => t(GESHIFILTER_FLUSH_LANGUAGE_DEFINITION_CACHE_TEXT),
      '#executes_submit_callback' => TRUE,
      '#submit' => array('_geshifilter_flush_language_definition_cache'),
    );

    // GeSHi filter tags and delimiters options
    $form['geshifilter_tag_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('GeSHi filter tags and delimiters'),
      '#collapsible' => TRUE,
    );
    // Usage of format specific options.
    $form['geshifilter_tag_options']['geshifilter_format_specific_options'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use text format specific tag settings.'),
      '#default_value' => geshifilter_use_format_specific_options(),
      '#description' => t('Enable seperate tag settings of the GeSHi filter for each <a href="!input_formats">text format</a> instead of global tag settings.', array('!input_formats' => url('admin/config/content/formats'))),
    );
    // Generic tags settings.
    // @todo: do this conditional form part showing/hiding in jQuery instead of in the form builder.
    if (!geshifilter_use_format_specific_options()) {
      $form['geshifilter_tag_options']['geshifilter_general_tags'] = _geshifilter_general_highlight_tags_settings();
      $form['#validate'][] = '_geshifilter_tag_styles_validate';
    }

    // GeSHi filter highlighting options
    $form['geshifilter_highlighting_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Syntax highlighting options'),
      '#collapsible' => TRUE,
    );
    // default language
    $languages = _geshifilter_get_enabled_languages();
    $form['geshifilter_highlighting_options']['geshifilter_default_highlighting'] = array(
      '#type' => 'select',
      '#title' => t('Default highlighting mode'),
      '#default_value' => variable_get('geshifilter_default_highlighting', GESHIFILTER_DEFAULT_PLAINTEXT),
      '#options' => array(
        t('No highlighting') => array(
          GESHIFILTER_DEFAULT_DONOTHING => t('Do nothing'),
          GESHIFILTER_DEFAULT_PLAINTEXT => t('As plain text'),
          ),
        t('Languages') => $languages,
      ),
      '#description' => t('Select the default highlighting mode to use when no language is defined with a language attribute in the tag.')
    );
    // Default line numbering scheme
    $form['geshifilter_highlighting_options']['geshifilter_default_line_numbering'] = array(
      '#type' => 'select',
      '#title' => t('Default line numbering'),
      '#default_value' => variable_get('geshifilter_default_line_numbering', GESHIFILTER_LINE_NUMBERS_DEFAULT_NONE),
      '#options' => array(
        GESHIFILTER_LINE_NUMBERS_DEFAULT_NONE => t('no line numbers'),
        GESHIFILTER_LINE_NUMBERS_DEFAULT_NORMAL => t('normal line numbers'),
        GESHIFILTER_LINE_NUMBERS_DEFAULT_FANCY5 => t('fancy line numbers (every @n lines)', array('@n' => GESHIFILTER_LINE_NUMBERS_DEFAULT_FANCY5)),
        GESHIFILTER_LINE_NUMBERS_DEFAULT_FANCY10 => t('fancy line numbers (every @n lines)', array('@n' => GESHIFILTER_LINE_NUMBERS_DEFAULT_FANCY10)),
        GESHIFILTER_LINE_NUMBERS_DEFAULT_FANCY20 => t('fancy line numbers (every @n lines)', array('@n' => GESHIFILTER_LINE_NUMBERS_DEFAULT_FANCY20)),
      ),
      '#description' => t('Select the default line numbering scheme: no line numbers, normal line numbers or fancy line numbers. With fancy line numbers every n<sup>th</sup> line number is highlighted. (GeSHi documentation: <a href="!link">Line numbers</a>).',
        array('!link' => 'http://qbnz.com/highlighter/geshi-doc.html#line-numbers')),
    );
    // highlight_string usage option
    $form['geshifilter_highlighting_options']['geshifilter_use_highlight_string_for_php'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use built-in PHP function <code>highlight_string()</code> for PHP source code.'),
      '#description' => t('When enabled, PHP source code will be syntax highlighted with the built-in PHP function <code><a href="!highlight_string">highlight_string()</a></code> instead of with the GeSHi library. GeSHi features, like line numbering and usage of an external CSS stylesheet for example, are not available.',
        array('!highlight_string' => 'http://php.net/manual/en/function.highlight-string.php')),
      '#default_value' => variable_get('geshifilter_use_highlight_string_for_php', FALSE),
    );
    // Option to disable Keyword URL's
    $form['geshifilter_highlighting_options']['geshifilter_enable_keyword_urls'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable GeSHi keyword URLs'),
      '#description' => t('For some languages GeSHi can link language keywords (e.g. standard library functions) to their online documentation. (GeSHi documentation: <a href="!link">Keyword URLs</a>).',
        array('!link' => 'http://qbnz.com/highlighter/geshi-doc.html#keyword-urls')),
      '#default_value' => variable_get('geshifilter_enable_keyword_urls', TRUE),
    );

    // Styling, layout and CSS
    $form['geshifilter_styling'] = array(
      '#type' => 'fieldset',
      '#title' => t('Styling, layout and CSS'),
      '#collapsible' => TRUE,
    );

    // CSS mode
    $form['geshifilter_styling']['geshifilter_css_mode'] = array(
      '#type' => 'radios',
      '#title' => t('CSS mode for syntax highlighting'),
      '#options' => array(
        GESHIFILTER_CSS_INLINE => t('Inline CSS style attributes.'),
        GESHIFILTER_CSS_CLASSES_AUTOMATIC => t('Use CSS classes and an automatically managed external CSS style sheet.'),
        GESHIFILTER_CSS_CLASSES_ONLY => t('Only add CSS classes to the markup.'),
      ),
      '#default_value' => variable_get('geshifilter_css_mode', GESHIFILTER_CSS_INLINE),
      '#description' => t('Inline CSS is easier to set up, does not depend on an external style sheets and is consequently more robust to copy/paste operations like content aggregation. However, usage of CSS classes and an external stylesheet requires much less markup code and bandwidth. The external style sheet can be managed automatically by the GeSHi filter module, but this feature requires the public <a href="!filesystem">download method</a>. If the GeSHi filter is configured to only add the CSS classes to the markup, the administrator or themer is responsible for adding the appropriate CSS rules to the pages (e.g. based on <a href="!cssdefaults">these defaults</a>). (GeSHi documentation: <a href="!geshidoc">Using CSS Classes</a>).',
        array(
          '!geshidoc' => 'http://qbnz.com/highlighter/geshi-doc.html#using-css-classes',
          '!filesystem' => url('admin/config/media/file-system'),
          '!cssdefaults' => url('admin/config/content/formats/geshifilter/generate_css'),
        )),
    );

    // Code container
    $container_options = array(
      GESHI_HEADER_PRE => t('%val: uses a @cnt wrapper, efficient whitespace coding, no automatic line wrapping, generates invalid HTML with line numbering.',
        array('%val' => 'GESHI_HEADER_PRE', '@cnt' => '<pre>')),
      GESHI_HEADER_DIV => t('%val: uses a @cnt wrapper, enables automatic line wrapping.',
        array('%val' => 'GESHI_HEADER_DIV', '@cnt' => '<div>')),
    );
    if (version_compare(GESHI_VERSION, '1.0.8', '>=')) {
      $container_options[GESHI_HEADER_PRE_VALID] = t('%val: uses @pre wrappers, ensures valid HTML with line numbering, but generates more markup.',
        array('%val' => 'GESHI_HEADER_PRE_VALID', '@pre' => '<pre>', '@li' => '<li>'));
      $container_options[GESHI_HEADER_PRE_TABLE] = t('%val: uses a @table construction for adding line numbers which avoids selection/copy/paste problems.',
        array('%val' => 'GESHI_HEADER_PRE_TABLE', '@table' => '<table>'));
    }
    if (version_compare(GESHI_VERSION, '1.0.7.2', '>=')) {
      $container_options[GESHI_HEADER_NONE] = t('%val: uses no wrapper.',
        array('%val' => 'GESHI_HEADER_NONE'));
    }

    $form['geshifilter_styling']['geshifilter_code_container'] = array(
      '#type' => 'radios',
      '#title' => t('Code container, wrapping technique'),
      '#description' => t('Define the wrapping technique to use for code blocks. (GeSHi documentation: <a href="!link">The Code Container</a>).',
        array('!link' => 'http://qbnz.com/highlighter/geshi-doc.html#the-code-container')
      ),
      '#options' => $container_options,
      '#default_value' => variable_get('geshifilter_code_container', GESHI_HEADER_PRE),
    );
  }
  // Make it a system settings form.
  $form  = system_settings_form($form);
  // Set custom validate/submit handlers (to be run after system_settings_form submit).
  $form['#validate'][] = 'geshifilter_admin_general_settings_validate';
  $form['#submit'][] = 'geshifilter_admin_general_settings_submit';
  return $form;
}

/**
 * Validate function for admin settings.
 */
function geshifilter_admin_general_settings_validate($form, &$form_state) {
  // Check if automatically managed style sheet is posible
  if (isset($form_state['values']['geshifilter_css_mode']) && $form_state['values']['geshifilter_css_mode'] == GESHIFILTER_CSS_CLASSES_AUTOMATIC && !_geshifilter_managed_external_stylesheet_possible()) {
    form_set_error('geshifilter_css_mode', t('GeSHi filter can not automatically manage an external CSS style sheet when the download method is private.'));
  }
}

/**
 * Submit function for admin settings.
 */
function geshifilter_admin_general_settings_submit($form, &$form_state) {
  // Regenerate language_css
  // Note the use of variable_get('geshifilter_css_mode') which is alread set
  // to $form_state['values']['geshifilter_css_mode'] by
  // system_settings_form_submit, which is run before this submit handler.
  if (variable_get('geshifilter_css_mode', GESHIFILTER_CSS_INLINE) == GESHIFILTER_CSS_CLASSES_AUTOMATIC) {
    _geshifilter_generate_languages_css_file();
  }
  // Always clear the filter cache.
  _geshifilter_clear_filter_cache();
}

/**
 * Menu callback for per language settings.
 */
function geshifilter_admin_per_language_settings($form, &$form_state, $view = 'enabled') {
  // check if GeSHi library is available
  $geshi_library = libraries_load('geshi');
  if (!$geshi_library['loaded']) {
    drupal_set_message($geshi_library['error message'], 'error');
    return;
  }
  $add_checkbox = TRUE;
  $add_tag_option = (!geshifilter_use_format_specific_options());
  $form['language_settings'] = geshifilter_per_language_settings(NULL, $view, $add_checkbox, $add_tag_option);

  // Make it a system settings form.
  $form = system_settings_form($form);

  // set custom validate and submit handlers
  $form['#validate'][] = 'geshifilter_per_language_settings_validate';
  $form['#submit'][] = 'geshifilter_admin_per_language_settings_submit';

  return $form;
}

/**
 * Validate function for per language settings.
 *
 * Check that language tags differ between languages and fro the generic tags.
 */
function geshifilter_per_language_settings_validate($form, &$form_state) {

  // if we're coming from the _geshifilter_filter_settings (sub)form, we should
  // take the text format into account
  $format = isset($form_state['values']['format']) ? $form_state['values']['format'] : NULL;
  $f = ($format === NULL) ? '' : "_$format";

  // language tags should differ from each other
  $languages = _geshifilter_get_available_languages();
  foreach ($languages as $language1 => $language_data1) {
    // iterate over the enabled languages: skip disabled ones
    $field = "geshifilter_language_enabled_{$language1}";
    if (!(isset($form_state['values'][$field]) ? $form_state['values'][$field] : variable_get($field, FALSE))) {
      continue;
    }
    // get the associated tags as $tags1
    $field = "geshifilter_language_tags_{$language1}{$f}";
    if (isset($form_state['values'][$field])) {
      $tags1 = _geshifilter_tag_split($form_state['values'][$field]);
    }
    else {
      $tags1 = _geshifilter_tag_split(geshifilter_language_tags($language1, $format));
    }
    // also include the generic tags
    $field = "geshifilter_tags{$f}";
    $generic_tags = isset($form_state['values'][$field]) ? $form_state['values'][$field] : geshifilter_tags($format);
    $tags1 = array_merge($tags1, _geshifilter_tag_split($generic_tags));

    // check that other languages do not use these tags
    foreach ($languages as $language2 => $language_data2) {
      // check these tags against the tags of other enabled languages
      $field = "geshifilter_language_enabled_{$language2}";
      if ($language1 == $language2 || !(isset($form_state['values'][$field]) ? $form_state['values'][$field] : variable_get($field, FALSE))) {
        continue;
      }
      // get tags for $language2, or skip when not in $form_state['values']
      $field = "geshifilter_language_tags_{$language2}{$f}";
      if (isset($form_state['values'][$field])) {
        $tags2 = _geshifilter_tag_split($form_state['values'][$field]);
      }
      else {
        continue;
      }
      // and now we can check tags1 against tags2
      foreach ($tags1 as $tag1) {
        foreach ($tags2 as $tag2) {
          if ($tag1 == $tag2) {
            form_set_error("geshifilter_language_tags_{$language2}{$f}",  t('The language tags should differ between languages and from the generic tags.'));
          }
        }
      }
    }
  }
}

/**
 * Submit function for per language settings.
 */
function geshifilter_admin_per_language_settings_submit($form, &$form_state) {
  // regenerate language_css
  if (variable_get('geshifilter_css_mode', GESHIFILTER_CSS_INLINE) == GESHIFILTER_CSS_CLASSES_AUTOMATIC) {
    _geshifilter_generate_languages_css_file();
  }
  _geshifilter_clear_filter_cache();
}

/**
 * Helper function for some settings form fields usable as general and specific settings.
 *
 * @param stdClass $format format object (e.g. with field $format->format).
 */
function _geshifilter_general_highlight_tags_settings($format = NULL) {
  $form = array();
  $f = ($format === NULL) ? '' : "_$format->format";
  $format_id = ($format === NULL) ? NULL : $format->format;
  
  // generic tags
  $form["geshifilter_tags$f"] = array(
    '#type' => 'textfield',
    '#title' => t('Generic syntax highlighting tags'),
    '#default_value' => geshifilter_tags($format_id),
    '#description' => t('Tags that should activate the GeSHi syntax highlighting. Specify a space-separated list of tagnames.'),
  );
  // Container tag styles.
  $form["geshifilter_tag_styles$f"] = array(
    '#type' => 'checkboxes',
    '#title' => t('Container tag style'),
    '#options' => array(
      GESHIFILTER_BRACKETS_ANGLE => '<code>'. check_plain('<foo> ... </foo>') .'</code>',
      GESHIFILTER_BRACKETS_SQUARE => '<code>'. check_plain('[foo] ... [/foo]') .'</code>',
      GESHIFILTER_BRACKETS_DOUBLESQUARE => '<code>'. check_plain('[[foo]] ... [[/foo]]') .'</code>',
      GESHIFILTER_BRACKETS_PHPBLOCK => t('PHP style source code blocks: !php and !percent', array(
        '!php' => '<code>'. check_plain('<?php ... ?>'). '</code>',
        '!percent' =>  '<code>'. check_plain('<% ... %>'). '</code>',
      )),
    ),
    '#default_value' => _geshifilter_tag_styles($format_id),
    '#description' => t('Select the container tag styles that should trigger GeSHi syntax highlighting.'),
  );
  return $form;
}

/**
 * Function for generating a form table for per language settings.
 */
function geshifilter_per_language_settings($format, $view, $add_checkbox, $add_tag_option) {
  $form = array(
    '#theme' => 'geshifilter_per_language_settings',
  );
  // table header
  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(),
  );
  $form['header']['#value'][] = t('Language');
  $form['header']['#value'][] = t('GeSHi language code');
  if ($add_tag_option) {
    $form['header']['#value'][] = t('Tag/language attribute value');
  }
  // table body
  $form['languages'] = array();
  $languages = _geshifilter_get_available_languages();
  foreach ($languages as $language => $language_data) {
    $enabled = variable_get("geshifilter_language_enabled_{$language}", FALSE);
    // skip items to hide
    if (($view == 'enabled' && !$enabled) || ($view == 'disabled' && $enabled)) {
      continue;
    }
    // build language row
    $form['languages'][$language] = array();
    // add enable/disable checkbox
    if ($add_checkbox) {
      $form['languages'][$language]["geshifilter_language_enabled_{$language}"] = array(
        '#type' => 'checkbox',
        '#default_value' => $enabled,
        '#title' => $language_data['fullname'],
      );
    }
    else {
      $form['languages'][$language]['fullname'] = array(
        '#type' => 'markup',
        '#markup' => $language_data['fullname'],
      );
    }
    // language code
    $form['languages'][$language]['name'] = array(
      '#type' => 'markup',
      '#markup' => $language,
    );
    // add a textfield for tags
    if ($add_tag_option) {
      $f = ($format === NULL) ? '' : "_$format";
      $form['languages'][$language]["geshifilter_language_tags_{$language}{$f}"] = array(
        '#type' => 'textfield',
        '#default_value' => geshifilter_language_tags($language, $format),
        '#size' => 20,
      );
    }
  }
  return $form;
}

/**
 * Theming function for the per language settings table.
 */
function theme_geshifilter_per_language_settings($variables) {
  $form = $variables['form'];
  // build table body
  $rows = array();
  foreach (element_children($form['languages']) as $language) {
    $row = array();
    foreach (element_children($form['languages'][$language]) as $key) {
      $row[] = drupal_render($form['languages'][$language][$key]);
    }
    $rows[] = $row;
  }
  // build table header
  $header = array();
  foreach ($form['header']['#value'] as $cell) {
    if ($cell == t('Enabled')) {
      // add JavaScript 'select all/select none' stuff
      $header[] = array('data' => $cell) + theme('table_select_header_cell');
    }
    else {
      $header[] = $cell;
    }
  }
  // return themed table
  return theme('table', array(
    'header' => $header,
    'rows' => $rows
  ));
}

/**
 * Helper function for flushing the GeSHi language definition cache.
 */
function _geshifilter_flush_language_definition_cache() {
  if (GESHIFILTER_CSS_CLASSES_AUTOMATIC == variable_get('geshifilter_css_mode', GESHIFILTER_CSS_INLINE)) {
    // Forced regeneration of the CSS file
    _geshifilter_generate_languages_css_file(TRUE);
  }
  // Flush cache of available languages
  variable_del('geshifilter_available_languages_cache');
  drupal_set_message(t('Flushed the GeSHi language definition cache.'));
}

/**
 * Helper function for clearing the appropriate filter cache entries.
 */
function _geshifilter_clear_filter_cache() {
  $message = FALSE;
  global $user;

  // Iterate over text formats and clear the ones GeSHi filter is active in.
  foreach (filter_formats($user) as $format => $input_format) {
    // Get the filters in this text format
    $filters = filter_list_format($format);
    // look if GeSHi is enabled in this text format
    if (isset($filters['geshifilter/0'])) {
      cache_clear_all($format .':', 'cache_filter', TRUE);
      $message = TRUE;
    }
  }

  // Clear the geshifilter entries in the cache_filter table,
  // created by geshifilter_geshi_process().
  cache_clear_all('geshifilter:', 'cache_filter', TRUE);

  // Show a message that the filter cache was cleared.
  if ($message) {
    drupal_set_message(t('GeSHi filter cleared the filter cache for the appropriate text formats.'));
  }
}

/**
 * Helper function for generating the CSS rules.
 *
 * @return string with the CSS rules.
 */
function _geshifilter_generate_languages_css_rules() {
  $output = '';
  $geshi_library = libraries_load('geshi');
  if ($geshi_library['loaded']) {
    require_once drupal_get_path('module', 'geshifilter') .'/geshifilter.pages.inc';
    $languages = _geshifilter_get_enabled_languages();
    foreach ($languages as $langcode => $language_full_name) {
      // create GeSHi object
      $geshi = _geshifilter_GeSHi_factory('', $langcode);
      _geshifilter_override_geshi_defaults($geshi, $langcode);
      // add CSS rules for current language
      $output .= $geshi->get_stylesheet(FALSE) ."\n";
      // release GeSHi object
      unset($geshi);
    }
  }
  else {
    drupal_set_message(t('Error while generating CSS rules: could not load GeSHi library.'), 'error');
  }
  return $output;
}

/**
 * Function for generating the external stylesheet.
 *
 * @param $force force the regeneration of the CSS file.
 */
function _geshifilter_generate_languages_css_file($force=FALSE) {
  $languages = _geshifilter_get_enabled_languages();
  // Serialize the array of enabled languages as sort of hash.
  $languages_hash = serialize($languages);

  // Check if generation of the CSS file is needed
  if ($force || $languages_hash != variable_get('geshifilter_cssfile_languages', '')) {
    // Build stylesheet.
    $stylesheet = _geshifilter_generate_languages_css_rules();
    // Save stylesheet.
    $stylesheet_filename = _geshifilter_language_css_path();;
    $ret = file_save_data($stylesheet, $stylesheet_filename, FILE_EXISTS_REPLACE);
    if ($ret) {
      drupal_set_message(t('(Re)generated external CSS style sheet %file.', array('%file' => $ret)));
    }
    else {
      drupal_set_message(t('Could not generate external CSS file. Check the settings of your <a href="!filesystem">file system</a>.',
        array('!filesystem' => url('admin/config/media/file-system'))), 'error');
    }
    // Remember for which list of languages the CSS file was generated
    variable_set('geshifilter_cssfile_languages', $languages_hash);
  }
}
