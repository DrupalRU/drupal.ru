<?php

/**
 * A preprocess function for theme('mimemail_message').
 *
 * The $variables array initially contains the following arguments:
 * - $recipient: The recipient of the message
 * - $subject: The message subject
 * - $body: The message body
 * - $css: Internal style sheets
 * - $module: The sending module
 * - $key: The message identifier
 * - $logo: The site logo for mail
 * - $menu_top: Main menu
 * - $menu_bottom: Secondary menu
 * - $base_path: Site base url
 *
 * @see mimemail-message.tpl.php
 */
function druru_preprocess_mimemail_message(&$variables) {
  $logo = drupal_get_path('theme', 'druru') . '/img/logos/logo-mail.png';

  $variables['logo'] = file_exists($logo) ? file_create_url($logo) : null;
  $variables['base_path'] = _druru_mail_link_builder(base_path())['link'];
  // ToDo: Uncomment for generate dynamically;
  // $variables['menu_top'] = druru_build_menu_for_mail('main-menu');
  // $variables['menu_bottom'] = druru_build_menu_for_mail('main-menu');
  $variables['menu_top'] = array_map(function ($i) {
    return _druru_mail_link_builder(...$i);
  }, [
    ['tracker', 'Трекер'],
    ['forum', 'Форум'],
    ['services', 'Компании'],
    ['events', 'События'],
  ]);
  $variables['menu_bottom'] = array_map(function ($i) {
    return _druru_mail_link_builder(...$i);
  }, [
    ['privacy-policy', 'Конфиденциальность'],
    ['rules', 'Правила'],
    ['ru_team', 'Команда'],
    ['contact', 'Обратная связь'],
  ]);
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
    return _druru_mail_link_builder($v['link']['href'], $v['link']['link_title']);
  }, array_filter(menu_tree_all_data($menu_name, null, 1), function ($e) {
    return !$e['link']['hidden'];
  }));
}

/**
 * Make links for mail.
 *
 * @param string|null $url   Link href.
 * @param string|null $title Link title.
 *
 * @return array
 */
function _druru_mail_link_builder($url = null, $title = null) {
  return [
    'link'  => url($url ?: $GLOBALS['base_url'], ['absolute' => true]),
    'title' => $title ?: variable_get('site_name'),
  ];
}
