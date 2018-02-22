<?php

/**
 * Implements hook_help().
 */
function dru_slack_help($path, $arg) {
  $output = '';
  if ($path == 'admin/help#dru_slack') {
    $output .= '<h2>Первоначальная настройка</h2>';
    $output .= '<ol>';
    $output .= "<li><p>У Вас должен быть уже настроенный <b>Workspace</b> в Slack,
                либо можете <a href='https://slack.com/create'>создать новый</a>.<br>
                (<em>Для настройки модуля Вы должны обладать правами не ниже администратора.</em>)
                </p></li>";
    $output .= "<li><p>Далее необходимо <a href='https://api.slack.com/apps'>создать приложение</a></p></li>";
    $output .= "<li><p>Для отправки инвайтов в Ваш workspace на электронный адрес
                пользователей так же потребуется наличие токена.<br>
                Сгенерировать новый токен для Вашего workspace можно на этой 
                <a href='https://api.slack.com/custom-integrations/legacy-tokens'>странице</a>
                </p></li>";
    $output .= "<li><p>Заполните поля <b>workspace</b> и <b>token</b> 
                соответствующими значениями на <a href='".url('admin/config/dru_slack/settings')."'>странице настроек модуля</a>
                </p></li>";
    $output .= '</ol>';

    $output .= '<h2>Инвайты</h2>';
    $output .= '<p>Для отправки инвайтов в Ваш workspace на электронный адрес
                пользователей используется метод <b>invite($email)</b> 
                основного класса для работы с API Slack <b>slackAPI</b>.
                Его можно вызвать в сабмите любой Вашей формы. Пример использования: 
                </p>';
    $output .= '<pre>
        
        require_once drupal_get_path("module", "dru_slack") . "/inc/slackAPI.php.inc";
        
        function custom_form_submit($form, &$form_state) {
          $email = $form_state["values"]["email"];
          $slack = new slackAPI();
          $result = $slack->invite($email);
          drupal_set_message($result["message"], $result["status"]);
        }
     
                </pre>';

    $output .= '<h2>Команды из Slack</h2>';
    $output .= '<p>Для обработки команд из Slack существует динамический роут
                <b>slack/callback/{method_name}</b>, где <em>method_name</em> -
                название метода класса <b>slackAPI</b>.
                </p>';
    $output .= '<p>Команды могут использовать аргументы или обходиться без них.
                Аргументы передаются через пробел в команде и обрабатываются на бекэнде, как обыкновенный массив.
                Для создания своих методов для команд используйте примеры из класса <b>slackAPI</b>:
                <em>call_without_args</em> и <em>call_with_args</em>.
                </p>';
    $output .= '<p>Свои команды Вы можете добавить в разделе <em>Slash commands</em>
                на странице настроек <a href="https://api.slack.com/apps">вашего приложения</a>.
                В <b>Request URL</b> нужно указать <em>{URL_сайта}/slack/callback/{название вашего метода для данной команды}</em>.
                Остальное по желанию.
                </p>';

    $output .= "<p>Для предотвращения несанкционированного доступа к использованию команд лицами, не имеющими на то право,
                на <a href='".url('admin/config/dru_slack/settings')."'>странице настроек модуля</a> 
                укажите через запятую ID участников Вашего workspace, которые будут иметь право на выполнение команд в соответствующем поле.
                </p>";

    $output .= '<h2>Сообщения в каналы Slack</h2>';
    $output .= '<p>Модуль может слать сообщения в различные каналы Вашего workspace.
                Эта возможность реализована двумя методами класса <b>slackAPI</b>:
                </p>';
    $output .= '<ul>';
    $output .= '<li><b>notificate_by_chanel</b> - по названию канала</li>';
    $output .= '<li><b>notificate_by_url</b> - по урлу</li>';
    $output .= '</ul>';
    $output .= '<p>Вторым аргументом методы принимают ассоциативный массив сообщения.
                Подробнее о формате сообщений можно прочитать в 
                <a href="https://api.slack.com/docs/message-formatting">официальной документации</a>.
                Пример использования:
                </p>';
    $output .= '<pre>
                    require_once drupal_get_path("module", "dru_slack") . "/inc/slackAPI.php.inc";
                    $slack = new slackAPI();
                    $ message = [
                      "attachments" => [
                          [
                            "title"     => "Title",
                            "pretext"   => "Pretext _supports_ mrkdwn",
                            "text"      => "Testing *right now!*",
                            "mrkdwn_in" => ["text", "pretext"]
                          ]
                      ]
                    ];
                    $slack->notificate_by_chanel("general", $message);
                </pre>';

    $output .= '<p>Интерфейс для управления доступными каналами для сообщений
                Вы можете найти в разделе <em>Incoming Webhooks</em>
                на странице настроек <a href="https://api.slack.com/apps">вашего приложения</a>.
                Аналогично укажите на <a href='.url("admin/config/dru_slack/settings").'>странице настроек модуля</a>
                маппинг <b>Канал->URL</b>(<em>Название канала будет использовано в качестве аргумента для <b>notificate_by_chanel()</b></em>).
                </p>';

    $output .= '<h2>Бейдж и информация об активности участников workspace</h2>';
    $output .= '<p>Для показа бейджа, например на страницах GitHub используйте url
                <b>{Ваш сайт}/slack/badge/{высота бейджа в px}/badge.svg</b>.
                Информация об активности участников реализована методом класса <b>slackAPI</b>
                <em>badge()</em>. Метод возвращает ассоциативный массив с ключами:
                </p>';
    $output .= '<ul>';
    $output .= '<li><b>link</b> - ссылка на Ваш workspace</li>';
    $output .= '<li><b>active</b> - количество активных участников</li>';
    $output .= '<li><b>total</b> - всего участников</li>';
    $output .= '</ul>';
  }
  return $output;
}
