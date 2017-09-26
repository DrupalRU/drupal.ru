<?php


function druru_form_comment_form_alter(&$form, &$form_state, $form_id) {
  if (isset($form['comment_body']['und'][0])) {
    $form['comment_body']['und'][0]['#title_display'] = 'invisible';
  }
  if(isset($form['author'])){
    $form['author']['#access'] = false;
  }
}
