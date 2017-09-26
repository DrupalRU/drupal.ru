<?php

function druru_label($variables) {
  $type = $variables['type'];
  $tag  = $variables['tag'];
  return "<$tag class='label label-$type'>". $variables['text'] . "</$tag>";
}
