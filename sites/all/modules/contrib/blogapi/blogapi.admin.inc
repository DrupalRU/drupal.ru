<?php

/**
 * BlogAPI admin settings form
 */
function blogapi_admin_settings_form($form, &$form_state) {
  $form = array();

  $form['settings_apis'] = array(
    '#type' => 'fieldset',
    '#title' => t('API configuration'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Default provider for each API.
  foreach (blogapi_get_api_types() as $type) {
    $providers = blogapi_get_info($type);
    $provider_options = array();
    foreach ($providers as $name => $provider) {
      $provider_options[$name] = $provider['name'];
    }

    $form['settings_apis']['blogapi_' . $type . '_default_provider'] = array(
      '#type' => 'radios',
      '#title' => t('Default provider for %type APIs', array('%type' => $type)),
      '#required' => TRUE,
      '#options' => $provider_options,
      '#default_value' => variable_get('blogapi_' . $type . '_default_provider', NULL),
      '#description' => t('Select the default provider for %type API clients. This may affect client configuration.', array('%type' => $type)),
    );
  }

  $form['settings_categories'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields for blog categories'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

//  // Set Taxonomy term reference field for BlogAPI categories
//  $fields = array();
//  foreach (field_info_fields() as $field) {
//    if ($field['type'] == 'taxonomy_term_reference') {
//      $fields[$field['field_name']] = $field['field_name'];
//    }
//  }
//
//  $form['settings_categories']['blogapi_category_field'] = array(
//    '#type' => 'radios',
//    '#title' => t('Taxonomy term field for blog categories'),
//    '#options' => $fields,
//    '#default_value' => variable_get('blogapi_category_field', (!empty($fields['field_tags']) ? 'field_tags' : '')),
//    '#description' => t('This field will be used for storing new terms created by mt.setPostCategories'),
//  );

  $form['settings_node_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node types'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Set up BlogAPI node types.
  $node_types = array_map('check_plain', node_type_get_names());
  $defaults = isset($node_types['article']) ? array('article' => 'article'): array();
  $form['settings_node_types']['blogapi_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable for external blogging clients'),
    '#required' => TRUE,
    '#default_value' => variable_get('blogapi_node_types', $defaults),
    '#options' => $node_types,
    '#description' => t('Select the content types available to external blogging clients via Blog API. If supported, each enabled content type will be displayed as a separate "blog" by the external client.'),
  );

  // $form['settings_formats'] = array(
  //   '#type' => 'fieldset',
  //   '#title' => t('Text filters'),
  //   '#collapsible' => FALSE,
  //   '#collapsed' => FALSE,
  // );

  // $formats = filter_formats();
  // $format_list = array();
  // foreach ($formats as $format) {
  //   $format_list[$format->format] = $format->name;
  // }

  // $form['settings_formats']['blogapi_default_format'] = array(
  //   '#type' => 'select',
  //   '#title' => t('Choose the input format for remote posts'),
  //   '#required' => TRUE,
  //   '#default_value' => variable_get('blogapi_default_format', filter_fallback_format()),
  //   '#options' => $format_list,
  //   '#description' => t('The user posting must have permissions for this format, otherwise the standard (anonymous) format is used.')
  // );

  $file_handling = FALSE;
  foreach (blogapi_get_api_types() as $type) {
    $providers = blogapi_get_info($type);

    foreach ($providers as $provider) {
      if (isset($provider['provides_file_handling']) && $provider['provides_file_handling'] === TRUE) {
        $file_handling = TRUE;
        break;
      }
    }
    if ($file_handling == TRUE) {
      break;
    }
  }

  // Only display the file options if we have a module that provides file
  // handling.
  if ($file_handling) {
    $form['settings_files'] = array(
      '#type' => 'fieldset',
      '#title' => t('File settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $blogapi_extensions_default = variable_get('blogapi_extensions_default', 'jpg jpeg gif png txt doc xls pdf ppt pps odt ods odp');
    $blogapi_uploadsize_default = variable_get('blogapi_uploadsize_default', 1);
    $blogapi_usersize_default = variable_get('blogapi_usersize_default', 1);

    $form['settings_files']['blogapi_extensions_default'] = array(
      '#type' => 'textfield',
      '#title' => t('Default permitted file extensions'),
      '#default_value' => $blogapi_extensions_default,
      '#maxlength' => 255,
      '#description' => t('Default extensions that users can upload. Separate extensions with a space and do not include the leading dot.'),
    );

    $form['settings_files']['blogapi_uploadsize_default'] = array(
      '#type' => 'textfield',
      '#title' => t('Default maximum file size per upload'),
      '#default_value' => $blogapi_uploadsize_default,
      '#size' => 5,
      '#maxlength' => 5,
      '#description' => t('The default maximum file size a user can upload.'),
      '#field_suffix' => t('MB'),
    );

    $form['settings_files']['blogapi_usersize_default'] = array(
      '#type' => 'textfield',
      '#title' => t('Default total file size per user'),
      '#default_value' => $blogapi_usersize_default,
      '#size' => 5,
      '#maxlength' => 5,
      '#description' => t('The default maximum size of all files a user can have on the site.'),
      '#field_suffix' => t('MB'),
    );

    $form['settings_files']['upload_max_size'] = array(
      '#prefix' => '<p>',
      '#value' => t('Your PHP settings limit the maximum file size per upload to %size.', array('%size' => format_size(file_upload_max_size()))),
      '#suffix' => '</p>',
    );

    $roles = user_roles(FALSE, 'manage content with blogapi');
    foreach ($roles as $rid => $role) {
      $form['settings_files']['settings_role_' . $rid] = array(
        '#type' => 'fieldset',
        '#title' => t('Settings for @role role', array('@role' => $role)),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['settings_files']['settings_role_' . $rid]['blogapi_extensions_' . $rid] = array(
        '#type' => 'textfield',
        '#title' => t('Permitted file extensions'),
        '#default_value' => variable_get('blogapi_extensions_' . $rid, $blogapi_extensions_default),
        '#maxlength' => 255,
        '#description' => t('Extensions that users in this role can upload. Separate extensions with a space and do not include the leading dot.'),
      );
      $form['settings_files']['settings_role_' . $rid]['blogapi_uploadsize_' . $rid] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum file size per upload'),
        '#default_value' => variable_get('blogapi_uploadsize_' . $rid, $blogapi_uploadsize_default),
        '#size' => 5,
        '#maxlength' => 5,
        '#description' => t('The maximum size of a file a user can upload (in megabytes).'),
        '#field_suffix' => t('MB'),
      );
      $form['settings_files']['settings_role_' . $rid]['blogapi_usersize_' . $rid] = array(
        '#type' => 'textfield',
        '#title' => t('Total file size per user'),
        '#default_value' => variable_get('blogapi_usersize_' . $rid, $blogapi_usersize_default),
        '#size' => 5,
        '#maxlength' => 5,
        '#description' => t('The maximum size of all files a user can have on the site (in megabytes).'),
        '#field_suffix' => t('MB'),
      );
    }
  }

  return system_settings_form($form);
}