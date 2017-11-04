<?php

/**
 * Return node 127170 (Moderation and responsibility).
 */
function dru_ticket_moderate_help() {
  $node = node_load(127170);
  $commands = [];
  $render_form = '<div id="ticket-help">';
  $render_form .= '<div id="close-form">×</div>';
  $render_form .= '<h2>';
  $render_form .= $node->title;
  $render_form .= '</h2>';
  $render_form .= check_markup($node->body['und'][0]['value'], $node->body['und'][0]['format'], '', TRUE);
  $render_form .= '</div>';
  $commands[] = ajax_command_append('body', '<div id="verdict-add">' . $render_form . '</div>');

  return [
    '#type' => 'ajax',
    '#commands' => $commands,
  ];
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
  $commands = [];
  switch ($entity_type) {
    case 'node':
      $node = node_load($entity_id);
      $node->status = NODE_NOT_PUBLISHED;
      node_save($node);
      $node_link = l(t('Publish'), 'admin/moderate/command/publish/node/' . $entity_id, [
        'attributes' => [
          'class' => [
            'use-ajax',
            'moderate-publish',
            'moderate-publish-' . $entity_id,
          ],
        ],
      ]);
      $commands[] = ajax_command_replace('.moderate-unpublish-' . $entity_id, $node_link);
      $commands[] = ajax_command_invoke('#node-' . $entity_id, 'addClass', ['node-unpublished']);
      break;

    case 'comment':
      $comment = comment_load($entity_id);
      $comment->status = COMMENT_NOT_PUBLISHED;
      comment_save($comment);
      $comment_link = l(t('Publish'), 'admin/moderate/command/publish/comment/' . $entity_id, [
        'attributes' => [
          'class' => [
            'use-ajax',
            'moderate-publish',
            'moderate-publish-' . $entity_id,
          ],
        ],
      ]);
      $commands[] = ajax_command_replace('.moderate-unpublish-' . $entity_id, $comment_link);
      $commands[] = ajax_command_invoke('.darkmatter-comment-' . $entity_id, 'addClass', ['comment-unpublished']);
      break;
  }

  return [
    '#type' => 'ajax',
    '#commands' => $commands,
  ];
}

/**
 * Moderate command callback - publish.
 */
function dru_ticket_moderate_command_publish($entity_type, $entity_id) {
  $commands = [];
  switch ($entity_type) {
    case 'node':
      $node = node_load($entity_id);
      $node->status = NODE_PUBLISHED;
      node_save($node);
      $node_link = l(t('Unpublish'), 'admin/moderate/command/unpublish/node/' . $entity_id, [
        'attributes' => [
          'class' => [
            'use-ajax',
            'moderate-unpublish',
            'moderate-unpublish-' . $entity_id,
          ],
        ],
      ]);
      $commands[] = ajax_command_replace('.moderate-publish-' . $entity_id, $node_link);
      $commands[] = ajax_command_invoke('#node-' . $entity_id, 'removeClass', ['node-unpublished']);
      break;

    case 'comment':
      $comment = comment_load($entity_id);
      $comment->status = COMMENT_PUBLISHED;
      comment_save($comment);
      $comment_link = l(t('Unpublish'), 'admin/moderate/command/unpublish/comment/' . $entity_id, [
        'attributes' => [
          'class' => [
            'use-ajax',
            'moderate-unpublish',
            'moderate-unpublish-' . $entity_id,
          ],
        ],
      ]);
      $commands[] = ajax_command_replace('.moderate-publish-' . $entity_id, $comment_link);
      $commands[] = ajax_command_invoke('.darkmatter-comment-' . $entity_id, 'removeClass', ['comment-unpublished']);
      break;
  }

  return [
    '#type' => 'ajax',
    '#commands' => $commands,
  ];
}
