<?php

/**
 * Implements hook_rrssb_buttons().
 */
function rrssb_rrssb_buttons() {
  $buttons = array(
    'delicious' => array(
      'share_url' => 'https://delicious.com/save?v=5&noui&jump=close&url=[rrssb:url]&title=[rrssb:title]',
      'color' => '#0b79e5',
      'color_hover' => '#095fb4',
    ),
    'email' => array(
      'svgfile' => '<icons>/mail.min.svg',
      'share_url' => 'mailto:?subject=[rrssb:title]&body=[rrssb:url]',
      'color' => '#0a88ff',
      'color_hover' => '#006ed6',
      'popup' => FALSE,
    ),
    'facebook' => array(
      'share_url' => 'https://www.facebook.com/sharer/sharer.php?u=[rrssb:url]',
      'follow_url' => 'https://www.facebook.com/[rrssb:username]',
      'color' => '#306199',
      'color_hover' => '#244872',
    ),
    'instagram' => array(
      'follow_url' => 'https://instagram.com/[rrssb:username]',
      'color' => '#125688',
      'color_hover' => '#0c3a5b',
    ),
    'github' => array(
      'follow_url' => 'https://github.com/[rrssb:username]',
      'color' => '#444',
      'color_hover' => '#2b2b2b',
    ),
    'googleplus' => array(
      'svgfile' => '<icons>/google_plus.min.svg',
      'share_url' => 'https://plus.google.com/share?url=[rrssb:title] [rrssb:url]',
      'follow_url' => 'https://plus.google.com/[rrssb:username]/posts',
      'color' => '#e93f2e',
      'color_hover' => '#ce2616',
      'text' => 'google+',
    ),
    'hackernews' => array(
      'share_url' => 'https://news.ycombinator.com/submitlink?u=[rrssb:url]&t=[rrssb:title]&text=[rrssb:title]',
      'color' => '#f60',
      'color_hover' => '#cc5200',
    ),
    'linkedin' => array(
      'share_url' => 'http://www.linkedin.com/shareArticle?mini=true&url=[rrssb:url]&title=[rrssb:title]',
      'follow_url' => 'https://www.linkedin.com/in/[rrssb:username]',
      'color' => '#007bb6',
      'color_hover' => '#005983',
    ),
    'pinterest' => array(
      // Encoding a slash prevents Pinterest's JS from "hijacking" the link - see http://stackoverflow.com/questions/11312923/custom-pinterest-button-for-custom-url-text-link-image-or-both.
      'share_url' => 'http://pinterest.com/pin/create%2Fbutton/?url=[rrssb:url]&media=[rrssb:image]&description=[rrssb:title]',
      'follow_url' => 'https://www.pinterest.com/[rrssb:username]',
      'color' => '#b81621',
      'color_hover' => '#8a1119',
    ),
    'pocket' => array(
      'share_url' => 'https://getpocket.com/save?url=[rrssb:url]',
      'color' => '#ed4054',
      'color_hover' => '#e4162d',
    ),
    'print' => array(
      'share_url' => 'javascript:window.print()',
      'color' => '#8d98a2',
      'color_hover' => '#717f8b',
      'popup' => FALSE,
    ),
    'reddit' => array(
      'share_url' => 'http://www.reddit.com/submit?url=[rrssb:url]&title=[rrssb:title]',
      'follow_url' => 'https://www.reddit.com/user/[rrssb:username]',
      'color' => '#8bbbe3',
      'color_hover' => '#62a3d9',
    ),
    'tumblr' => array(
      'share_url' => 'http://tumblr.com/share?s=&amp;v=3&t=[rrssb:title]&u=[rrssb:url]',
      'follow_url' => 'http://[rrssb:username].tumblr.com/',
      'color' => '#32506d',
      'color_hover' => '#22364a',
    ),
    'twitter' => array(
      'share_url' => 'https://twitter.com//intent/tweet?text=[rrssb:title]&url=[rrssb:url]',
      'follow_url' => 'https://twitter.com/[rrssb:username]',
      'color' => '#26c4f1',
      'color_hover' => '#0eaad6',
    ),
    'vk' => array(
      'share_url' => 'http://vk.com/share.php?url=[rrssb:url]',
      'color' => '#4d71a9',
      'color_hover' => '#3d5a86',
      'text' => 'vk.com',
    ),
    'whatsapp' => array(
      'share_url' => 'whatsapp://send?text=[rrssb:title] [rrssb:url]',
      'color' => '#43d854',
      'color_hover' => '#28c039',
    ),
    'youtube' => array(
      'follow_url' => 'https://www.youtube.com/user/[rrssb:username]',
      'color' => '#df1c31',
      'color_hover' => '#b21627',
    ),
  );

  return $buttons;
}
