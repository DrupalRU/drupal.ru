#!/usr/bin/php
<?php

$data['github_path'] = get_promt_answer('GITHUB DIR');  
$data['site_path'] = get_promt_answer('DOCROOT');
$data['mysql_user'] = get_promt_answer('MySQL User');
$data['mysql_db'] = get_promt_answer('MySQL DB');
$data['mysql_pass'] = get_promt_answer('MySQL Password');

print_r($data);

function get_promt_answer($promt){
  if (PHP_OS == 'WINNT' or !function_exists('readline')) {
    echo $promt .': ';
    $line = stream_get_line(STDIN, 1024, PHP_EOL);
  } else {
    $line = readline($promt . ': ');
  }
  return $line;
}
