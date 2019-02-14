<?php

function druru_form_alter(&$form, &$form_state) {
  if (isset($form['inner_poll'])) {
    $poll = &$form['inner_poll'];
    $poll['question']['#title'] = t('Question');
    unset($poll['question']['#description'], $poll['#description']);
    $poll['choice_wrapper']['choice_0']['#title'] = t('Response options');
    foreach (element_children($poll['choice_wrapper']) as $child) {
      unset($poll['choice_wrapper'][$child]['#description']);
    }
  }
}

function druru_form_blog_node_form_alter(&$form, &$form_state) {
  $weight = 100;
  $form['type_version'] = array(
    '#type'       => 'container',
    '#weight'     => $weight,
    '#attributes' => array(
      'class' => array('row'),
    ),
  );
  $name = 'taxonomy_vocabulary_8';
  if (isset($form[$name])) {
    $field = $form[$name];
    $form['type_version'][$name] = $field;
    $form['type_version'][$name]['#attributes']['class'] = array('col-sm-6');
    $weight = $weight > $field['#weight'] ? $field['#weight'] : $weight;
    $form['type_version']['#weight'] = $weight;
    unset($form[$name]);
  }
  $name = 'taxonomy_vocabulary_7';
  if (isset($form[$name])) {
    $field = $form[$name];
    $form['type_version'][$name] = $field;
    $form['type_version'][$name]['#attributes']['class'] = array('col-sm-6');
    $weight = $weight > $field['#weight'] ? $field['#weight'] : $weight;
    $form['type_version']['#weight'] = $weight;
    unset($form[$name]);
  }

  $weight = 100;
  $form['keys'] = array(
    '#type'       => 'container',
    '#weight'     => $weight,
    '#attributes' => array(
      'class' => array('row'),
    ),
  );
  $name = 'taxonomy_vocabulary_10';
  if (isset($form[$name])) {
    $field = $form[$name];
    $form['keys'][$name] = $field;
    $form['keys'][$name]['#attributes']['class'] = array('col-sm-6');
    $weight = $weight > $field['#weight'] ? $field['#weight'] : $weight;
    $form['keys']['#weight'] = $weight;
    unset($form[$name]);
  }
}
