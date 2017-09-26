<?php


function druru_mark($variables) {
  $type = $variables['type'];
  global $user;
  if ($user->uid) {
    if ($type == MARK_NEW) {
      return ' <sup class="marker text-success">' . t('new') . '</sup>';
    }
    elseif ($type == MARK_UPDATED) {
      return ' <sup class="marker text-success">' . t('updated') . '</sup>';
    }
  }
}