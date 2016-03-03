<?php

/**
 * Return node 127170 (Moderation and responsibility).
 */
function dru_ticket_moderate_help() {
  $node= node_load(127170);

  $commands = array();

  $render_form  = '<div id="ticket-help">';
  $render_form .= '<div id="close-form"><i class="fa fa-times"></i></div>';
  $render_form .= '<h2>';
  $render_form .= $node->title;
  $render_form .= '</h2>';
  $render_form .= check_markup($node->body['und'][0]['value'], $node->body['und'][0]['format'], '', TRUE);
  $render_form .= '</div>';

  $commands[] = ajax_command_append('body', '<div id="verdict-add">' . $render_form . '</div>');

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Moderate commands.
 */
function dru_ticket_moderate_command($command, $entity_type, $entity_id) {
  $dru_ticket_moderate_command_callback = 'dru_ticket_moderate_command_' . $command;
  return $dru_ticket_moderate_command_callback($entity_type, $entity_id);
}

/**
 * Moderate command callback - unpublish.
 */
function dru_ticket_moderate_command_unpublish($entity_type, $entity_id) {

  $commands = array();
  
  switch ($entity_type) {
    case 'node':
      $node = node_load($entity_id);
      $node->status = NODE_NOT_PUBLISHED;
      node_save($node);
        
      $node_link = l(t('Publish'), 'admin/moderate/command/publish/node/' . $entity_id, array('attributes' => array('class' => array('use-ajax', 'moderate-publish', 'moderate-publish-' . $entity_id))));

      $commands[] = ajax_command_replace('.moderate-unpublish-' . $entity_id, $node_link);

      $commands[] = ajax_command_invoke('#node-' . $entity_id, 'addClass', array('node-unpublished'));
      break;
    case 'comment':
      $comment = comment_load($entity_id);
      $comment->status = COMMENT_NOT_PUBLISHED;
      comment_save($comment);
        
      $comment_link = l(t('Publish'), 'admin/moderate/command/publish/comment/' . $entity_id, array('attributes' => array('class' => array('use-ajax', 'moderate-publish', 'moderate-publish-' . $entity_id))));

      $commands[] = ajax_command_replace('.moderate-unpublish-' . $entity_id, $comment_link);

      $commands[] = ajax_command_invoke('.darkmatter-comment-' . $entity_id, 'addClass', array('comment-unpublished'));
      break;
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Moderate command callback - publish.
 */
function dru_ticket_moderate_command_publish($entity_type, $entity_id) {

  $commands = array();
  
  switch ($entity_type) {
    case 'node':
      $node = node_load($entity_id);
      $node->status = NODE_PUBLISHED;
      node_save($node);
        
      $node_link = l(t('Unpublish'), 'admin/moderate/command/unpublish/node/' . $entity_id, array('attributes' => array('class' => array('use-ajax', 'moderate-unpublish', 'moderate-unpublish-' . $entity_id))));

      $commands[] = ajax_command_replace('.moderate-publish-' . $entity_id, $node_link);

      $commands[] = ajax_command_invoke('#node-' . $entity_id, 'removeClass', array('node-unpublished'));
      break;
    case 'comment':
      $comment = comment_load($entity_id);
      $comment->status = COMMENT_PUBLISHED;
      comment_save($comment);
        
      $comment_link = l(t('Unpublish'), 'admin/moderate/command/unpublish/comment/' . $entity_id, array('attributes' => array('class' => array('use-ajax', 'moderate-unpublish', 'moderate-unpublish-' . $entity_id))));

      $commands[] = ajax_command_replace('.moderate-publish-' . $entity_id, $comment_link);
      $commands[] = ajax_command_invoke('.darkmatter-comment-' . $entity_id, 'removeClass', array('comment-unpublished'));
      break;
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Moderate command callback - promote.
 */
function dru_ticket_moderate_command_promote($entity_type, $entity_id) {

  $commands = array();
  
  $node = node_load($entity_id);
  $node->promote = NODE_PROMOTED;
  node_save($node);
        
  $node_link = l(t('Unpromote'), 'admin/moderate/command/unpromote/node/' . $entity_id, array('attributes' => array('class' => array('use-ajax', 'moderate-unpromote', 'moderate-unpromote-' . $entity_id))));

  $commands[] = ajax_command_replace('.moderate-promote-' . $entity_id, $node_link);

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Moderate command callback - unpromote.
 */
function dru_ticket_moderate_command_unpromote($entity_type, $entity_id) {

  $commands = array();
  
  $node = node_load($entity_id);
  $node->promote = NODE_NOT_PROMOTED;
  node_save($node);
        
  $node_link = l(t('Promote'), 'admin/moderate/command/promote/node/' . $entity_id, array('attributes' => array('class' => array('use-ajax', 'moderate-promote', 'moderate-promote-' . $entity_id))));

  $commands[] = ajax_command_replace('.moderate-unpromote-' . $entity_id, $node_link);

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}
