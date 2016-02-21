<?php

// Test
// php install.drupalru.php --github_url=https://github.com/mbaev/drupal.ru --github_branch=stage --site_path=/var/www/drupal.loc --mysql_host=127.0.0.1 --mysql_user=root --mysql_db=drupal --mysql_pass="111" --domain=drupal.loc --account_name=admin --account_email=admin@example.com --account_pass=111

define('MAKE_SCRIPT_URL', 'https://raw.githubusercontent.com/DrupalRu/drupal.ru/stage/scripts/drupalru.make');


################ Helpers ################

/**
 * Get user input for variables.
 */
function to_ask($promt) {
  if (is_windows() or !function_exists('readline')) {
    print $promt . ': ';
    $line = stream_get_line(STDIN, 1024, PHP_EOL);
  }
  else {
    $line = readline($promt . ': ');
  }
  return $line;
}

function check_exception($condition, $message = NULL) {
  if (!$condition) {
    if ($message) {
      to_show($message);
    }
    exit;
  }
}

function is_windows() {
  return PHP_OS == 'WINNT';
}

function title($title) {
  to_show('');
  to_show('################## ' . $title . ' ##################');
}

function to_show($message) {
  print call_user_func_array('format', func_get_args()) . PHP_EOL;
}

function format($message) {
  $args = func_get_args();
  $args[0] = is_array($args[0]) ? implode(PHP_EOL, $args[0]) : $args[0];
  return call_user_func_array('sprintf', $args);
}

function _do($script, $params) {
  exec(strtr($script, $params));
}

function param_to_opt($opt) {
  return $opt . '::';
}

function execute_func($param) {
  $result = NULL;
  if (is_string($param) && function_exists($param)) {
    $result = call_user_func($param);
  }
  elseif (is_array($param)) {
    $function = array_shift($param);
    $result = call_user_func_array($function, $param);
  }
  return $result;
}

function command_exists($command) {
  $process = proc_open("which $command", array(
    array("pipe", "r"),
    array("pipe", "w"),
    array("pipe", "r"),
  ), $pipes);
  $exists = stream_get_contents($pipes[1]);
  proc_close($process);
  return !empty($exists);
}


################ Custom functions. ################

function check_drush_version() {
  $process = proc_open("drush --version", array(
    array("pipe", "r"),
    array("pipe", "w"),
    array("pipe", "r"),
  ), $pipes);
  $result = stream_get_contents($pipes[1]);
  proc_close($process);
  preg_match("/\d(.*)/", $result, $version);
  $version = isset($version[0]) ? trim($version[0]) : FALSE;

  if (FALSE === $version) {
    check_exception(FALSE, 'Can not detect drush version');
  }

  if(!version_compare(trim($version), '6.0', '>=')){
    check_exception(FALSE, "Your drush version {$version}, but needed more than 6.0");
  }
}

function check_root($root) {
  if (!is_dir($root)) {
    mkdir($root);
  }
}

function to_docroot($data) {
  check_root($data['site_path']);
  chdir($data['site_path']);
}


################ System functions ################

function get_params() {
  // List of params which need to get from user.
  // Example:
  // 'param_name' => array(
  //    'question' => "Question for show to user",
  //    'default'  => "Default value for the param",
  // ).
  $params = array(
    'github_url' => array(
      'question' => 'Provide url to your drupal.ru fork (Example: "https://github.com/DrupalRu/drupal.ru")',
    ),
    'github_branch' => array(
      'question' => 'Branch name',
      'default' => 'stage',
    ),
    'domain' => array(
      'question' => 'Domain',
      'default' => 'drupal.loc',
    ),
    'site_path' => array(
      'question' => 'Site directory',
      'default' => __DIR__,
    ),
    'mysql_host' => array(
      'question' => 'MySQL Host',
      'default' => 'localhost',
    ),
    'mysql_user' => array(
      'question' => 'MySQL User',
      'default' => 'root',
    ),
    'mysql_db' => array(
      'question' => 'MySQL DB',
      'default' => 'drupal',
    ),
    'mysql_pass' => array(
      'question' => 'MySQL Password',
    ),
    'account_name' => array(
      'question' => 'Drupal User name',
      'default' => 'admin',
    ),
    'account_email' => array(
      'question' => 'Drupal User email',
      'default' => 'admin@example.com',
    ),
    'account_pass' => array(
      'question' => 'Drupal User Password',
      'default' => '123',
    ),
    'site_name' => array(
      'question' => 'Site name',
      'default' => 'Drupal.ru Dev version',
    ),
  );

  // Get params from request.
  $data = getopt('', array_map('param_to_opt', array_keys($params)));

  // Check params.
  foreach ($params as $key => &$param) {
    if (!isset($data[$key])) {
      if (isset($param['default'])) {
        $param['question'] = format($param['question'] . ' [%s]', $param['default']);
      }
      $data[$key] = to_ask($param['question']);
      if (!$data[$key] && isset($param['default'])) {
        $data[$key] = $param['default'];
      }
    }
    else {
      to_show("{$param['question']}: %s", $data[$key]);
    }

    if ('domain' == $key) {
      $params['site_path']['default'] .= '/' . $data[$key];
    }
  }
  // Some static variables.
  $data['github_path'] = 'profiles/drupalru';
  to_show("Github DIR: '%s'", $data['github_path']);
  return $data;
}

function rules($data) {
  $ghp = $data['github_path'];
  return array(
    array(
      'title' => 'Download Drupal',
      'do before' => array('to_docroot', $data),
      'commands' => array(
        array('drush -y make %s', MAKE_SCRIPT_URL),
        array('git clone -b  %s %s profiles/drupalru', $data['github_branch'], $data['github_url']),
      ),
    ),
    array(
      'title' => 'Install Drupal',
      'commands' => array(
        array(
          'drush site-install drupalru -y --root=%s --account-name=%s' .
          ' --account-mail=%s --account-pass=%s --uri=http://%s --site-name="%s"' .
          ' --site-mail=%s --db-url=mysql://%s:%s@%s/%s',
          $data['site_path'],
          $data['account_name'],
          $data['account_email'],
          $data['account_pass'],
          $data['domain'],
          $data['site_name'],
          $data['account_email'],
          $data['mysql_user'],
          $data['mysql_pass'],
          $data['mysql_host'],
          $data['mysql_db'],
        ),
      ),
    ),
    array(
      'do before' => array('to_docroot', $data),
      'title' => 'Import roles',
      'validate' => array(
        "Dir $ghp not found" => array('is_dir', $ghp),
      ),
      'commands' => array(
        array('drush ddi roles --file=%s/root/roles.export', $ghp),
      ),
    ),
    array(
      'title' => 'Import filters',
      'commands' => array(
        array('drush ddi filters --file=%s/root/filters.export', $ghp),
      ),
    ),
    array(
      'title' => 'Import content types',
      'commands' => array(
        array('drush ddi node_types --file=%s/root/blog.node_types.export', $ghp),
        array('drush ddi node_types --file=%s/root/organization.node_types.export', $ghp),
        array('drush ddi node_types --file=%s/root/simple_event.node_types.export', $ghp),
        array('drush ddi node_types --file=%s/root/ticket.node_types.export', $ghp),
      ),
    ),
    array(
      'title' => 'Import taxonomy',
      'commands' => array(
        array('drush ddi taxonomy --file=%s/root/vocabulary_1.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/vocabulary_2.taxonomy.export',$ghp),
        array('drush ddi taxonomy --file=%s/root/vocabulary_3.taxonomy.export',$ghp),
        array('drush ddi taxonomy --file=%s/root/vocabulary_4.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/vocabulary_5.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/vocabulary_6.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/vocabulary_7.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/vocabulary_8.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/vocabulary_9.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/vocabulary_10.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/claim_category.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/event_types.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/organizations.taxonomy.export', $ghp),
        array('drush ddi taxonomy --file=%s/root/ticket_status.taxonomy.export', $ghp),
      ),
    ),
    array(
      'title' => 'Import forum',
      'commands' => array(
        array('drush ddi forum --file=%s/root/forum.export', $ghp),
      ),
    ),
    array(
      'title' => 'Import menu structure',
      'commands' => array(
        array('drush ddi menu --file=%s/root/main-menu.menu_links.export', $ghp),
        array('drush ddi menu --file=%s/root/user-menu.menu_links.export', $ghp),
      ),
    ),
    array(
      'title' => 'Import theme blocks settings',
      'commands' => array(
        array('drush ddi blocks --file=%s/root/alpha.blocks.export', $ghp),
      ),
    ),
    array(
      'title' => 'Import theme settings',
      'commands' => array(
        array('drush ddi variables --file=%s/root/theme_settings.variables.export', $ghp),
        array('drush ddi variables --file=%s/root/theme_alpha_settings.variables.export', $ghp),
      ),
    ),
    array(
      'title' => 'Import modules settings',
      'commands' => array(
        array('drush ddi variables --file=%s/root/advanced_sphinx.variables.export', $ghp),
        array('drush ddi variables --file=%s/root/darkmatter_notify.variables.export', $ghp),
        array('drush ddi variables --file=%s/root/dru_frontpage.variables.export', $ghp),
        array('drush ddi variables --file=%s/root/resolve_can.variables.export', $ghp),
        array('drush ddi variables --file=%s/root/user_info_notify.variables.export', $ghp),
        array('drush ddi variables --file=%s/root/quote.variables.export', $ghp),
        array('drush ddi variables --file=%s/root/validate_api.variables.export', $ghp),
      ),
    ),
    array(
      'title' => 'Disable drupal_deploy',
      'commands' => array(
        array('drush dis -y drupal_deploy'),
      ),
    ),
    array(
      'title' => 'Generate content and users',
      'commands' => array(
        array('drush generate-users 100'),
        array('drush generate-content 100 100'),
      ),
    ),
    array(
      'title' => 'Update translation',
      'commands' => array(
        array('drush -y dl drush_language'),
        array('drush language-add ru'),
        array('drush language-default ru'),
        array('drush -y l10n-update-refresh'),
        array('drush -y l10n-update'),
        array('drush -y language-import ru %s/modules/user_filter/user_filter_notify/translations/user_filter_notify.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/validate_api/translations/validate_api.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/validate_api/antiswearing_validate/translations/antiswearing_validate.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/validate_api/antinoob_validate/translations/antinoob_validate.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/darkmatter/translations/darkmatter.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/dru_tickets/dru_claim/translations/dru_claim.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/dru_tickets/translations/dru_tickets.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/simple_events/translations/simple_events.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/user_filter/user_filter_notify/translations/user_filter_notify.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/resolve/translations/resolve.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/marketplace/translations/marketplace.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/dru_tnx/translations/dru_tnx.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/validate_api/translations/validate_api.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/validate_api/antiswearing_validate/translations/antiswearing_validate.ru.po', $ghp),
        array('drush -y language-import ru %s/modules/validate_api/antinoob_validate/translations/antinoob_validate.ru.po', $ghp),
      ),
    ),
  );

}

function install($rules) {
  foreach ($rules as $rule) {

    if (isset($rule['validate'])) {
      foreach ($rule['validate'] as $message => $validate_rule) {
        check_exception(execute_func($validate_rule), $message);
      }
    }

    if (isset($rule['do before'])) {
      execute_func($rule['do before']);
    }
    if (isset($rule['title'])) {
      title($rule['title']);
    }
    if (isset($rule['commands'])) {
      foreach ($rule['commands'] as $command) {
        exec(call_user_func_array('sprintf', $command));
      }
    }
    if (isset($rule['do after'])) {
      execute_func($rule['do after']);
    }
  };
}

to_show("");
to_show("/*********************************************/");
to_show("/*     This is install script to create      */");
to_show("/*    dev environment for drupal.ru code.    */");
to_show("/*********************************************/");
to_show("");

check_exception(command_exists('drush'), 'You do not have installed drush.');
check_drush_version();
check_exception(command_exists('git'), 'You do not have installed git.');

$params = get_params();
$rules = rules($params);
install($rules);

