<?php

/**
 * Process variables for user-profile.tpl.php.
 *
 * @param array $variables
 *     An associative array containing:
 *     - elements: An associative array containing the user information and any
 *     fields attached to the user. Properties used:
 *     - #account: The user account of the profile being viewed.
 *
 * @see user-profile.tpl.php
 */
function druru_preprocess_user_profile(&$variables) {
  if (empty($variables['elements']['#account'])) {
    return;
  }

  _druru_prepare_profile_thanks($variables);

  _druru_prepare_profile_groups($variables);

  module_load_include('inc', 'blog', 'blog.pages');
  $variables['blog'] = blog_page_user($variables['elements']['#account']);
  if(!$variables['blog']){
    $variables['blog'] = druru_status_messages(array(
      'display' => FALSE,
      'messages' => array(
        'info' => array(
          $GLOBALS['user']->uid == $variables['elements']['#account']->uid
            ? t('You don\'t have posts. You can create <a href="@url">first post</a>', array(
              '@url' => 'node/add/blog'
            ))
            : t('This user has no posts.')
        )
      ),
    ));
  }

  _druru_prepare_order($variables['user_profile']);
}

function _druru_prepare_profile_groups(&$variables) {

  $profile = &$variables['user_profile'];
  $elements = &$variables['elements'];
  $account = $elements['#account'];
  _druru_prepare_private_message($elements);
  _druru_prepare_user_picture($elements);

  foreach (element_children($elements) as $key) {
    $is_element = isset($elements[$key]['#type']);
    if ($is_element && $elements[$key]['#type'] == 'user_profile_category') {
      foreach ($elements[$key] as $item_key => $item_value) {
        $is_item = isset($item_value['#type']);
        if ($is_item && $item_value['#type'] == 'user_profile_item') {
          if (empty($item_value['#title'])) {
            $elements[$key][$item_key]['#title'] = $elements[$key]['#title'];
            // We add title only for first item in the group.
            break;
          }
        }
      }
    }
    $profile[$key] = $elements[$key];
  }

  // Preprocess fields.
  field_attach_preprocess('user', $account, $elements, $variables);

  $variables['name'] = $account->name;
  $variables['realname'] = @($account->realname ?: $account->name);
  $variables['realname'] = drupal_ucfirst($variables['realname']);

  if ($account->signature) {
    $variables['signature'] = check_markup($account->signature, $account->signature_format, '', TRUE);
  }
}

function _druru_prepare_private_message(&$elements) {
  $link = &$elements['privatemsg_send_new_message'];
  $title = '<span class="visible-xs">' . t('Send message') . '</span>';
  $link['#title'] = $title . ' ' . druru_icon('envelope');
  $link['#options']['html'] = TRUE;
  $attributes = &$link['#options']['attributes'];
  if (!is_array($attributes['class'])) {
    $attributes['class'] = array($attributes['class']);
  }
  $attributes['class'][] = 'btn';
  $attributes['class'][] = 'btn-primary';
  $attributes['class'][] = 'pull-right';
}

function _druru_prepare_user_picture(&$elements) {
  $account = $elements['#account'];
  $picture = isset($account->picture) ? $account->picture : null;
  if (!empty($picture->uri)) {
    $filepath = $picture->uri;
  }
  elseif (variable_get('user_picture_default', '')) {
    $filepath = variable_get('user_picture_default', '');
  }
  if (isset($filepath)) {
    $elements['user_picture']['#markup'] = theme('image_style', array(
      'style_name' => 'avatar_profile',
      'path' => $filepath,
      'alt' => 'user-icon',
    ));
  }
}

function _druru_prepare_order(&$elements) {
  $x = -100;
  $weigts = array(
    'Персональные данные' => ++$x,
    'Контакты' => ++$x,
    'Координаты в интернете' => ++$x,
    'Предлагаю сервисы для Drupal' => ++$x,
    'Мои работы для Drupal' => ++$x,
    'Рассылки' => ++$x,
    'summary' => ++$x,
    'simplenews' => ++$x,
  );
  foreach ($weigts as $key => $weigt) {
    if(isset($elements[$key])){
      $elements[$key]['#weight'] = $weigt;
    }
  }
}

function _druru_prepare_profile_thanks(&$variables) {
  $profile = &$variables['elements'];
  $copy_icon = druru_icon('copy');
  $comments_icon = druru_icon('comments-o');

  foreach (array('user_tnx', 'users_tnx') as $stat) {
    if (!empty($profile['summary'][$stat])) {
      $user_tnx = &$profile['summary'][$stat];
      $user_tnx['#markup'] = "$copy_icon {$user_tnx['#thx_node']}";
      $user_tnx['#markup'] .= ", $comments_icon {$user_tnx['#thx_comment']}";
    }
  }
}
