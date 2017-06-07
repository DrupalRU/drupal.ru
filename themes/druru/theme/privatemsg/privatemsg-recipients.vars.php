<?php

function druru_preprocess_privatemsg_recipients(&$vars) {
  $title = drupal_get_title() . ' <small>' . $vars['participants'] . '</small>';
  drupal_set_title($title, PASS_THROUGH);
}
