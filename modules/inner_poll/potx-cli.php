#!/usr/bin/php
<?php
// $Id: potx-cli.php,v 1.1.2.6.4.3 2008/09/09 12:50:24 goba Exp $

/**
 * @file
 *   Translation template generator for Drupal (command line version).
 *
 *   Extracts translatable strings from t(), t(,array()), format_plural()
 *   and other function calls, plus adds some file specific strings. Only
 *   literal strings with no embedded variables can be extracted. Generates
 *   POT files, errors are printed on STDERR.
 */

if (isset($_SERVER['REQUEST_METHOD'])) {
  // Try to prevent running this script from the web. It is not designed so.
  print 'The potx-cli.php script is designed to be used from the command line. Please use the Drupal module web interface to extract strings through the web, instead of this script, if you prefer a web interface.';
}

// Functions shared with web based interface
include dirname(__FILE__) .'/potx.inc';

// We need a lot of resources probably, so try to set memory 
// limit higher and set unlimited time for our work.
$memory_limit = @ini_get('memory_limit');
if ($memory_limit != '' && (int)$memory_limit < 16) {
  // ini_get returns the original set value, such as "32M",
  // so we check for the int version. Before PHP 5.2, this
  // limit was less then 16M. 
  @ini_set('memory_limit', 16777216);
}
@set_time_limit(0);

if (!defined("STDERR")) {
  define('STDERR', fopen('php://stderr', 'w'));
}

$files = array();
$build_mode = POTX_BUILD_SINGLE;
$argv = $GLOBALS['argv'];
array_shift ($argv);
if (count($argv)) {
  switch ($argv[0]) {
    case '--help' :
      print <<<END
Drupal command line translation template generator
Usage: potx-cli.php [OPTION]

Possible options:
 --auto
     Autodiscovers files in current folder (default).
 --files
     Specify a list of files to generate templates for.
 --mode=core
     Core extraction mode, .info files folded into general.pot.
 --mode=multiple
     Multiple file output mode, .info files folded into module pot files.
 --mode=single
     Single file output mode, every file folded into the single outpout file (default).
 --debug
     Only perform a 'self test'.
 --help
     Display this message.
 
END;
      return 1;
      break;
    case '--files' :
      array_shift($argv);
      $files = $argv;
      break;
    case '--mode=core' :
      $build_mode = POTX_BUILD_CORE;
      break;
    case '--mode=multiple' :
      $build_mode = POTX_BUILD_MULTIPLE;
      break;
    case '--mode=single' :
      $build_mode = POTX_BUILD_SINGLE;
      break;
    case '--debug' :
      $files = array(__FILE__);
      break;
    case '--auto' :
      $files = _potx_explore_dir();
      break;
  }
}

// Fall back to --auto, if --files are not specified
if (empty($files)) {
  $files = _potx_explore_dir();
}

foreach ($files as $file) {
  potx_status('status', "Processing $file...\n");
  _potx_process_file($file);
}

_potx_build_files(POTX_STRING_RUNTIME, $build_mode);
_potx_build_files(POTX_STRING_INSTALLER, POTX_BUILD_SINGLE, 'installer');
_potx_write_files();
potx_status('status', "\nDone.\n");

return;

// These are never executed, you can run potx-cli.php on itself to test it
// -----------------------------------------------------------------------------

$a = t("Test string 1" );
$b = t("Test string 2 %string", array("%string" => "how do you do"));
$c = t('Test string 3');
$d = t("Special\ncharacters");
$e = t('Special\ncharacters');
$f = t("Embedded $variable");
$g = t('Embedded $variable');
$h = t("more \$special characters");
$i = t('even more \$special characters');
$j = t("Mixed 'quote' \"marks\"");
$k = t('Mixed "quote" \'marks\'');
$l = t('This is some repeating text');
$m = t("This is some repeating text");
$n = t(embedded_function_call());
$o = format_plural($days, "one day", "@count days");
$p = format_plural(embedded_function_call($count), "one day", "@count days");

function embedded_function_call($dummy) { return 12; }

function potxcli_perm() {
  return array("access potx data", 'administer potx data');
}

function potxcli_help($section = 'default') {
  watchdog('help', t('Help called'));
  return t('This is some help');
}

function potxcli_node_types() {
  return array("extractor-cooltype", "extractor-evencooler");
}
