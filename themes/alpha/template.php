<?php

/**
 * @file
 * template.php
 */

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
  print_r($variables['theme_hook_suggestions']);
  
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

  drupal_add_js('(function($){ $(".field-name-body img").addClass("img-responsive");})(jQuery);', array('type' => 'inline', 'scope' => 'footer'));
  drupal_add_js('(function($){ $(".comment .content img").addClass("img-responsive");})(jQuery);', array('type' => 'inline', 'scope' => 'footer'));
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
