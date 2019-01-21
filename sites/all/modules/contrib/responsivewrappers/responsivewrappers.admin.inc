<?php

/**
 * @file
 * Administrative page callbacks for responsive wrappers module.
 */

/**
 * Generate a form for configuring overall responsive wrappers settings.
 *
 * @ingroup forms
 */
function responsivewrappers_admin_settings($form, &$form_state) {
  $form['responsivewrappers_intro'] = array(
    '#markup' => t('<p>This page contains global settings for all responsive wrappers enabled filters. If you are looking for specific filter configuration options check the <a href="@format">format configurations page</a> and select the specific format you would like to configure.</p>', array('@format' => url('admin/config/content/formats'))),
  );
  $form['responsivewrappers_output_version'] = array(
    '#type' => 'select',
    '#title' => t('Bootstrap output version'),
    '#options' => array(
      3 => t('Bootstrap 3'),
      4 => t('Bootstrap 4'),
    ),
    '#default_value' => variable_get('responsivewrappers_output_version', 3),
    '#description' => t('The HTML output for responsive images or tables has changed between Bootstrap 3 and Bootstrap 4. You can choose witch output version you want, for example, bootstrap 3 use img-responsive class and bootstrap 4 img-fluid.'),
  );
  $form['responsivewrappers_add_css'] = array(
    '#type' => 'select',
    '#title' => t('Add responsive wrappers CSS'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#default_value' => variable_get('responsivewrappers_add_css', 0),
    '#description' => t('If you are using a Bootstrap 3/4 theme or subtheme the responsive classes works without any extra CSS, if not you can use this to add the CSS styles needed to work.'),
  );
  $form = system_settings_form($form);

  return $form;
}
