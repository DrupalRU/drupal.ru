<?php

/**
 * A preprocess function for theme('mimemail_message').
 *
 * The $variables array initially contains the following arguments:
 * - $recipient: The recipient of the message
 * - $key:  The mailkey associated with the message
 * - $subject: The message subject
 * - $body:  The message body
 *
 * @see mimemail-message.tpl.php
 */
function druru_preprocess_mimemail_message(&$variables) {
  $logo = drupal_get_path('theme', 'druru') . '/img/logos/logo-mail.png';

  $variables['logo'] = file_exists($logo) ? file_create_url($logo) : null;
  $variables['base_path'] = base_path();
  $variables['menu_top'] = druru_build_menu_for_mail('main-menu');
  $variables['menu_bottom'] = druru_build_menu_for_mail('main-menu');
}

/**
 * Build menu links for email template.
 *
 * @param string $menu_name
 *
 * @return array
 */
function druru_build_menu_for_mail($menu_name) {
  return array_map(function ($v) {
    return [
      'link'  => url($v['link']['href'], ['absolute' => true]),
      'title' => $v['link']['link_title'],
    ];
  }, array_filter(menu_tree_all_data($menu_name, null, 1), function ($e) {
    return !$e['link']['hidden'];
  }));
}
