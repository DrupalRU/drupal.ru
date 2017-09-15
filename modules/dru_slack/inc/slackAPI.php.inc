<?php
/**
 * Created by PhpStorm.
 * User: hulygun
 * Date: 10.09.17
 * Time: 23:11
 */

define('SLACK_INVITE_ALREADY_IN_TEAM', 'already_in_team');
define('SLACK_INVITE_SENT_RECENTLY', 'sent_recently');
define('SLACK_INVITE_ALREADY_INVITED', 'already_invited');

/**
 * Class slackAPI
 * Реализация интерфейса для работы с API Slack
 */
class slackAPI {

  protected $hostname;

  protected $token;

  public function __construct() {
    $this->hostname = variable_get('slack_team_hostname', NULL);
    $this->token = variable_get('slack_token', NULL);;
  }

  /**
   * Отправка сообщений в Slack по урлу
   *
   * @param $url
   *   url, на который следует слать сообщение
   * @param $message
   *   массив сообщения
   *
   * @return object
   *   drupal_http_request
   */
  public function notificate_by_url($url, $message) {
    $json_string = json_encode($message);
    $options = [
      'method' => 'POST',
      'headers' => [
        'Content-Type' => 'application/json',
        'Content-Length' => strlen($json_string),
      ],
      'data' => $json_string,
    ];

    return drupal_http_request("{$url}", $options);
  }

  /**
   * Отправка сообщений в Slack по названию канала
   *
   * @param $chanel
   *   название канала
   * @param $message
   *   массив сообщения
   *
   * @return object
   */
  public function notificate_by_chanel($chanel, $message) {
    $url = variable_get('slack_chanels')[$chanel];
    return $this->notificate_by_url($url, $message);
  }

  /**
   * Отправка инвайтов в Slack по email
   *
   * @param $email
   *   email получателя
   *
   * @return array
   */
  public function invite($email) {
    $url = "https://{$this->hostname}.slack.com/api/users.admin.invite?t=" . time();
    $data = [
      '_attempts' => 1,
      'email' => $email,
      'set_active' => 'true',
      'token' => $this->token,
    ];
    $data = drupal_http_build_query($data);
    $options = [
      'method' => 'POST',
      'headers' => ['Content-Type' => 'application/x-www-form-urlencoded'],
      'data' => $data,
    ];

    $response = drupal_http_request("{$url}", $options);
    $status = 'error';
    if ($response->code == 200) {
      $data = drupal_json_decode($response->data);
      if (!empty($data['ok'])) {
        $message = t("Приглашение отправлено на адрес {$email}");
        $status = 'status';
      }
      else {
        $status = 'warning';
        switch ($data['error']) {
          case SLACK_INVITE_ALREADY_IN_TEAM:
            $message = t("Пользователь с email {$email} уже является участником https://{$this->hostname}.slack.com/");
            break;

          case SLACK_INVITE_SENT_RECENTLY:
            $message = t("Пользователю с email {$email} недавно уже было отправлено приглашение");
            break;

          case SLACK_INVITE_ALREADY_INVITED:
            $message = t("Пользователь с email {$email} уже приглашён");
            break;

          default:
            $message = $data['error'];
            $status = 'error';
            break;
        }
      }
    }
    else {
      $message = t("Возникла ошибка при отправке приглашения на email {$email}. За более подробной информацией обратитесь к администратору.");
    }
    return ['status' => $status, 'message' => $message];

  }


  /**
   * Получение данных об участниках Slack team
   *
   * @return array
   */
  public function badge() {
    $url = "https://slack.com/api/users.list";
    $data = [
      'token' => $this->token,
      'presence' => true,
    ];
    $data = drupal_http_build_query($data);
    $response = drupal_http_request("{$url}?{$data}");
    $active = 0;
    $total = 0;
    if ($response->code == 200) {
      $data = drupal_json_decode($response->data);
      if (isset($data['members'])) {
        $total = count($data['members']);
        $active = count(array_filter($data['members'], function ($user) {
          return (isset($user['presence']) && $user['presence'] == 'active');
        }));
      }

    }
    return [
      'link' => "https://{$this->hostname}.slack.com",
      'active' => $active,
      'total' => $total,
    ];
  }

  /**
   * Метод, который вызывает остальные по строковому значению.
   * Через него все кастомные команды
   *
   * @param $action
   *   Название метода
   * @param $args
   *   Массив аргументов для выполнения команды
   *
   * @return array
   *   Ответ для передачи сообщения в Slack
   */
  public function run($action, $args) {
    if (is_callable([$this, $action])) {
      return $this->$action($args);
    }
    else {
      return [
        'username' => 'Командос',
        "attachments" => [
          [
            "color" => "#ff9933",
            "text" => t("	:radioactive_sign: Мне весьма жаль, но Вы уверены, что есть команда *{$action}*?"),
          ],
        ],

      ];
    }
  }

  /**
   * Примеры для создания своих команд для бота с аргументами и без
   *
   * public function call_without_args($args) {
   *   return ['text' => t("This message return after call the method **call_without_args**")];
   * }
   *
   * public function call_with_args($args) {
   *   return ['text' => t("This message return after call the method **call_with_args** с аргументами {implode(', ', $args)}")];
   * }

  */

  public function invite_user($args) {
    $args_len = count($args);
    $message = [
      'username' => 'Приглашатель 3000',
      "attachments" => []
    ];
    if ($args_len == 0) {
      $message['attachments'][0]['pretext'] = 'Ну и чё ты ждёшь? Кому я, по-твоему, должен отправить инвайт?';
    } elseif ($args_len == 1) {
        $invite = $this->invite($args[0]);
        switch ($invite['status']) {
          case 'warning':
            $color = '#ff7909';
            break;
          case 'error':
            $color = '#f00';
            break;
          default:
            $color = '#390';
            break;
        }
        $message['attachments'][0]['pretext'] = 'Отнёс лично в отделение Почты России. Во, чё сказали:';
        $message['attachments'][0]['color'] = $color;
        $message['attachments'][0]['text'] = $invite['message'];
    } else {
      $message['attachments'][0]['pretext'] = 'Помедленнее, пжлста... я записываю';
    }
    return $message;
  }
}