<?php

/**
 * @file
 * Administrative interface for modifying tags and settings.
 */

/**
 * List custom tags and edit or delete them.
 *
 * @param $form
 *   The prepopulated form array.
 * @param $form_state
 *   The state of the (entire) configuration form.
 * @param $name
 *   If passed, load this tag for editing. Otherwise, list all tags and show a
 *   collapsed tag creation form.
 *
 * @return
 *   A form ready for building.
 */
function xbbcode_custom_tags($form, &$form_state, $name = NULL) {
  module_load_include('inc', 'xbbcode', 'xbbcode.crud');
  // Determine whether the user has loaded an existing tag for editing (via edit link).
  $editing_tag = !empty($name);
  // If the form was submitted, then a new tag is being added.
  $adding_tag = !empty($form_state['input']) && $form_state['input']['op'] == t('Save');
  $access_php = module_exists('php') && user_access('use PHP for settings');
  $use_php = FALSE;

  // The upshot is that if a tag is being edited or added, the otherwise optional fields become required.

  // If editing a tag, load this tag and populate the form with its values.
  if ($editing_tag) {
    $tag = xbbcode_custom_tag_load($name);
    $use_php = $tag->options['php'];
    $form['edit'] = array(
      '#type' => 'fieldset',
      '#title' => t('Editing Tag %name', array('%name' => $name)),
      '#collapsible' => FALSE,
    );
  }
  else {
    $tags = array_keys(xbbcode_custom_tag_load());

    // If any tags already exist, build a list for deletion and editing.
    if (!empty($tags)) {
      foreach ($tags as $tag) {
        $options[$tag] = '[' . $tag . '] ' . l(t('Edit'), "admin/config/content/xbbcode/tags/$tag/edit");
      }
      $form['existing'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Existing tags'),
        '#description' => t('Check these tags and click "Delete" to delete them.'),
        '#options' => $options,
      );
    }
    else {
      // If no tags exist, then a new tag must be added now.
      $adding_tag = TRUE;
    }

    $form['edit'] = array(
      '#type' => 'fieldset',
      '#title' => t('Create new tag'),
      '#collapsible' => TRUE,
      '#collapsed' => count($tags),
    );

    // Create an empty tag.
    $tag = (object)array(
      'name' => '',
      'description' => '',
      'markup' => '',
      'sample' => '',
    );
  }

  // Regardless of whether a new tag or an existing tag is being edited,
  // show the edit form now. The fields are required only if a new tag is being
  // saved (during the submission phase), or if an existing tag is being edited.

  $form['edit']['name'] = array(
    '#type' => 'textfield',
    '#default_value' => $tag->name,
    '#field_prefix' => '[',
    '#field_suffix' => ']',
    '#required' => $editing_tag || $adding_tag,
    '#maxlength' => 32,
    '#size' => 16,
    '#description' => t('The name of this tag. The name will be used in the text as [name]...[/name]. Must be alphanumeric and will automatically be converted to lowercase.'),
  );

  $form['edit']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $tag->description,
    '#required' => $editing_tag || $adding_tag,
    '#description' => t('This will be shown on help pages'),
  );

  $form['edit']['sample'] = array(
    '#type' => 'textfield',
    '#title' => t('Sample tag'),
    '#required' => $editing_tag || $adding_tag,
    '#description' => t('Enter an example of how this tag would be used. It will be shown on the help pages.'),
    '#default_value' => $tag->sample,
  );

  $form['edit']['options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Tag options'),
    '#options' => array(
      'selfclosing' => t('Tag is self-closing (requires no closing tag, like <code>[img]</code>).'),
      'nocode' => t('Ignore further BBCode inside this tag.'),
      'plain' => t('Escape all HTML inside this tag.'),
    ),
    '#description' => t('The last two options should in most cases be used together. Note that HTML will not be escaped twice even if this tag is used in a format that allows no HTML in the first place.'),
  );

  $form['edit']['php'] = array(
    '#type' => 'checkbox',
    '#title' => t('Evaluate as PHP code.'),
    '#return_value' => TRUE,
    '#description' => t('This option requires the PHP module to be enabled, and the appropriate permission.'),
    '#default_value' => $use_php,
  );

  foreach ($form['edit']['options']['#options'] as $key => $value) {
    if (!empty($tag->options[$key])) {
      $form['edit']['options']['#default_value'][] = $key;
    }
  }

  $form['edit']['markup'] = array(
    '#type' => 'textarea',
    '#attributes' => array('style' => 'font-family:monospace'),
    '#title' => t('Rendering code'),
    '#default_value' => $tag->markup,
    '#required' => $editing_tag || $adding_tag,
    '#description' => t('The text that [tag]content[/tag] should be replaced with, or PHP code that prints/returns the text.'),
  );

  if (!$access_php) {
    $form['edit']['php']['#disabled'] = TRUE;
    $form['edit']['php']['#value'] = $form['edit']['php']['#default_value'];
    // Imitate the behavior of filter.module on forbidden formats.
    if ($use_php) {
      $form['edit']['markup']['#disabled'] = TRUE;
      $form['edit']['markup']['#resizable'] = FALSE;
      $form['edit']['markup']['#value'] = $form['edit']['markup']['#default_value'];
      $form['edit']['markup']['#pre_render'] = array('filter_form_access_denied');
    }
  }

  $form['edit']['help'] = array(
    '#type' => 'markup',
    '#title' => t('Coding help'),
    '#markup' => t('<p>The above field should be filled either with HTML or PHP code depending on whether you enabled the PHP code option. PHP code must be placed in &lt;?php ?&gt; enclosures, or it will be
    printed literally.</p>
    <p>If your tag uses static HTML, then the tag\'s content and attributes will be inserted into your code by replacing placeholders. In PHP code, they will be available in the <code>$tag</code> object.</p>
    <dl>
      <dt><code>{content}</code> or <code>$tag->content</code></dt>
      <dd>The text between opening and closing tags, if the tag is not self-closing. Example: <code>[url=http://www.drupal.org]<strong>Drupal</strong>[/url]</code></dd>
      <dt><code>{option}</code> or <code>$tag->option</code></dt>
      <dd>The single tag attribute, if one is entered. Example: <code>[url=<strong>http://www.drupal.org</strong>]Drupal[/url]</code>.</dd>
      <dt>any other <code>{attribute}</code> or <code>$tag->attr(\'attribute\')</code></dt>
      <dd>The tag attribute of the same name, if it is entered. E.g: <strong>{by}</strong> or <strong><code>$tag->attr(\'by\')</code></strong> for <code>[quote&nbsp;by=<strong>Author</strong>&nbsp;date=2008]Text[/quote]</code>. If the attribute is not entered, the placeholder will be replaced with an empty string, and the <code>attr()</code> return value will be <code>NULL</code>.</dd>
    </dl>'),
  );

  $form['edit']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('xbbcode_custom_tags_save_submit'),
  );

  if (!empty($name) || count($tags)) {
    $delete = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('xbbcode_custom_tags_delete_submit'),
    );
    if (!empty($name)) {
      $form['edit']['delete'] = $delete;
    }
    else {
      $form['delete'] = $delete;
    }
  }

  return $form;
}

/**
 * Validation of the custom tags form.
 */
function xbbcode_custom_tags_validate($form, $form_state) {
  if (!preg_match('/^[a-z0-9]*$/i', $form_state['values']['name'])) {
    form_set_error('name', t('The name must be alphanumeric.'));
  }

  if ($form['edit']['name']['#default_value'] != $form_state['values']['name']) {
    if (xbbcode_custom_tag_exists($form_state['values']['name'])) {
      form_set_error('name', t('This name is already taken. Please delete or edit the old tag, or choose a different name.'));
    }
  }
}

/**
 * Delete a custom tag.
 */
function xbbcode_custom_tags_delete_submit($form, $form_state) {
  $delete = array();

  if (!empty($form_state['values']['name'])) {
    $delete[] = $form_state['values']['name'];
  }
  elseif (is_array($form_state['values']['existing'])) {
    foreach ($form_state['values']['existing'] as $tag => $checked) {
      if ($checked) {
        $delete[] = $tag;
      }
    }
  }

  xbbcode_custom_tag_delete($delete);

  $tags = '[' . implode('], [', $delete) . ']';

  drupal_set_message(format_plural(count($delete), 'The tag %tags has been deleted.', 'The tags %tags have been deleted.', array('%tags' => $tags)), 'status');
  drupal_static_reset('xbbcode_custom_tag_load');
  xbbcode_rebuild_handlers();
  xbbcode_rebuild_tags();
}

/**
 * Save (create or update) a custom tag.
 */
function xbbcode_custom_tags_save_submit($form, &$form_state) {
  $tag = (object) $form_state['values'];
  $tag->name = strtolower($tag->name);
  foreach ($tag->options as $name => $value) {
    $tag->options[$name] = $value ? 1 : 0;
  }
  $tag->options['php'] = $tag->php;

  if (xbbcode_custom_tag_save($tag)) {
    if ($form['edit']['name']['#default_value']) {
      drupal_set_message(t('Tag [@name] has been changed.', array('@name' => $tag->name)));
    }
    else {
      drupal_set_message(t('Tag [@name] has been created.', array('@name' => $tag->name)));
    }
  }
  $form_state['redirect'] = array('admin/config/content/xbbcode/tags', array());
  drupal_static_reset('xbbcode_custom_tag_load');
  xbbcode_rebuild_handlers();
  xbbcode_rebuild_tags();
}

/**
 * Modify the global handler settings.
 */
function xbbcode_settings_handlers($form, &$form_state) {
  // Load the database interface.
  module_load_include('inc', 'xbbcode', 'xbbcode.crud');
  // Find out which formats use global settings.
  $formats = xbbcode_formats();

  $form = array(
    'global' => array(),
    'tags' => array(),
  );

  $form['global'] = array(
    '#weight' => -1,
    '#markup' => t('You are changing the global settings.'),
  );

  foreach ($formats as &$list) {
    foreach ($list as $format_id => $format_name) {
      $list[$format_id] = l($format_name, 'admin/config/content/formats/' . $format_id);
    }
  }

  if (!empty($formats['specific'])) {
    if (!empty($formats['global'])) {
      $form['global']['#markup'] .= ' ' . t('The following formats are affected by the global settings:');
      $form['global']['#markup'] .= '<ul><li>' . implode('</li><li>', $formats['global']) . '</li></ul>';
    }
    else {
      $form['global']['#markup'] .= ' ' . t('All formats using XBBCode currently override the global settings, so they have no effect.');
    }
    $form['global']['#markup'] .= ' ' . t('The following formats override the global settings, and will not be affected:');
    $form['global']['#markup'] .= '<ul><li>' . implode('</li><li>', $formats['specific']) . '</li></ul>';
  }
  else {
    $form['global']['#markup'] .= ' ' . t('All formats currently follow the global settings.');
  }

  $form['tags'] = xbbcode_settings_handlers_format();

  $form['save'] = array(
    '#type' => 'submit',
    '#name' => 'op',
    '#value' => t('Save changes'),
    '#submit' => array('xbbcode_settings_handlers_save_submit'),
  );
  return $form;
}

/**
 * Modify handler settings (subform).
 */
function xbbcode_settings_handlers_format($format = XBBCODE_GLOBAL) {
  drupal_add_css(drupal_get_path('module', 'xbbcode') . '/xbbcode.css');
  drupal_add_js(drupal_get_path('module', 'xbbcode') . '/xbbcode.js');
  module_load_include('inc', 'xbbcode');
  $handlers = _xbbcode_build_handlers();
  $defaults = xbbcode_handlers_load($format, TRUE);

  $form = array(
    '#type' => 'fieldset',
    '#theme' => 'xbbcode_settings_handlers_format',
    '#tree' => TRUE,
    '#title' => t('Tag settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['_enabled'] = array(
    '#type' => 'tableselect',
    '#header' => array(
      'tag' => t('BBCode tag'),
      'module' => t('Module'),
    ),
    '#default_value' => array(),
    '#options' => array(),
    '#attributes' => array('id' => 'xbbcode-handlers'),
    '#empty' => t('No tags or handlers are defined. Please <a href="@modules">install a tag module</a> or <a href="@custom">create some custom tags</a>.', array('@modules' => url('admin/modules', array('fragment' => 'edit-modules-xbbcode')), '@custom' => url('admin/config/content/xbbcode/tags'))),
  );

  foreach ($handlers as $name => $handler) {
    if(!isset($defaults[$name])) {
      $defaults[$name] = (object) array(
        'module' => key($handler['modules']),
        'enabled' => FALSE,
      );
    }

    $form['_enabled']['#options'][$name] = array(
      'tag' => array(
        'data' => _xbbcode_build_descriptions($name, $handler['info'], $defaults[$name]->module),
        'class' => array('xbbcode-tag-description', 'xbbcode-tag-td'),
      ),
      'module' => array(
        'data' => 'handler-select',
        'class' => array('xbbcode-tag-td'),
      ),
      '#attributes' => array('class' => $defaults[$name]->enabled ? array('selected') : array()),
    );
    $form['_enabled']['#default_value'][$name] = $defaults[$name]->enabled ? 1 : NULL;

    $form[$name]['module'] = array(
      '#type' => 'select',
      '#options' => $handler['modules'],
      '#default_value' => $defaults[$name]->module,
      '#attributes' => array('class' => array('xbbcode-tag-handler')),
    );
  }
  return $form;
}

/**
 * Renders the handlers subform as a table.
 */
function theme_xbbcode_settings_handlers_format($variables) {
  $fieldset = $variables['fieldset'];
  $fieldset['_enabled']['#attributes']['id'] = 'xbbcode-handlers';

  foreach (element_children($fieldset) as $tag) {
    if (isset($fieldset[$tag]['#type'])) {
      continue;
    }

    if (count($fieldset[$tag]['module']['#options']) == 1) {
      $fieldset[$tag]['module'] = array(
        'shown' => array(
          '#type' => 'markup',
          '#markup' => current($fieldset[$tag]['module']['#options']),
        ),
        '#type' => 'value',
        '#value' => key($fieldset[$tag]['module']['#options']),
      );
    }

    $fieldset['_enabled']['#options'][$tag]['module']['data'] = drupal_render($fieldset[$tag]['module']);
  }
  ksort($fieldset['_enabled']['#options']);

  $html = drupal_render($fieldset['_enabled']);
  foreach (element_children($fieldset) as $element) {
    $html .= drupal_render($fieldset[$element]);
  }
  return $html;
}

/**
 * Save the handler settings.
 */
function xbbcode_settings_handlers_save_submit($form, $form_state) {
  // Determine if the settings are edited globally or in a text format.
  if (isset($form['#format'])) {
    // If a format has just been created, the #format info is still empty.
    if (!empty($form['#format']->format)) {
      $format_id = $form['#format']->format;
    }
    else {
      $format_id = $form_state['values']['format'];
    }
    $settings = $form_state['values']['filters']['xbbcode']['settings'];
  }
  else {
    $format_id = XBBCODE_GLOBAL;
    $settings = $form_state['values'];
  }

  if ($format_id == XBBCODE_GLOBAL || $settings['override']) {
    // Change the global settings or a format with specific settings.
    $enabled = $settings['tags']['_enabled'];
    unset($settings['tags']['_enabled']);
    foreach ($settings['tags'] as $name => $values) {
      if (is_array($values)) {
        $values['name'] = $name;
        $values['enabled'] = $enabled[$name] ? 1 : 0;
        xbbcode_handler_save((object)$values, $format_id);
      }
    }
    drupal_set_message(t('The tag settings were updated.'));
    xbbcode_rebuild_tags($format_id);
  }
  else {
    // If the format doesn't override, remove any specific settings.
    if (xbbcode_handlers_delete_format($format_id)) {
      drupal_set_message(t('The format-specific tag settings were reset.'));
      xbbcode_rebuild_tags($format_id);
    }
  }
}

