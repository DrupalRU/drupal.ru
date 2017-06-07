<?php

function druru_form_user_register_form_alter(&$form, &$form_state) {
  $account = &$form['account'];
  $account['name']['#prefix'] = '<div class="row">';
  $account['name']['#prefix'] .= '<div class="col-vsd-5">';
  $account['name']['#prefix'] .= '<div class="well-lg bg-primary">';

  $account['mail']['#suffix'] = '</div>';
  $account['mail']['#suffix'] .= '</div>';

  $account['pass']['#prefix'] = '<div class="col-vsd-7 ">';
  $account['pass']['#suffix'] = '</div>';
  $account['pass']['#suffix'] .= '</div>';
}
