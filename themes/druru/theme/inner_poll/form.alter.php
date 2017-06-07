<?php

function druru_form_inner_poll_form_alter(&$form, &$form_state) {
  $button = &$form['button']['#suffix'];
  $btn = explode('onclick', $button);
  $button = $btn[0] . ' class="btn btn-primary" onclick' . $btn[1];

  $button = &$form['abstain']['#suffix'];
  $btn = explode('onclick', $button);
  $button = $btn[0] . ' class="btn btn-primary" onclick' . $btn[1];

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['button'] = $form['button'];
  $form['actions']['abstain'] = $form['abstain'];

  unset($form['button'], $form['abstain']);

}
