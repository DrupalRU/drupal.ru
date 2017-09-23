<?php
/**
 * @file
 * template.php
 */

/**
 * Implements hook_css_alter().
 */
function bootstrap_lite_css_alter(&$css) {
  $theme_path = drupal_get_path('theme', 'bootstrap_lite');

  if ($bootstrap_cdn = theme_get_setting('bootstrap_lite_cdn')) {
    // Add CDN.
    if ($bootswatch = theme_get_setting('bootstrap_lite_bootswatch')) {
      $cdn = '//netdna.bootstrapcdn.com/bootswatch/' . $bootstrap_cdn  . '/' . $bootswatch . '/bootstrap.min.css';
    }
    else {
      $cdn = '//netdna.bootstrapcdn.com/bootstrap/' . $bootstrap_cdn  . '/css/bootstrap.min.css';
    }
    $css[$cdn] = array(
      'data' => $cdn,
      'type' => 'external',
      'every_page' => TRUE,
      'media' => 'all',
      'preprocess' => FALSE,
      'group' => CSS_THEME,
      'browsers' => array('IE' => TRUE, '!IE' => TRUE),
      'weight' => -2,
    );
    // Add overrides.
    $override = $theme_path . '/css/overrides.css';
    $css[$override] = array(
      'data' => $override,
      'type' => 'file',
      'every_page' => TRUE,
      'media' => 'all',
      'preprocess' => TRUE,
      'group' => CSS_THEME,
      'browsers' => array('IE' => TRUE, '!IE' => TRUE),
      'weight' => -1,
    );
  }
  if ($font_awesome = theme_get_setting('bootstrap_lite_font_awesome')) {
    $awesome = 'https://maxcdn.bootstrapcdn.com/font-awesome/' . $font_awesome . '/css/font-awesome.min.css';
    $css[$awesome] = array(
      'data' => $awesome,
      'type' => 'external',
      'every_page' => TRUE,
      'media' => 'all',
      'preprocess' => FALSE,
      'group' => CSS_THEME,
      'browsers' => array('IE' => TRUE, '!IE' => TRUE),
      'weight' => -2,
    );
  }

}

/**
 * Implements hook_js_alter().
 */
function bootstrap_lite_js_alter(&$js) {
  if (theme_get_setting('bootstrap_lite_cdn')) {
    $cdn = '//netdna.bootstrapcdn.com/bootstrap/' . theme_get_setting('bootstrap_lite_cdn')  . '/js/bootstrap.min.js';
    $js[$cdn] = drupal_js_defaults();
    $js[$cdn]['data'] = $cdn;
    $js[$cdn]['type'] = 'external';
    $js[$cdn]['every_page'] = TRUE;
    $js[$cdn]['weight'] = -100;
  }
  if('fixed-top' == theme_get_setting('bootstrap_lite_navbar_position')){
    drupal_add_js('var themeTableHeaderOffset = function() { var offsetheight = jQuery("#navbar").height(); return offsetheight; }', 'inline');
    drupal_add_js(array('tableHeaderOffset' => 'themeTableHeaderOffset'), 'setting');
  }
}

/**
 * Implements hook_preprocess_html().
 */
function bootstrap_lite_preprocess_html(&$variables) {
  if($navbar_position = theme_get_setting('bootstrap_lite_navbar_position')){
    $variables['classes_array'][] = 'navbar-is-' . $navbar_position;
  }
/*    if($navbar_position == 'fixed-top' && user_access('access administration bar') && !admin_bar_suppress(FALSE) && !$config->get('position_fixed') ){
      drupal_add_js(drupal_get_path('theme', 'bootstrap_lite') . '/js/navbar-fixed-top.js');
    }
    if($navbar_position == 'static-top'){
      drupal_add_js(drupal_get_path('theme', 'bootstrap_lite') . '/js/navbar-static-top.js');
    }*/

}


/**
 * Implements hook_preprocess_page().
 */
function bootstrap_lite_preprocess_page(&$variables){
  $no_old_ie_compatibility_modes = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'X-UA-Compatible',
      'content' => 'IE=edge',
    ),
  );
  drupal_add_html_head($no_old_ie_compatibility_modes, 'no_old_ie_compatibility_modes');

  $variables['navbar_classes_array'] = array('navbar');
  if($navbar_position = theme_get_setting('bootstrap_lite_navbar_position'))
  {
    $variables['navbar_classes_array'][] = 'navbar-' . $navbar_position;
  }

  $variables['container_class'] = theme_get_setting('bootstrap_lite_container');

  if (theme_get_setting('bootstrap_lite_navbar_inverse')) {
    $variables['navbar_classes_array'][] = 'navbar-inverse';
  }
  else {
    $variables['navbar_classes_array'][] = 'navbar-default';
  }

  if (module_exists('toolbar')) {
    if (user_access('Use the administration toolbar')) {
      $variables['classes'][] = 'navbar-admin-bar';
    }
  }

  // Primary nav.
  $variables['primary_nav'] = FALSE;
  if ($variables['main_menu']) {
    // Build links.
    $variables['primary_nav'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    // Provide default theme wrapper function.
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__system_primary_menu');
  }

  // Secondary nav.
  $variables['secondary_nav'] = FALSE;
  if ($variables['secondary_menu']) {
    // Build links.
    $variables['secondary_nav'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    // Provide default theme wrapper function.
    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__system_secondary_menu');
  }

  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-sm-6"';
  }
  elseif (!empty($variables['page']['sidebar_first']) || !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-sm-9"';
  }
  else {
    $variables['content_column_class'] = ' class="col-sm-12"';
  }

}


/**
 * Implements hook_process_page().
 *
 * @see page.tpl.php
 */
function bootstrap_lite_process_page(&$variables) {
  $variables['navbar_classes'] = implode(' ', $variables['navbar_classes_array']);
}

/**
 * Returns HTML for a fieldset form element and its children.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #attributes, #children, #collapsed, #collapsible,
 *     #description, #id, #title, #value.
 *
 * @ingroup themeable
 */
function bootstrap_lite_fieldset($variables) {
  if(isset($variables['element']['#group_fieldset']) && !empty($variables['element']['#group_fieldset'])){
    return theme_fieldset($variables);
  }
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));
  $element['#attributes']['class'][] = 'panel';
  $element['#attributes']['class'][] = 'panel-default';
  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';
  if (!empty($element['#title'])) {
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<legend class="panel-heading"><span class="fieldset-legend">' . $element['#title'] . '</span></legend>';
  }
  $output .= '<div class="fieldset-wrapper panel-body">';
  if (!empty($element['#description'])) {
    $output .= '<div class="fieldset-description">' . $element['#description'] . '</div>';
  }
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  $output .= '</div>';
  $output .= "</fieldset>\n";
  return $output;
}

/**
 * Returns HTML for a button form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #attributes, #button_type, #name, #value.
 *
 * @ingroup themeable
 */
function bootstrap_lite_button($variables) {

  if(is_array($variables['element']['#parents'])){
    $count  = count($variables['element']['#parents']);
    $button_type = '';
    if($count > 0){
      $button_type = $variables['element']['#parents'][$count - 1];
    }
  }

  $button_class = 'btn-default';

  switch($button_type){
    case 'submit':
      $button_class = 'btn-primary';
      break;
    case 'update':
      $button_class = 'btn-success';
      break;
    case 'clear':
    case 'check':
      $button_class = 'btn-warning';
      break;
    case 'preview':
      $button_class = 'btn-info';
      break;
    case 'delete':
    case 'cancel':
      $button_class = 'btn-danger';
      break;
  };

  if($variables['element']['#value'] == t('Delete')){
    $button_class = 'btn-danger';
  }

  if($variables['element']['#value'] == t('Remove')){
    $button_class = 'btn-danger';
  }

  $variables['element']['#attributes']['class'][] = 'btn';
  $variables['element']['#attributes']['class'][] = $button_class;


  return theme_button($variables);
}

/**
 * Returns HTML for an email form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #size, #maxlength,
 *     #placeholder, #required, #attributes, #autocomplete_path.
 *
 * @ingroup themeable
 */
function bootstrap_lite_email($variables) {
  $variables['element']['#attributes']['class'][] = 'form-control';
  return theme_email($variables);
}

/**
 * Returns HTML for a textfield form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #size, #maxlength,
 *     #placeholder, #required, #attributes, #autocomplete_path.
 *
 * @ingroup themeable
 */
function bootstrap_lite_textfield($variables) {
  $variables['element']['#attributes']['class'][] = 'form-control';
  return theme_textfield($variables);
}

/**
 * Returns HTML for a textarea form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #rows, #cols,
 *     #placeholder, #required, #attributes
 *
 * @ingroup themeable
 */
function bootstrap_lite_textarea($variables) {
  $variables['element']['#attributes']['class'][] = 'form-control';
  return theme_textarea($variables);
}

/**
 * Returns HTML for a form element.
 *
 * Each form element is wrapped in a DIV container having the following CSS
 * classes:
 * - form-item: Generic for all form elements.
 * - form-type-#type: The internal element #type.
 * - form-item-#name: The internal form element #name (usually derived from the
 *   $form structure and set via form_builder()).
 * - form-disabled: Only set if the form element is #disabled.
 *
 * In addition to the element itself, the DIV contains a label for the element
 * based on the optional #title_display property, and an optional #description.
 *
 * The optional #title_display property can have these values:
 * - before: The label is output before the element. This is the default.
 *   The label includes the #title and the required marker, if #required.
 * - after: The label is output after the element. For example, this is used
 *   for radio and checkbox #type elements as set in system_element_info().
 *   If the #title is empty but the field is #required, the label will
 *   contain only the required marker.
 * - invisible: Labels are critical for screen readers to enable them to
 *   properly navigate through forms but can be visually distracting. This
 *   property hides the label for everyone except screen readers.
 * - attribute: Set the title attribute on the element to create a tooltip
 *   but output no label element. This is supported only for checkboxes
 *   and radios in form_pre_render_conditional_form_element(). It is used
 *   where a visual label is not needed, such as a table of checkboxes where
 *   the row and column provide the context. The tooltip will include the
 *   title and required marker.
 *
 * If the #title property is not set, then the label and any required marker
 * will not be output, regardless of the #title_display or #required values.
 * This can be useful in cases such as the password_confirm element, which
 * creates children elements that have their own labels and required markers,
 * but the parent element should have neither. Use this carefully because a
 * field without an associated label can cause accessibility challenges.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #title_display, #description, #id, #required,
 *     #children, #type, #name.
 *
 * @ingroup themeable
 */
function bootstrap_lite_form_element($variables){
  if(isset($variables['element']['#type'])){
    if($variables['element']['#type'] == 'checkbox'){
      $variables['element']['#wrapper_attributes']['class'][] = 'checkbox';
    }
    if($variables['element']['#type'] == 'radio'){
      $variables['element']['#wrapper_attributes']['class'][] = 'radio';
    }
  }
  $description = FALSE;
  if(isset($variables['element']['#description'])){
    $description = $variables['element']['#description'];
    unset($variables['element']['#description']);
  }
  $output = theme_form_element($variables);
  if($description){
    $output .= '<div class="description help-block">' . $description . "</div>\n";
  }
  return $output;
}

/**
 * Returns HTML for a password form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #size, #maxlength,
 *     #placeholder, #required, #attributes.
 *
 * @ingroup themeable
 */
function bootstrap_lite_password($variables) {
  $variables['element']['#attributes']['class'][] = 'form-control';
  return theme_password($variables);
}

/**
 * Returns HTML for a search form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #size, #maxlength,
 *     #placeholder, #required, #attributes, #autocomplete_path.
 *
 * @ingroup themeable
 */
function bootstrap_lite_search($variables) {

  if(isset($variables['element']['#attributes']['placeholder']) && $variables['element']['#attributes']['placeholder'] == t('Menu search')){
    return theme_search($variables);
  }

  $variables['element']['#attributes']['class'][] = 'form-control';
  return theme_search($variables);
}

/**
 * Returns HTML for a select form element.
 *
 * It is possible to group options together; to do this, change the format of
 * $options to an associative array in which the keys are group labels, and the
 * values are associative arrays in the normal $options format.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #options, #description, #extra,
 *     #multiple, #required, #name, #attributes, #size.
 *
 * @ingroup themeable
 */
function bootstrap_lite_select($variables) {
  if(isset($variables['element']['#size'])){
    unset($variables['element']['#size']);
  }
  $variables['element']['#attributes']['class'][] = 'form-control';
  return theme_select($variables);
}

/**
 * Implements hook_preprocess_table().
 */
function bootstrap_lite_preprocess_table(&$variables) {
  $variables['attributes']['class'][] = 'table';
  $variables['attributes']['class'][] = 'table-hover';
  if (!in_array('table-no-striping', $variables['attributes']['class'])) {
    $variables['attributes']['class'][] = 'table-striped';
  }
}

/**
 * Returns HTML for an individual permission description.
 *
 * @param $variables
 *   An associative array containing:
 *   - permission_item: An associative array representing the permission whose
 *     description is being themed. Useful keys include:
 *     - description: The text of the permission description.
 *     - warning: A security-related warning message about the permission (if
 *       there is one).
 *
 * @ingroup themeable
 */
function bootstrap_lite_user_permission_description($variables) {
  $description = array();
  $permission_item = $variables['permission_item'];
  if (!empty($permission_item['description'])) {
    $description[] = $permission_item['description'];
  }
  if (!empty($permission_item['warning'])) {
    $description[] = '<em class="permission-warning text-danger">' . $permission_item['warning'] . '</em>';
  }
  if (!empty($description)) {
    return implode(' ', $description);
  }
}

/**
 * Returns HTML for an administrative block for display.
 *
 * @param $variables
 *   An associative array containing:
 *   - block: An array containing information about the block:
 *     - show: A Boolean whether to output the block. Defaults to FALSE.
 *     - title: The block's title.
 *     - content: (optional) Formatted content for the block.
 *     - description: (optional) Description of the block. Only output if
 *       'content' is not set.
 *
 * @ingroup themeable
 */
function bootstrap_lite_admin_block($variables) {
  $block = $variables['block'];
  $output = '';

  // Don't display the block if it has no content to display.
  if (empty($block['show'])) {
    return $output;
  }

  $output .= '<div class="panel panel-default">';
  if (!empty($block['title'])) {
    $output .= '<div class="panel-heading"><h3 class="panel-title">' . $block['title'] . '</h3></div>';
  }
  if (!empty($block['content'])) {
    $output .= '<div class="body panel-body">' . $block['content'] . '</div>';
  }
  else {
    $output .= '<div class="description panel-body">' . $block['description'] . '</div>';
  }
  $output .= '</div>';

  return $output;
}

/**
 * Returns HTML for the output of the dashboard page.
 *
 * @param $variables
 *   An associative array containing:
 *   - menu_items: An array of modules to be displayed.
 *
 * @ingroup themeable
 */
function bootstrap_lite_system_admin_index($variables) {
  $menu_items = $variables['menu_items'];

  $stripe = 0;
  $container = array('left' => '', 'right' => '');
  $flip = array('left' => 'right', 'right' => 'left');
  $position = 'left';

  // Iterate over all modules.
  foreach ($menu_items as $module => $block) {
    list($description, $items) = $block;

    // Output links.
    if (count($items)) {
      $block = array();
      $block['title'] = $module;
      $block['content'] = theme('admin_block_content', array('content' => $items));
      $block['description'] = t($description);
      $block['show'] = TRUE;

      if ($block_output = theme('admin_block', array('block' => $block))) {
        if (!isset($block['position'])) {
          // Perform automatic striping.
          $block['position'] = $position;
          $position = $flip[$position];
        }
        $container[$block['position']] .= $block_output;
      }
    }
  }

  $output = '<div class="admin clearfix">';
  foreach ($container as $id => $data) {
    $output .= '<div class=" col-md-6 col-sm-12 clearfix">';
    $output .= $data;
    $output .= '</div>';
  }
  $output .= '</div>';

  return $output;
}

/**
 * Returns HTML for an administrative page.
 *
 * @param $variables
 *   An associative array containing:
 *   - blocks: An array of blocks to display. Each array should include a
 *     'title', a 'description', a formatted 'content' and a 'position' which
 *     will control which container it will be in. This is usually 'left' or
 *     'right'.
 *
 * @ingroup themeable
 */
function bootstrap_lite_admin_page($variables) {
  $blocks = $variables['blocks'];

  $stripe = 0;
  $container = array();

  foreach ($blocks as $block) {
    if ($block_output = theme('admin_block', array('block' => $block))) {
      if (empty($block['position'])) {
        // perform automatic striping.
        $block['position'] = ++$stripe % 2 ? 'left' : 'right';
      }
      if (!isset($container[$block['position']])) {
        $container[$block['position']] = '';
      }
      $container[$block['position']] .= $block_output;
    }
  }

  $output = '<div class="admin clearfix">';

  foreach ($container as $id => $data) {
    $output .= '<div class="clearfix  col-md-6 col-sm-12 ">';
    $output .= $data;
    $output .= '</div>';
  }
  $output .= '</div>';
  return $output;
}

/**
 * Returns HTML for primary and secondary local tasks.
 *
 * @param $variables
 *   An associative array containing:
 *     - primary: (optional) An array of local tasks (tabs).
 *     - secondary: (optional) An array of local tasks (tabs).
 *
 * @ingroup themeable
 * @see menu_local_tasks()
 */
function bootstrap_lite_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="nav nav-tabs tabs-primary">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="nav nav-pills tabs-secondary">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

/**
 * Implements hook_links().
 */
function bootstrap_lite_links__dropbutton($menu){
  foreach($menu['links'] as $name => $settings){
    $menu['links'][$name]['attributes']['class'][] = 'btn';
    $menu['links'][$name]['attributes']['class'][] = 'btn-default';
  }
  return theme_links($menu);
}

/**
 * Returns rendered HTML for the local actions.
 */
function bootstrap_lite_menu_local_actions(&$variables) {
  foreach($variables['actions'] as $key => $link){
    switch($link['#link']['path']){
      case 'admin/people/create':
          $variables['actions'][$key]['#link']['title'] =  '<i class="fa fa-user-plus"></i>' . $link['#link']['title'];
          $variables['actions'][$key]['#link']['options']['html'] = TRUE;
          $variables['actions'][$key]['#link']['localized_options']['html'] = TRUE;
        break;
      default:
          $variables['actions'][$key]['#link']['title'] =  '<i class="fa fa-plus"></i>' . $link['#link']['title'];
          $variables['actions'][$key]['#link']['options']['html'] = TRUE;
          $variables['actions'][$key]['#link']['localized_options']['html'] = TRUE;
    }
  }

  $output = drupal_render($variables['actions']);
  if ($output) {
    $output = '<ul class="nav nav-pills action-links">' . $output . '</ul>';
  }
  return $output;
}

/**
 * Returns HTML for a breadcrumb trail.
 *
 * @param $variables
 *   An associative array containing:
 *   - breadcrumb: An array containing the breadcrumb links.
 */
function bootstrap_lite_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  $output = '';
  if (!empty($breadcrumb)) {
    $output .= '<nav role="navigation">';
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output .= '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $output .= '<ol  class="breadcrumb" >';
    $count = 1;
    foreach($breadcrumb as $item){
      if($count == count($breadcrumb)){
        $output .= '<li class="active">' . $item . '</li>';
      }else{
        $output .= '<li>' . $item . '</li>';
      }
      $count ++;
    }
    $output .= '</ol></nav>';
  }
  return $output;
}


/**
 * Implements hook_preprocess_breadcrumb().
 */
function bootstrap_lite_preprocess_breadcrumb(&$variables) {
  $breadcrumb = &$variables['breadcrumb'];

  // Optionally get rid of the homepage link.
  $show_breadcrumb_home = theme_get_setting('bootstrap_lite_breadcrumb_home');
  if (!$show_breadcrumb_home) {
    array_shift($breadcrumb);
  }
  if (theme_get_setting('bootstrap_lite_breadcrumb_title') && !empty($breadcrumb)) {
    $item = menu_get_item();
    $breadcrumb[] = !empty($item['tab_parent']) ? check_plain($item['title']) : drupal_get_title();
  }
}

/**
 * Returns HTML to wrap child elements in a container.
 *
 * Used for grouped form items. Can also be used as a #theme_wrapper for any
 * renderable element, to surround it with a <div> and add attributes such as
 * classes or an HTML id.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #id, #attributes, #children.
 *
 * @ingroup themeable
 */
function bootstrap_lite_container($variables) {
  if(isset($variables['element']['#attributes']['class'][0]) && $variables['element']['#attributes']['class'][0] == 'views-display-column'){
    $variables['element']['#attributes']['class'] = array('col-xs-12','cols-sm-12', 'col-md-4');
  }
  return theme_container($variables);
}

/**
 * Display a view as a table style.
 */
function bootstrap_lite_preprocess_views_view_table(&$variables) {
  $variables['classes'][] = 'table';
}

/**
 * Implements hook_form_alter().
 */
function bootstrap_lite_form_alter(array &$form, array &$form_state = array(), $form_id = NULL) {
  if ($form_id) {
    if(isset($form['actions']['cancel']) && isset($form['actions']['cancel']['#type']) && $form['actions']['cancel']['#type'] == 'link'){
       $form['actions']['cancel']['#options']['attributes']['class'][] = 'btn';
       $form['actions']['cancel']['#options']['attributes']['class'][] = 'btn-default';
    }
    if(isset($form['actions']['cancel_form']) && $form['actions']['cancel_form']['#type'] == 'link'){
       $form['actions']['cancel']['#options']['attributes']['class'][] = 'btn';
       $form['actions']['cancel']['#options']['attributes']['class'][] = 'btn-default';
    }

  }
}

/**
 * Overrides theme_node_add_list().
 *
 * Display the list of available node types for node creation.
 */
function bootstrap_lite_node_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<ul class="list-group">';
    foreach ($content as $item) {
      $title = '<h4 class="list-group-item-heading">' . $item['title'] . '</h4>';
      if(isset($item['description'])){
        $title .= '<p class="list-group-item-text">' . filter_xss_admin($item['description']) . '</p>';
      }
      $item['localized_options']['attributes']['class'][] = 'list-group-item';
      $item['localized_options']['html'] = TRUE;
      $output .= l($title, $item['href'], $item['localized_options']);
    }
    $output .= '</ul>';
  }
  else {
    $output = '<p>' . t('You have not created any content types yet. Go to the <a href="@create-content">content type creation page</a> to add a new content type.', array('@create-content' => url('admin/structure/types/add'))) . '</p>';
  }
  return $output;
}

/**
 * Overrides theme_admin_block_content().
 *
 * Use unordered list markup in both compact and extended mode.
 */
function bootstrap_lite_admin_block_content($variables) {
  return bootstrap_lite_node_add_list($variables);
}

/**
 * Process variables for user-picture.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $account: A user, node or comment object with 'name', 'uid' and 'picture'
 *   fields.
 *
 * @see user-picture.tpl.php
 */
function bootstrap_lite_preprocess_user_picture(&$variables) {
 // print_r($variables);
//  $variables['user_picture'] = '';
/*  if (config_get('system.core', 'user_pictures')) {
    $account = $variables['account'];
    if (!empty($account->picture)) {
      // @TODO: Ideally this function would only be passed file entities, but
      // since there's a lot of legacy code that JOINs the {users} table to
      // {node} or {comments} and passes the results into this function if we
      // a numeric value in the picture field we'll assume it's a file id
      // and load it for them. Once we've got user_load_multiple() and
      // comment_load_multiple() functions the user module will be able to load
      // the picture files in mass during the object's load process.
      if (is_numeric($account->picture)) {
        $account->picture = file_load($account->picture);
      }
      if (!empty($account->picture->uri)) {
        $filepath = $account->picture->uri;
      }
    }
    elseif (config_get('system.core', 'user_picture_default')) {
      $filepath = config_get('system.core', 'user_picture_default');
    }
    if (isset($filepath)) {
      $alt = t("@user's picture", array('@user' => user_format_name($account)));
      // If the image does not have a valid drupal scheme (for eg. HTTP),
      // don't load image styles.
      if (module_exists('image') && file_valid_uri($filepath) && $style = config_get('system.core', 'user_picture_style')) {
        $variables['user_picture'] = theme('image_style', array('style_name' => $style, 'uri' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => 'img-circle')));
      }
      else {
        $variables['user_picture'] = theme('image', array('uri' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => 'img-circle')));
      }
      if (!empty($account->uid) && user_access('access user profiles')) {
        $attributes = array('attributes' => array('title' => t('View user profile.')), 'html' => TRUE);
        $variables['user_picture'] = l($variables['user_picture'], "user/$account->uid", $attributes);
      }
    }
  }*/
}

/**
 * Implements hook_preprocess_comment().
 */
function bootstrap_lite_preprocess_comment(&$variables){
//  print_r($variables);
  if (theme_get_setting('bootstrap_lite_datetime')) {
    $comment = $variables['elements']['#comment'];
    $variables['timeago'] = t('@time ago', array('@time' => format_interval(time() - $comment->changed)));
  }
}

/**
 * Implements hook_preprocess_node().
 */
function bootstrap_lite_preprocess_node(&$variables){
  if (theme_get_setting('bootstrap_lite_datetime')) {
    $node = $variables['elements']['#node'];
    $variables['timeago'] = t('@time ago', array('@time' => format_interval(time() - $node->created)));
  }
}

/**
 * Implements theme_status_messages().
 */
function bootstrap_lite_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
    'info' => t('Informative message'),
  );

  // Map Drupal message types to their corresponding Bootstrap classes.
  // @see http://twitter.github.com/bootstrap/components.html#alerts
  $status_class = array(
    'status' => 'success',
    'error' => 'danger',
    'warning' => 'warning',
    'info' => 'info',
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    $class = (isset($status_class[$type])) ? ' alert-' . $status_class[$type] : '';
    $output .= "<div class=\"alert alert-block$class\">\n";
    $output .= "  <a class=\"close\" data-dismiss=\"alert\" href=\"#\">&times;</a>\n";

    if (!empty($status_heading[$type])) {
      $output .= '<h4 class="element-invisible">' . $status_heading[$type] . "</h4>\n";
    }
    switch($type){
      case 'success':
        $output .= '<i class="fa fa-check"></i> ';
        break;
      case 'error':
        $output .= '<i class="fa fa-times"></i> ';
        break;
      case 'warning':
        $output .= '<i class="fa fa-exclamation-triangle"></i> ';
        break;
      case 'info':
      default:
        $output .= '<i class="fa fa-info-circle"></i> ';
    }
    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= $messages[0];
    }

    $output .= "</div>\n";
  }
  return $output;
}

/**
 * implements theme_menu_link().
 */
function bootstrap_lite_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    // Prevent dropdown functions from being added to management menu so it
    // does not affect the navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
      // Add our own wrapper.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = '<ul class="dropdown-menu">' . drupal_render($element['#below']) . '</ul>';
      // Generate as standard dropdown.
      $element['#title'] .= ' <span class="caret"></span>';
      $element['#attributes']['class'][] = 'dropdown';
      $element['#localized_options']['html'] = TRUE;

      // Set dropdown trigger element to # to prevent inadvertant page loading
      // when a submenu link is clicked.
      $element['#localized_options']['attributes']['data-target'] = '#';
      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
      $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';
    }
  }
  // On primary navigation menu, class 'active' is not set on active menu item.
  // @see https://drupal.org/node/1896674
  if (($element['#href'] == $_GET['q'] || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']))) {
    $element['#attributes']['class'][] = 'active';
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

function bootstrap_lite_menu_tree__system_primary_menu($variables){
  $navbar_menu_position = theme_get_setting('bootstrap_lite_navbar_menu_position');
  return '<ul class="menu nav navbar-nav ' . $navbar_menu_position . ' primary-menu">' . $variables['tree'] . '</ul>';
}

function bootstrap_lite_menu_tree__system_secondary_menu($variables){
  return '<ul class="menu nav navbar-nav navbar-right secondary-menu">' . $variables['tree'] . '</ul>';
}
