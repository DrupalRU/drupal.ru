<?php

/**
 * @file
 * template.php
 */

/**
 * Implements hook_theme().
 */
function alpha_theme($existing, $type, $theme, $path) {
  return array(
    'menu_user_blog_links' => array(
      'variables' => array('primary' => array(), 'secondary' => array()),
    ),
  );
}
 
/**
 * Implements hook_js_alter().
 */
function alpha_js_alter(&$javascript) {
  drupal_add_js('var themeTableHeaderOffset = function() { var offsetheight = jQuery("#navbar").height(); return offsetheight; }', 'inline');
  drupal_add_js(array('tableHeaderOffset' => 'themeTableHeaderOffset'), 'setting'); 
}

/**
 * Preprocess variables for page.tpl.php
 *
 * Most themes utilize their own copy of page.tpl.php. The default is located
 * inside "modules/system/page.tpl.php". Look in there for the full list of
 * variables.
 *
 * Uses the arg() function to generate a series of page template suggestions
 * based on the current path.
 *
 * Any changes to variables in this preprocessor should also be changed inside
 * template_preprocess_maintenance_page() to keep all of them consistent.
 *
 * @see drupal_render_page()
 * @see template_process_page()
 * @see page.tpl.php
 */
function alpha_preprocess_page(&$variables){
  if($variables['theme_hook_suggestions'][0] == 'page__user'){
    $account = $variables['page']['content']['#account'];
    
    $picture = $account->picture;
    if (!empty($picture)) {
      if (!empty($picture->uri)) {
        $filepath = $picture->uri;
      }
    }
    elseif (variable_get('user_picture_default', '')) {
      $filepath = variable_get('user_picture_default', '');
    }
    if (isset($filepath)) {
      if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style_node', '')) {
        $variables['user_picture'] = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => array('img-circle'))));
      }
      else {
        $variables['user_picture'] = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => array('img-circle'))));
      }
    }
  }
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
function alpha_preprocess_user_picture(&$variables) {
  $variables['user_picture'] = '';
  if (variable_get('user_pictures', 0)) {
    $account = $variables['account'];
    if (!empty($account->picture)) {
      // @TODO: Ideally this function would only be passed file objects, but
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
    elseif (variable_get('user_picture_default', '')) {
      $filepath = variable_get('user_picture_default', '');
    }
    if (isset($filepath)) {
      $alt = t("@user's picture", array('@user' => format_username($account)));
      // If the image does not have a valid Drupal scheme (for eg. HTTP),
      // don't load image styles.
      if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')) {
        $variables['user_picture'] = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => array('img-circle'))));
      }
      else {
        $variables['user_picture'] = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => array('img-circle'))));
      }
      if (!empty($account->uid) && user_access('access user profiles')) {
        $attributes = array('attributes' => array('title' => t('View user profile.')), 'html' => TRUE);
        $variables['user_picture'] = l($variables['user_picture'], "user/$account->uid", $attributes);
      }
    }
  }
}

/**
 * Implements hook_preprocess_comment().
 */
function alpha_preprocess_comment(&$variables){
  $comment = $variables['elements']['#comment'];
  $variables['timeago'] = t('@time ago', array('@time' => format_interval(time() - $comment->changed)));
  
  $uri = entity_uri('comment', $comment);
  $variables['permalink'] = l('#', $uri['path'], $uri['options']);
}

/**
 * Implements hook_preprocess_node().
 */
function alpha_preprocess_node(&$variables){
  $node = $variables['elements']['#node'];
  if ($variables['teaser']) {
    // Add a new item into the theme_hook_suggestions array.
    $variables['theme_hook_suggestions'][] = 'node__teaser';
  }
  
  $variables['timeago'] = t('@time ago', array('@time' => format_interval(time() - $node->changed)));

  $picture = $node->picture;
  if (!empty($picture)) {
    if (is_numeric($picture)) {
      $picture = file_load($picture);
    }
    if (!empty($picture->uri)) {
      $filepath = $picture->uri;
    }
  }
  elseif (variable_get('user_picture_default', '')) {
    $filepath = variable_get('user_picture_default', '');
  }
  if (isset($filepath)) {
    $alt = $node->name;
    if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style_node', '')) {
      $variables['user_picture'] = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => array('img-circle'))));
    }
    else {
      $variables['user_picture'] = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => array('img-circle'))));
    }
    if (!empty($node->uid) && user_access('access user profiles')) {
      $attributes = array('attributes' => array('title' => t('View user profile.')), 'html' => TRUE);
      $variables['user_picture'] = l($variables['user_picture'], "user/$node->uid", $attributes);
    }
  }
  
  $variables['title_attributes'] = 'title';
  if(!$variables['teaser']){
    drupal_add_js(drupal_get_path('theme', 'alpha') . '/js/' . 'node-view.js');
  }
  
  drupal_add_js(drupal_get_path('theme', 'alpha') . '/js/' . 'node-img-responsive.js');
}


/**
 * Process variables for user-profile.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - elements: An associative array containing the user information and any
 *     fields attached to the user. Properties used:
 *     - #account: The user account of the profile being viewed.
 *
 * @see user-profile.tpl.php
 */
function alpha_preprocess_user_profile(&$variables) {
  global $user;
  $account = $variables['elements']['#account'];
  if($account->data['contact']){
    //add contact form link
  }
  if(!$account->privatemsg_disabled && $user->uid > 0){
    //add private message link
  }
  
  foreach (element_children($variables['elements']) as $key) {
    
    if(isset($variables['elements'][$key]['#type']) && $variables['elements'][$key]['#type'] == 'user_profile_category'){
      foreach($variables['elements'][$key] as $item_key => $item_value){
        if(is_array($item_value) && isset($item_value['#type']) && $item_value['#type'] == 'user_profile_item'){
          if(empty($item_value['#title'])){
            $variables['elements'][$key][$item_key]['#title'] = $variables['elements'][$key]['#title'];
            // We add title only for first item in the group.
            break;
          }
        }
      }
    }
    $variables['user_profile'][$key] = $variables['elements'][$key];
  }
  


  // Preprocess fields.
  field_attach_preprocess('user', $account, $variables['elements'], $variables);

  
//  print_r($variables);
  $variables['name'] = $account->name;
  if(!empty($account->realname)){
    $variables['name'] = $account->realname;
  }
  
  if($account->signature){
    $variables['signature'] = check_markup($account->signature, $account->signature_format, '', TRUE);
  }
  
  module_load_include('inc', 'blog', 'blog.pages');
  $variables['blog'] = blog_page_user($account);
  
//  print_r($variables);
}


/**
 * Implements hook_file_formatter_table().
 */
function alpha_file_formatter_table($variables) {
  $links = '';
  foreach ($variables['items'] as $delta => $item) {
    $links .= '<li>' . theme('file_link', array('file' => (object) $item)) . '<span class="size">' . format_size($item['filesize']) . '</span>' . '</li>';
  }
  
  return empty($links) ? '' : '<ul class="file-links" >' . $links . '</ul>';
}

function alpha_menu_user_blog_links($variables){
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<ul class="menu nav navbar-nav">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<ul class="menu nav navbar-nav">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}
