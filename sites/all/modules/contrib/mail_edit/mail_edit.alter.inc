<?php

/**
 * @file
 * Implementation of hook_mail_alter() for the Mail Editor module.
 */

/**
 * Loads and returns a template.
 *
 * @param string $id
 * @param string|object $language
 * @param bool $force_always
 *
 * @return array|null|bool
 *   Returns FALSE if we get an invalid $id or $language,
 *   NULL if we have no template yet, or a keyed array with the default and/or
 *   customized templates.
 */
function _mail_edit_load($id, $language, $force_always = FALSE) {
  if (is_string($language)) {
    $languages = language_list();
    $language = (isset($languages[$language]) ? $languages[$language] : NULL);
  }
  if (!is_object($language)) {
    // If the language is unknown, we bail out.
    return FALSE;
  }
  // Find out if this mail id is in our registry.
  if (!cache_get('mail_edit_registry')) {
    _mail_edit_module_load_include('admin.inc');
    _mail_edit_key_registry_rebuild();
  }
  $query = db_select('mail_edit_registry', 'mer')
    ->fields('mer')
    ->condition('mer.id', $id);
  if (!$template_type = $query->execute()->fetchAssoc()) {
    // If we don't have any registry record we shouldn't attempt to alter it.
    return FALSE;
  }
  $module = $template_type['module'];
  $mailkey = $template_type['mailkey'];

  // Load mail template if we have altered it.
  $query = db_select('mail_edit', 'me')
    ->fields('me')
    ->condition('me.id', $id)
    ->condition('me.language', $language->language);
  $template = $query->execute()->fetchAssoc();
  $default_template = module_invoke($module, 'mail_edit_text', $mailkey, $language);
  if ($template) {
    if ($default_template) {
      $template += $default_template;
    }
  }
  else {
    if (!$default_template) {
      return NULL;
    }
    if (empty($default_template['always']) && !$force_always) {
      return NULL;
    }
    $template = $default_template + array('default' => TRUE);
  }
  $template += array(
    'language' => $language,
    'type'     => $template_type,
  );
  return $template;
}

/**
 * Implements hook_mail_alter().
 *
 * @param array $message
 */
function _mail_edit_mail_alter(array &$message) {
  //dpm($message, "BEFORE mail_edit_mail_alter({$message['id']})");
  if (!$template = _mail_edit_load($message['id'], $message['language'])) {
    return;
  }

  $data = (isset($message['params']['data']) ? $message['params']['data'] : $message['params']);
  $data['template'] = $template;
  if (isset($message['params']['account']) && !isset($data['user'])) {
    $data['user'] = $message['params']['account'];
  }
  $options = array(
    'language' => $message['language'],
    'clear'    => TRUE,
  );
  if ($message['module'] == 'user') {
    $options['callback'] = 'user_mail_tokens';
  }
  $subject = mail_edit_format($template['subject'], $data, $options);
  // #2185909: Remove newline character introduced by _drupal_wrap_mail_line().
  $message['subject'] = str_replace(array("\n"), '', trim(drupal_html_to_text($subject)));
  $context = (isset($message['params']['context']['mail_edit']) ? $message['params']['context']['mail_edit'] : NULL);
  $body = mail_edit_format($template['body'], $data, $options, $context);
  // Remove trailing spaces because these may be interpreted as soft line
  // breaks by the email client.
  $message['body'] = array(preg_replace('/ +(\r?\n)/', '\\1', $body));

  /* Uncomment this line for debugging...
  dpm($message, 'drupal_mail() is disabled in _mail_edit_mail_alter(), this would be sent');
  $message['to'] = '';
  /**/
}

/**
 * Preprocess a mail template, detecting conditional text
 * that conform to a prescribed syntax.
 *
 * @param string $template
 *  The template for preprocessing.
 * @param array $data
 *  $data parameter for token_replace();
 * @param array $options
 *  $options parameter for token_replace();
 * @param string $context
 *
 * @return string
 *
 * This function supports ternary-type conditions to determine what text
 * is used in a mail in a particular situation, based on token comparisons.
 * The syntax is standard PHP/C-style ternary syntax:
 *
 * {{leftOPright?true_text:false_text}}
 *
 * OP is any of the common comparison operators.
 * left and right must not contain a question mark or operator.
 * The true_text must not contain any colon, except as a token name.
 * true_text and false_text may contain newlines as well as a nested
 * conditional text (one level of recursion).
 */
function _mail_edit_preprocess($template, array $data, array $options, $context) {
  for ($n = 0; $n < 5 && $result = preg_match_all('/{{(?P<condition>[^?#@]+?)(?P<operator>\?|#|@)(?P<text>(({{(({{(({{(({{((.|\n)*?)}}|.|\n)*?)}}|.|\n)*?)}}|.|\n)*?)}}|.|\n)*?))}}/', $template, $clauses); ++$n) {
    //'(?P<text>((               .|\n)*?))';
    //'(?P<text>(({{((.|\n)*?)}}|.|\n)*?))';
    //'(?P<text>(({{(({{((.|\n)*?)}}|.|\n)*?)}}|.|\n)*?))';
    //'(?P<text>(({{(({{(({{((.|\n)*?)}}|.|\n)*?)}}|.|\n)*?)}}|.|\n)*?))';
    //'(?P<text>(({{(({{(({{(({{((.|\n)*?)}}|.|\n)*?)}}|.|\n)*?)}}|.|\n)*?)}}|.|\n)*?))';
    foreach ($clauses[0] as $i => $clause) {
      $replacement = '{{SYNTAX_ERROR}}';
      if ($clauses['operator'][$i] == '?') {
        if (!preg_match('/^(?P<true>(({{((({{((({{((({{((.|\n)*?)}})|.|\n)*?)}})|.|\n)*?)}})|.|\n)*?)}})|(\[[^]]*?\])|[^:[])*):(?P<false>(.|\n)*)$/', $clauses['text'][$i], $replacements)) {
          //'(?P<true> (    ({{((.|\n)*?)}})    |    (\[[^]]*?\])    |    [^:[]    )* )';
          //'(?P<true> (    ({{((({{((.|\n)*?)}})|.|\n)*?)}})    |    (\[[^]]*?\])    |    [^:[]    )* )';
          //'(?P<true> (    ({{((({{((({{((.|\n)*?)}})|.|\n)*?)}})|.|\n)*?)}})    |    (\[[^]]*?\])    |    [^:[]    )* )';
          //'(?P<true> (    ({{((({{((({{((({{((.|\n)*?)}})|.|\n)*?)}})|.|\n)*?)}})|.|\n)*?)}})    |    (\[[^]]*?\])    |    [^:[]    )* )';
          $template = str_replace($clause, $replacement, $template);
          continue;
        }
        if (preg_match('/^(?P<operand_1>.*?)\s*(?P<operator>==|!=|\<\>|\>=|\<=|\>|\<)\s*(?P<operand_2>.*?)$/', $clauses['condition'][$i], $condition)) {
          $operand1 = token_replace($condition['operand_1'], $data, $options + array('clear' => TRUE));
          $operand2 = token_replace($condition['operand_2'], $data, $options + array('clear' => TRUE));
          switch ($condition['operator']) {
            case '<>':
            case '!=':  $result = ($operand1 != $operand2);  break;
            case '==':  $result = ($operand1 == $operand2);  break;
            case '>':   $result = ($operand1 > $operand2);   break;
            case '<':   $result = ($operand1 < $operand2);   break;
            case '>=':  $result = ($operand1 >= $operand2);  break;
            case '<=':  $result = ($operand1 <= $operand2);  break;
            default:
              $result = NULL;
          }
          if (isset($result)) {
            $replacement = $replacements[$result ? 'true' : 'false'];
            $template = str_replace($clause, $replacement, $template);
            continue;
          }
        }
      }
      if (preg_match('/^(?P<not>!?!?)(?P<condition>(\[[^]]*?\])|[0-9]+|[A-Za-z0-9_]+)$/', $clauses['condition'][$i], $condition)) {
        $result = $condition['condition'];
        if ($result[0] == '[') {
          $result = token_replace($result, $data, $options + array('clear' => TRUE));
        }
        if ($not = $condition['not']) {
          $result = ($not == '!' ? !$result : !!$result);
        }
        if ($clauses['operator'][$i] == '?') {
          /** @noinspection PhpUndefinedVariableInspection */
          $replacement = $replacements[empty($result) ? 'false' : 'true'];
        }
        else {
          if (is_bool($result)) {
            $result = (int) $result;
          }
          if ($clauses['operator'][$i] == '#' && is_numeric($result)) {
            $replacement = '';
            for ($j = 0; $j < $result; ++$j) {
              $text = $clauses['text'][$i];
              if (!$not) {
                if (preg_match_all('/\[(?P<token>[^\]]*?):index:#0(?P<tail>.*?)\]/', $text, $tokens)) {
                  foreach ($tokens['token'] as $k => $token) {
                    $key = token_replace("[$token:keys:value:$j]", $data, $options);
                    $text = str_replace($tokens[0][$k], "[$token:value:$key{$tokens['tail'][$k]}]", $text);
                  }
                }
                $text = str_replace('#0', $j, $text);
                $text = str_replace('#1', $j+1, $text);
              }
              $replacement .= $text;
            }
          }
          elseif ($clauses['operator'][$i] == '@') {
            $replacement = ((($result == $context) xor $not) ? $clauses['text'][$i] : '');
          }
        }
      }
      $template = str_replace($clause, $replacement, $template);
    }
  }

  // Add white space support to [*:join:?] tokens.
  if (preg_match_all('/\[[^\]]*?:join:(?P<sep>[^\]]*?)\]/', $template, $join_tokens)) {
    $marker = token_replace("[random:hash:md5]");
    foreach ($join_tokens[0] as $i => $join_token) {
      $token = str_replace($join_tokens['sep'][$i], $marker, $join_tokens[0][$i]);
      $token = token_replace($token, $data, $options);
      $token = str_replace($marker, $join_tokens['sep'][$i], $token);
      $template = str_replace($join_tokens[0][$i], $token, $template);
    }
  }
  return $template;
}
