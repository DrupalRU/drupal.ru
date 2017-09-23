<?php
/**
 * Run the BBCode parser stand alone on text data for testing/ debugging purposes.
 * It also severs as an example of how to use it outside of Drupal.
 */

// Load the BBCode parser
include_once 'bbcode-filter.inc';
$settings = array(
	'bbcode_paragraph_breaks' => 2,
	'bbcode_encode_mailto' => 1,
	'bbcode_make_links' => 1,
	'bbcode_filter_nofollow' => 1
);
function t($msg) { return $msg; }

// Read test data
$body = file_get_contents('bbcode-test.txt');

// Present the CSS
print "<html>\n<title>BBCode Test</title>\n<style>\n";
print file_get_contents('bbcode-test.css');
print "</style>\n<body>\n";

// Run parser and present output
$time_start = microtime(true);
print _bbcode_filter_process($body, $settings);
$time_end = microtime(true);

// Report execution time
$time = $time_end - $time_start;
print "<hr>Process time: {$time}\n";
	
print "</body>\n</html>\n";
