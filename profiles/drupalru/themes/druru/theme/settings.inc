<?php
/**
 * @file
 * settings.inc
 */

/**
 * Contains the form for the theme settings.
 *
 * @param array $form
 *   The form array, passed by reference.
 * @param array $form_state
 *   The form state array, passed by reference.
 */
function _druru_settings_form(&$form, $form_state) {
  // Do not add Bootstrap specific settings to non-druru based themes.
  $theme = !empty($form_state['build_info']['args'][0]) ? $form_state['build_info']['args'][0] : FALSE;
  // Global settings.
  if ($theme === FALSE) {
    return;
  }
  // Other theme settings.
  elseif ($theme !== 'druru') {
    $base_theme = druru_get_theme_info($theme, 'base theme');
    if (!$base_theme || (!is_array($base_theme) && $base_theme !== 'druru') || (is_array($base_theme) && !in_array('druru', $base_theme))) {
      return;
    }
  }

  // Display a warning if jquery_update isn't enabled.
  if ((!module_exists('jquery_update') || !version_compare(variable_get('jquery_update_jquery_version', 0), 1.7, '>=')) && !theme_get_setting('druru_toggle_jquery_error')) {
    drupal_set_message(t('jQuery Update is not enabled, Bootstrap requires a minimum jQuery version of 1.7 or higher.<br/>Please enable <a href="https://drupal.org/project/jquery_update">jQuery Update module</a> 7.x-2.3 or higher, you must manually set this in the configuration after it is installed.'), 'error');
  }

  // Wrap global setting fieldsets in vertical tabs.
  $form['general'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<h2><small>' . t('Override Global Settings') . '</small></h2>',
    '#weight' => -9,
  );
  $form['theme_settings']['#group'] = 'general';
  $form['logo']['#group'] = 'general';
  $form['favicon']['#group'] = 'general';

  // Do not add Bootstrap specific settings to global settings.
  if (empty($form_state['build_info']['args'][0])) {
    unset($form['general']['#prefix']);
    return;
  }

  $form['bootstrap'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js'  => array(drupal_get_path('theme', 'druru') . '/js/druru.admin.js'),
    ),
    '#prefix' => '<h2><small>' . t('Bootstrap Settings') . '</small></h2>',
    '#weight' => -10,
  );

  // Components.
  $form['components'] = array(
    '#type' => 'fieldset',
    '#title' => t('Components'),
    '#group' => 'bootstrap',
  );
  // Breadcrumbs.
  $form['components']['breadcrumbs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumbs'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['components']['breadcrumbs']['druru_breadcrumb'] = array(
    '#type' => 'select',
    '#title' => t('Breadcrumb visibility'),
    '#default_value' => theme_get_setting('druru_breadcrumb'),
    '#options' => array(
      0 => t('Hidden'),
      1 => t('Visible'),
      2 => t('Only in admin areas'),
    ),
  );
  $form['components']['breadcrumbs']['druru_breadcrumb_home'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show "Home" breadcrumb link'),
    '#default_value' => theme_get_setting('druru_breadcrumb_home'),
    '#description' => t('If your site has a module dedicated to handling breadcrumbs already, ensure this setting is enabled.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="druru_breadcrumb"]' => array('value' => 0),
      ),
    ),
  );
  $form['components']['breadcrumbs']['druru_breadcrumb_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show current page title at end'),
    '#default_value' => theme_get_setting('druru_breadcrumb_title'),
    '#description' => t('If your site has a module dedicated to handling breadcrumbs already, ensure this setting is disabled.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="druru_breadcrumb"]' => array('value' => 0),
      ),
    ),
  );
  // Navbar.
  $form['components']['navbar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navbar'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['components']['navbar']['druru_navbar_position'] = array(
    '#type' => 'select',
    '#title' => t('Navbar Position'),
    '#description' => t('Select your Navbar position.'),
    '#default_value' => theme_get_setting('druru_navbar_position'),
    '#options' => array(
      'static-top' => t('Static Top'),
      'fixed-top' => t('Fixed Top'),
      'fixed-bottom' => t('Fixed Bottom'),
    ),
    '#empty_option' => t('Normal'),
  );
  $form['components']['navbar']['druru_navbar_inverse'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inverse navbar style'),
    '#description' => t('Select if you want the inverse navbar style.'),
    '#default_value' => theme_get_setting('druru_navbar_inverse'),
  );
  // Icons.
  $form['components']['icons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Icons'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  if ($icon_libraries = druru_get_libraries_list()) {
    $form['components']['icons']['druru_iconize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Iconize'),
      '#description' => t('Select if you want to iconize buttons and some links.'),
      '#default_value' => theme_get_setting('druru_iconize'),
    );
    $form['components']['icons']['druru_icons_library'] = array(
      '#type' => 'select',
      '#title' => t('Iconize'),
      '#description' => t('Select your library of icons.'),
      '#default_value' => theme_get_setting('druru_icons_library'),
      '#options' => $icon_libraries,
      '#empty_value' => '',
      '#states' => array(
        'invisible' => array(
          ':input[name="druru_iconize"]' => array(
            'checked' => FALSE,
          ),
        ),
      ),
    );
  }

  // Region wells.
  $wells = array(
    '' => t('None'),
    'well' => t('.well (normal)'),
    'well well-sm' => t('.well-sm (small)'),
    'well well-lg' => t('.well-lg (large)'),
  );
  $form['components']['region_wells'] = array(
    '#type' => 'fieldset',
    '#title' => t('Region wells'),
    '#description' => t('Enable the <code>.well</code>, <code>.well-sm</code> or <code>.well-lg</code> classes for specified regions. See: documentation on !wells.', array(
      '!wells' => l(t('Bootstrap Wells'), 'http://getbootstrap.com/components/#wells'),
    )),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  foreach (system_region_list($theme) as $name => $title) {
    $form['components']['region_wells']['druru_region_well-' . $name] = array(
      '#title' => $title,
      '#type' => 'select',
      '#attributes' => array(
        'class' => array('input-sm'),
      ),
      '#options' => $wells,
      '#default_value' => theme_get_setting('druru_region_well-' . $name),
    );
  }

  // JavaScript settings.
  $form['javascript'] = array(
    '#type' => 'fieldset',
    '#title' => t('JavaScript'),
    '#group' => 'bootstrap',
  );
  // Anchors.
  $form['javascript']['anchors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Anchors'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['javascript']['anchors']['druru_anchors_fix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fix anchor positions'),
    '#default_value' => theme_get_setting('druru_anchors_fix'),
    '#description' => t('Ensures anchors are correctly positioned only when there is margin or padding detected on the BODY element. This is useful when fixed navbar or administration menus are used.'),
  );
  $form['javascript']['anchors']['druru_anchors_smooth_scrolling'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable smooth scrolling'),
    '#default_value' => theme_get_setting('druru_anchors_smooth_scrolling'),
    '#description' => t('Animates page by scrolling to an anchor link target smoothly when clicked.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="druru_anchors_fix"]' => array('checked' => FALSE),
      ),
    ),
  );
  // Popovers.
  $form['javascript']['popovers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Popovers'),
    '#description' => t('Add small overlays of content, like those on the iPad, to any element for housing secondary information.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['javascript']['popovers']['druru_popover_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable popovers.'),
    '#description' => t('Elements that have the !code attribute set will automatically initialize the popover upon page load. !warning', array(
      '!code' => '<code>data-toggle="popover"</code>',
      '!warning' => '<strong class="error text-error">WARNING: This feature can sometimes impact performance. Disable if pages appear to "hang" after initial load.</strong>',
    )),
    '#default_value' => theme_get_setting('druru_popover_enabled'),
  );
  $form['javascript']['popovers']['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#description' => t('These are global options. Each popover can independently override desired settings by appending the option name to !data. Example: !data-animation.', array(
      '!data' => '<code>data-</code>',
      '!data-animation' => '<code>data-animation="false"</code>',
    )),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="druru_popover_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['javascript']['popovers']['options']['druru_popover_animation'] = array(
    '#type' => 'checkbox',
    '#title' => t('animate'),
    '#description' => t('Apply a CSS fade transition to the popover.'),
    '#default_value' => theme_get_setting('druru_popover_animation'),
  );
  $form['javascript']['popovers']['options']['druru_popover_html'] = array(
    '#type' => 'checkbox',
    '#title' => t('HTML'),
    '#description' => t("Insert HTML into the popover. If false, jQuery's text method will be used to insert content into the DOM. Use text if you're worried about XSS attacks."),
    '#default_value' => theme_get_setting('druru_popover_html'),
  );
  $form['javascript']['popovers']['options']['druru_popover_placement'] = array(
    '#type' => 'select',
    '#title' => t('placement'),
    '#description' => t('Where to position the popover. When "auto" is specified, it will dynamically reorient the popover. For example, if placement is "auto left", the popover will display to the left when possible, otherwise it will display right.'),
    '#default_value' => theme_get_setting('druru_popover_placement'),
    '#options' => drupal_map_assoc(array(
      'top',
      'bottom',
      'left',
      'right',
      'auto',
      'auto top',
      'auto bottom',
      'auto left',
      'auto right',
    )),
  );
  $form['javascript']['popovers']['options']['druru_popover_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('selector'),
    '#description' => t('If a selector is provided, tooltip objects will be delegated to the specified targets. In practice, this is used to enable dynamic HTML content to have popovers added. See !this and !example.', array(
      '!this' => l(t('this'), 'https://github.com/twbs/bootstrap/issues/4215'),
      '!example' => l(t('an informative example'), 'http://jsfiddle.net/fScua/'),
    )),
    '#default_value' => theme_get_setting('druru_popover_selector'),
  );
  $form['javascript']['popovers']['options']['druru_popover_trigger'] = array(
    '#type' => 'checkboxes',
    '#title' => t('trigger'),
    '#description' => t('How a popover is triggered.'),
    '#default_value' => theme_get_setting('druru_popover_trigger'),
    '#options' => drupal_map_assoc(array(
      'click',
      'hover',
      'focus',
      'manual',
    )),
  );
  $form['javascript']['popovers']['options']['druru_popover_title'] = array(
    '#type' => 'textfield',
    '#title' => t('title'),
    '#description' => t("Default title value if \"title\" attribute isn't present."),
    '#default_value' => theme_get_setting('druru_popover_title'),
  );
  $form['javascript']['popovers']['options']['druru_popover_content'] = array(
    '#type' => 'textfield',
    '#title' => t('content'),
    '#description' => t("Default content value if \"data-content\" attribute isn't present."),
    '#default_value' => theme_get_setting('druru_popover_content'),
  );
  $form['javascript']['popovers']['options']['druru_popover_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('delay'),
    '#description' => t('The amount of time to delay showing and hiding the popover (in milliseconds). Does not apply to manual trigger type.'),
    '#default_value' => theme_get_setting('druru_popover_delay'),
  );
  $form['javascript']['popovers']['options']['druru_popover_container'] = array(
    '#type' => 'textfield',
    '#title' => t('container'),
    '#description' => t('Appends the popover to a specific element. Example: "body". This option is particularly useful in that it allows you to position the popover in the flow of the document near the triggering element - which will prevent the popover from floating away from the triggering element during a window resize.'),
    '#default_value' => theme_get_setting('druru_popover_container'),
  );
  // Tooltips.
  $form['javascript']['tooltips'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tooltips'),
    '#description' => t("Inspired by the excellent jQuery.tipsy plugin written by Jason Frame; Tooltips are an updated version, which don't rely on images, use CSS3 for animations, and data-attributes for local title storage. See !link for more documentation.", array(
      '!link' => l(t('Bootstrap tooltips'), 'http://getbootstrap.com/javascript/#tooltips'),
    )),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['javascript']['tooltips']['druru_tooltip_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable tooltips'),
    '#description' => t('Elements that have the !code attribute set will automatically initialize the tooltip upon page load. !warning', array(
      '!code' => '<code>data-toggle="tooltip"</code>',
      '!warning' => '<strong class="error text-error">WARNING: This feature can sometimes impact performance. Disable if pages appear to "hang" after initial load.</strong>',
    )),
    '#default_value' => theme_get_setting('druru_tooltip_enabled'),
  );
  $form['javascript']['tooltips']['druru_tooltip_descriptions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Convert simple form descriptions'),
    '#description' => t('Form items that contain simple descriptions (no HTML and shorter than 200 characters) will be converted into tooltips. This helps reduce the sometimes unnecessary noise of form item descriptions. By treating descriptions that contain longer text or HTML as actionable descriptions, we ensure it is always visible so its usability remains intact.'),
    '#default_value' => theme_get_setting('druru_tooltip_descriptions'),
    '#states' => array(
      'visible' => array(
        ':input[name="druru_tooltip_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['javascript']['tooltips']['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#description' => t('These are global options. Each tooltip can independently override desired settings by appending the option name to !data. Example: !data-animation.', array(
      '!data' => '<code>data-</code>',
      '!data-animation' => '<code>data-animation="false"</code>',
    )),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="druru_tooltip_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['javascript']['tooltips']['options']['druru_tooltip_animation'] = array(
    '#type' => 'checkbox',
    '#title' => t('animate'),
    '#description' => t('Apply a CSS fade transition to the tooltip.'),
    '#default_value' => theme_get_setting('druru_tooltip_animation'),
  );
  $form['javascript']['tooltips']['options']['druru_tooltip_html'] = array(
    '#type' => 'checkbox',
    '#title' => t('HTML'),
    '#description' => t("Insert HTML into the tooltip. If false, jQuery's text method will be used to insert content into the DOM. Use text if you're worried about XSS attacks."),
    '#default_value' => theme_get_setting('druru_tooltip_html'),
  );
  $form['javascript']['tooltips']['options']['druru_tooltip_placement'] = array(
    '#type' => 'select',
    '#title' => t('placement'),
    '#description' => t('Where to position the tooltip. When "auto" is specified, it will dynamically reorient the tooltip. For example, if placement is "auto left", the tooltip will display to the left when possible, otherwise it will display right.'),
    '#default_value' => theme_get_setting('druru_tooltip_placement'),
    '#options' => drupal_map_assoc(array(
      'top',
      'bottom',
      'left',
      'right',
      'auto',
      'auto top',
      'auto bottom',
      'auto left',
      'auto right',
    )),
  );
  $form['javascript']['tooltips']['options']['druru_tooltip_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('selector'),
    '#description' => t('If a selector is provided, tooltip objects will be delegated to the specified targets.'),
    '#default_value' => theme_get_setting('druru_tooltip_selector'),
  );
  $form['javascript']['tooltips']['options']['druru_tooltip_trigger'] = array(
    '#type' => 'checkboxes',
    '#title' => t('trigger'),
    '#description' => t('How a tooltip is triggered.'),
    '#default_value' => theme_get_setting('druru_tooltip_trigger'),
    '#options' => drupal_map_assoc(array(
      'click',
      'hover',
      'focus',
      'manual',
    )),
  );
  $form['javascript']['tooltips']['options']['druru_tooltip_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('delay'),
    '#description' => t('The amount of time to delay showing and hiding' .
      ' the tooltip (in milliseconds). Does not apply to manual trigger type.'),
    '#default_value' => theme_get_setting('druru_tooltip_delay'),
  );
  $form['javascript']['tooltips']['options']['druru_tooltip_container'] = array(
    '#type' => 'textfield',
    '#title' => t('container'),
    '#description' => t('Appends the tooltip to a specific element. Example: "body"'),
    '#default_value' => theme_get_setting('druru_tooltip_container'),
  );

  // JavaScript settings.
  $form['forum'] = array(
    '#type' => 'fieldset',
    '#title' => t('Forum'),
    '#group' => 'bootstrap',
  );
  $icons = array();
  foreach (theme_get_setting('forum_icons') as $path => $icon) {
    $icons[] = $path.'|'.$icon;
  }
  $form['forum']['forum_icons'] = array(
    '#type'             => 'textarea',
    '#title'            => t('Forum icons', array(), array('context' => 'drurum')),
    '#description'      => t('Here you can add icons for forums at page with list of forum links and in title at forum page.'
      . ' You need to know next points: '
      . theme('item_list', array(
        'items' => array(
          'You can use format "path|icon-name" per each line.',
          'You need to use relative path like that "alias-of-any-forum".',
          'Each row it\'s separated rule.',
          'You need to wrie clean icons name like "user" for "fa fa-user" or "glyphicon glyphicon-user".',
        ),
      ))),
    '#element_validate' => array('druru_forum_icons_validate'),
    '#default_value'    => implode(PHP_EOL, $icons),
  );

  // Advanced settings.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#group' => 'bootstrap',
  );
  // BootstrapCDN.
  $form['advanced']['druru_bootstrap_cdn'] = array(
    '#type' => 'fieldset',
    '#title' => t('BootstrapCDN'),
    '#description' => t('Use !bootstrapcdn to serve the Bootstrap framework' .
      ' files. Enabling this setting will prevent this theme from attempting' .
      ' to load any Bootstrap framework files locally. !warning', array(
      '!bootstrapcdn' => l(t('BootstrapCDN'), 'http://bootstrapcdn.com', array(
        'external' => TRUE,
      )),
      '!warning' => '<div class="alert alert-info messages info"><strong>'
        . t('NOTE')
        . ':</strong> '
        . t('While BootstrapCDN (content distribution network) is the' .
          ' preferred method for providing huge performance gains in ' .
          'load time, this method does depend on using this third party ' .
          'service. BootstrapCDN is under no obligation or commitment to' .
          ' provide guaranteed up-time or service quality for this theme.' .
          ' If you choose to disable this setting, you must provide your' .
          ' own Bootstrap source and/or optional CDN delivery implementation.')
        . '</div>',
    )),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['druru_bootstrap_cdn']['druru_bootstrap_cdn'] = array(
    '#type' => 'select',
    '#title' => t('BootstrapCDN version'),
    '#options' => drupal_map_assoc(array(
      '3.0.0',
      '3.0.1',
      '3.0.2',
    )),
    '#default_value' => theme_get_setting('druru_bootstrap_cdn'),
    '#empty_option' => t('Disabled'),
    '#empty_value' => NULL,
  );
  // Bootswatch.
  $bootswatch_themes = array();
  $request = drupal_http_request('http://api.bootswatch.com/3/');
  if ($request && $request->code === '200' && !empty($request->data)) {
    if (($api = drupal_json_decode($request->data)) && is_array($api) && !empty($api['themes'])) {
      foreach ($api['themes'] as $bootswatch_theme) {
        $bootswatch_themes[strtolower($bootswatch_theme['name'])] = $bootswatch_theme['name'];
      }
    }
  }
  $form['advanced']['druru_bootstrap_cdn']['druru_bootswatch'] = array(
    '#type' => 'select',
    '#title' => t('Bootswatch theme'),
    '#description' => t('Use !bootstrapcdn to serve a Bootswatch Theme. Choose Bootswatch theme here.', array(
      '!bootstrapcdn' => l(t('BootstrapCDN'), 'http://bootstrapcdn.com', array(
        'external' => TRUE,
      )),
    )),
    '#default_value' => theme_get_setting('druru_bootswatch'),
    '#options' => $bootswatch_themes,
    '#empty_option' => t('Disabled'),
    '#empty_value' => NULL,
    '#suffix' => '<div id="bootswatch-preview"></div>',
    '#states' => array(
      'invisible' => array(
        ':input[name="druru_bootstrap_cdn"]' => array('value' => ''),
      ),
    ),
  );
  if (empty($bootswatch_themes)) {
    $form['advanced']['druru_bootstrap_cdn']['druru_bootswatch']['#prefix'] = '<div class="alert alert-danger messages error"><strong>' . t('ERROR') . ':</strong> ' . t('Unable to reach Bootswatch API. Please ensure the server your website is hosted on is able to initiate HTTP requests.') . '</div>';
  }
  // Rebuild registry.
  $form['advanced']['druru_rebuild_registry'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rebuild theme registry on every page.'),
    '#default_value' => theme_get_setting('druru_rebuild_registry'),
    '#description' => t('During theme development, it can be very useful to continuously !rebuild. !warning', array(
      '!rebuild' => l(t('rebuild the theme registry'), 'http://drupal.org/node/173880#theme-registry'),
      '!warning' => '<div class="alert alert-warning messages warning"><strong>' . t('WARNING') . ':</strong> ' . t('This is a huge performance penalty and must be turned off on production websites. ') . '</div>',
    )),
  );
  // Suppress jQuery message.
  $form['advanced']['druru_toggle_jquery_error'] = array(
    '#type' => 'checkbox',
    '#title' => t('Suppress jQuery version error message'),
    '#default_value' => theme_get_setting('druru_toggle_jquery_error'),
    '#description' => t('Enable this if the version of jQuery has been upgraded to 1.7+ using a method other than the !jquery_update module.', array(
      '!jquery_update' => l(t('jQuery Update'), 'https://drupal.org/project/jquery_update'),
    )),
  );

  // Add support of upload SVG logo.
  druru_svg_logo_form($form, $form_state);
}

/**
 * Generate part of theme settings form which add ability to upload SVG logo.
 */
function druru_svg_logo_form(&$form, $form_state) {
  $theme = !empty($form_state['build_info']['args'][0]) ? $form_state['build_info']['args'][0] : FALSE;
  $path = theme_get_setting('svg_logo_path', $theme);

  // Weight for the right order.
  // Add back to form.
  $form['logo']['settings']['svg_logo'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use a svg logo'),
    '#description'   => t('Check here if you want the theme to use a svg logo if the browser supports it.'),
    '#default_value' => theme_get_setting('svg_logo', $theme),
    '#weight'        => -1,
  );
  $lpath = &$form['logo']['settings']['logo_path'];
  $lupload = &$form['logo']['settings']['logo_upload'];
  $lpath['#states']['invisible']['input[name="svg_logo"]']['checked']   = TRUE;
  $lupload['#states']['invisible']['input[name="svg_logo"]']['checked'] = TRUE;

  // Add container.
  $form['logo']['settings']['svg'] = array(
    '#type'   => 'container',
    '#states' => array(
      // Hide the logo settings when using the default logo.
      'invisible' => array(
        'input[name="svg_logo"]' => array('checked' => FALSE),
      ),
    ),
    '#weight' => 1,
  );

  $form['logo']['settings']['svg']['svg_logo_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Path to custom svg logo'),
    '#description'   => t('The path to the file you would like to use as your svg logo file instead of the default logo.'),
    '#default_value' => $path,
  );

  $form['logo']['settings']['svg']['svg_logo_upload'] = array(
    '#type'        => 'file',
    '#title'       => t('Upload logo svg image'),
    '#description' => t("If you don't have direct file access to the server, use this field to upload your svg logo."),
    '#maxlength'   => 40,
  );

  $form['logo']['logo_height'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Logo height'),
    '#description'   => t('The height of logo in pixels.'),
    '#default_value' => theme_get_setting('logo_height', $theme),
  );

  $form['logo']['logo_width'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Logo width'),
    '#description'   => t('The width of logo in pixels.'),
    '#default_value' => theme_get_setting('logo_width', $theme),
  );

  // Add validator and submit function for svg path or upload.
  $form['#validate'][] = 'druru_svg_logo_form_validate';
  $form['#submit'][]   = 'druru_svg_logo_form_submit';
}

/**
 * Implements hook_form_validate() for druru_svg_logo_form().
 */
function druru_svg_logo_form_validate($form, &$form_state) {
  // Handle file uploads.
  $validators = array('file_validate_extensions' => array('svg'));

  // Check for a new uploaded logo.
  $file = file_save_upload('svg_logo_upload', $validators);
  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values']['svg_logo_upload'] = $file;
    }
    else {
      // File upload failed.
      form_set_error('svg_logo_upload', t('The svg logo could not be uploaded.'));
    }
  }

  // If the user provided a path for a logo or favicon file, make sure a file
  // exists at that path.
  if ($form_state['values']['svg_logo_path']) {
    $path = _system_theme_settings_validate_path($form_state['values']['svg_logo_path']);
    if (!$path) {
      form_set_error('svg_logo_path', t('The custom svg logo path is invalid.'));
    }
  }
}

/**
 * Implements hook_form_submit() for druru_svg_logo_form().
 */
function druru_svg_logo_form_submit($form, &$form_state) {
  $values = &$form_state['values'];

  // If the user uploaded a new logo or favicon, save it to a permanent location
  // and use it in place of the default theme-provided file.
  if ($file = $values['svg_logo_upload']) {
    unset($values['svg_logo_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $values['svg_logo']      = !!$filename;
    $values['svg_logo_path'] = $filename;
  }
  $logo = $values['svg_logo_path'];
  // If the user entered a path relative to the system files directory for
  // a logo, store a public:// URI so the theme system can handle it.
  // A path relative to the Drupal root or a fully qualified URI is valid.
  if (empty($logo) || drupal_realpath($logo) == $logo || !is_file($logo)) {
    $logo = NULL;
  }

  // Prepend 'public://' for relative file paths within public filesystem.
  if (file_uri_scheme($logo) === FALSE) {
    $logo = 'public://' . $logo;
  }

  if (!is_file($logo)) {
    $logo = NULL;
  }

  $values['svg_logo_path'] = $logo;
}

function druru_forum_icons_validate($element, &$form_state, $form) {
  if (!empty($form_state['values']['forum_icons'])) {
    $rules = explode(PHP_EOL, $form_state['values']['forum_icons']);
    $bad_rows = array();
    $rows = array();
    // Validate rules. Each rule (row) must have pipeline.
    foreach ($rules as $row => $rule) {
      if (strpos($rule, '|') === FALSE) {
        $bad_rows[] = $row + 1;
      }
      else {
        $rule = explode('|', $rule);
        // If row has a few pipelines '|'.
        $rows[array_shift($rule)] = implode('|', $rule);
      }
    }

    // If some rows has no pipeline, show error message.
    if ($bad_rows) {
      form_set_error('forum_icons', t('You have bad rules in next rows: @rows', array(
        '@rows' => implode(', ', $bad_rows),
      )));
    }
    else {
      $form_state['values']['forum_icons'] = $rows;
    }
  }
}
