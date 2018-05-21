<?php

/**
 * @file
 */

/**
 * Slack API admin settings form.
 */
function dru_slack_admin() {

  $chanels = variable_get('slack_chanels');
  $chanelsForms = [];
  $i = 0;
  if ($chanels) {
    foreach ($chanels as $key => $val) {
      $chanelsForms['inline'][] = _dru_slack_init_chanel($key, $val, $i);
      $i++;
    }
  }
  else {
    $chanelsForms['inline'][] = _dru_slack_init_chanel('', '', $i);
  }
  $chanelsForms['#prefix'] = '<div id="chanels"><h2>' . t('Slack chanels') . '</h2>';
  $chanelsForms['#suffix'] = '</div>';

  $form = [
    'slack_team_hostname' => [
      '#type' => 'textfield',
      '#title' => t('Slack hostname'),
      '#default_value' => variable_get('slack_team_hostname'),
      '#description' => t('hostname without suffix ".slack.com"'),
      '#required' => TRUE,
    ],
    'slack_token' => [
      '#type' => 'textfield',
      '#title' => t('Auth token'),
      '#default_value' => variable_get('slack_token'),
      '#description' => t('You can create a token for your application on the next <a href="@url">page</a>', ['@url' => 'https://api.slack.com/custom-integrations/legacy-tokens']),
      '#required' => TRUE,
    ],
    'slack_approved_users' => [
      '#type' => 'textfield',
      '#title' => t('User list to access for slack commands'),
      '#default_value' => variable_get('slack_approved_users'),
      '#description' => t('Comma separated user list, who access to execute slack commands'),
      '#required' => FALSE,
    ],
    'container' => $chanelsForms,
    'add_url' => [
      '#type' => 'button',
      '#value' => t('Add chanel'),
      '#ajax' => [
        'callback' => 'dru_slack_add_chanel',
        'wrapper' => 'chanels',
        'method' => 'replace',
        'effect' => 'fade',
      ],
    ],

  ];
  $settingsForm = system_settings_form($form);
  $settingsForm['#submit'] = ['dru_slack_form_submit'];
  return $settingsForm;
}

/**
 * Формирует массив для инлайново формсета.
 *
 * @param $label
 *   Название канала
 * @param $value
 *   URL для сообщений в канал
 * @param $i
 *   Порядковый номер для идентификации значений
 *
 * @return array
 *   Набор элементов для инлайн формы
 */
function _dru_slack_init_chanel($label, $value, $i) {
  $output = [
    '#prefix' => '<div style="display: flex;">',
    '#suffix' => '</div>',
    'label_' . $i => [
      '#type' => 'textfield',
      '#title' => t('Chanel'),
      '#description' => t('Chanel name without #'),
      '#attributes' => ['style' => 'margin-right: 2em'],
      '#default_value' => $label,
      '#required' => FALSE,
    ],
    'value_' . $i => [
      '#type' => 'textfield',
      '#title' => t('Url'),
      '#default_value' => $value,
      '#required' => FALSE,
    ],
  ];
  if ($label != '' && $value != '') {
    $output['remove_' . $i] = [
      '#type' => 'image_button',
      '#src' => drupal_get_path('module', 'dru_slack') . '/img/garbage.svg',
      '#attributes' => ['style' => 'height:1em; background: none; border: none; line-height:2; display: block; margin-top: 2em; margin-right: 0; fill: #0074BD'],
      '#ajax' => [
        'callback' => 'dru_slack_remove_chanel',
        'wrapper' => 'chanels',
        'method' => 'replace',
      ],
    ];
    $output['send_message_' . $i] = [
      '#type' => 'image_button',
      '#src' => drupal_get_path('module', 'dru_slack') . '/img/sent-mail.svg',
      '#attributes' => ['style' => 'height:1em; background: none; border: none; line-height:2; display: block; margin-top: 2em; margin-right: 0; fill: #0074BD'],
      '#ajax' => [
        'callback' => 'dru_slack_send_test_message',
      ],
    ];
  }
  return $output;
}

/**
 * Возвращает подготовленный массив для сохранения в переменную каналов.
 *
 * @param $values
 *   массив значений из $form_state
 * @param int $trigger
 *   Указатель на удаляемый элемент.
 *
 * @return array
 */
function _dru_slack_chanels_to_save($values, $trigger = -1) {
  $data = [];
  foreach ($values as $k => $v) {
    if (substr($k, 0, 5) == 'label') {
      $karr = explode('_', $k);
      $i = end($karr);
      if ($values[$k] != '' && $values["value_{$i}"] != '' && $trigger != $i) {
        $data[$values[$k]] = $values["value_{$i}"];
      }
    }
  }
  return $data;
}

/**
 * Добавление инлайновой формы в основную.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function dru_slack_add_chanel($form, &$form_state) {
  $form['container']['inline'][] = _dru_slack_init_chanel('', '', count($form['container']['inline']) - 2);
  return $form['container'];
}

/**
 * Удаление инлановой формы из основной.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function dru_slack_remove_chanel($form, &$form_state) {
  $tmp = explode('_', $form_state['triggering_element']['#name']);
  $iter = end($tmp);
  $inlinedata = _dru_slack_chanels_to_save($form_state['values'], $iter);
  $old = variable_get('slack_chanels', []);
  if ($old != $inlinedata) {
    variable_set('slack_chanels', $inlinedata);
  }
  unset($form['container']['inline'][$iter]);
  return $form['container'];

}

/**
 * Отправка тестового сообщения в Slack.
 *
 * @param $form
 * @param $form_state
 *
 * @return null
 */
function dru_slack_send_test_message($form, &$form_state) {
  $tmp = explode('_', $form_state['triggering_element']['#name']);
  $iter = end($tmp);
  $url = $form_state['values']['value_' . $iter];
  $slack = new slackAPI();
  $slack->notificate_by_url(
    $url,
    [
      'text' => t('Test message'),
    ]
  );
  return NULL;
}

/**
 * Slack API admin form submit.
 *
 * @param $form
 * @param $form_state
 */
function dru_slack_form_submit($form, &$form_state) {
  $requiredKeys = [
    'slack_token',
    'slack_approved_users',
    'slack_team_hostname',
  ];
  foreach ($requiredKeys as $formKey) {
    if ($form[$formKey]['#default_value'] != $form_state['values'][$formKey]) {
      variable_set($formKey, $form_state['values'][$formKey]);
    }
  }
  $inlinedata = _dru_slack_chanels_to_save($form_state['values']);
  $old = variable_get('slack_chanels');
  if ($old != $inlinedata) {
    variable_set('slack_chanels', $inlinedata);
  }
}
