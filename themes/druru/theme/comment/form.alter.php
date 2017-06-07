<?php


function druru_form_comment_form_alter(&$form, &$form_state, $form_id) {
  if (isset($form['comment_body']['und'][0])) {
    $form['comment_body']['und'][0]['#title_display'] = 'invisible';
  }
  if (isset($form['author']['_author']['#markup'])) {
    $form['author']['_author']['#title'] = l($GLOBALS['user']->name, '');
    $style = variable_get('user_picture_style', '');
    variable_set('user_picture_style', 'avatar');
    $form['author']['_author']['#markup'] = theme('user_picture', array(
      'account'    => $GLOBALS['user'],
      'style_name' => 'large',
    ));
    variable_set('user_picture_style', $style);
  }
}
