<?php

/**
 * Callback dru_tnx_entity
 *
 * @param string $tnx_type
 * @param string $entity_type
 * @param string $ajax
 * @param string $entity
 * @param string $token
 *
 * @return array
 */
function dru_tnx_entity($tnx_type, $entity_type, $ajax, $entity, $token) {
  if (!drupal_valid_token($token, 'dru_tnx')) {
    return NULL;
  }

  $user = $GLOBALS['user'];
  $id_type = ($entity_type == 'node') ? 'nid' : 'cid';
  $id = $entity->$id_type;
  $tnx = dru_tnx_get_user_tnx($entity_type, $id, $user->uid);

  if ($tnx === ($tnx_type == 'tnx')) {
    $vars = [
      'type'     => $entity_type,
      'id'       => $id,
      'id_type'  => $id_type,
      'entity'   => $entity,
      'tnx_type' => $tnx_type,
      'tnx'      => $tnx,
      'user'     => $user,
    ];

    return dru_tnx_callback($ajax, $vars);
  }

  drupal_set_message(
    $tnx ? t("You can't unthanks twice!") : t("You can't thanks twice!")
  );
  drupal_goto();
}

/**
 * dru_tnx_callback
 */
function dru_tnx_callback($ajax, $vars) {

  dru_tnx_data($vars);
  $entity = dru_tnx_entity_prepare($vars);

  if ($ajax == 'ajax') {
    $commands = [];
    $animate = ['plus', 'minus'];
    $selector = "*[data-{$vars['type']}-id=\"{$vars['id']}\"]";
    $commands[] = ajax_command_replace($selector, drupal_render($entity));
    $commands[] = ajax_command_invoke(
      $selector . " .dru-tnx-{$vars['type']}-{$vars['id']}-counter",
      'addClass',
      [$animate[($vars['tnx_type'] == 'untnx')]]
    );

    return [
      '#type'     => 'ajax',
      '#commands' => $commands,
    ];
  }

  drupal_set_message($vars['tnx'] == 'tnx' ? t('Your Thank Accepted') : t('Your Thank Deleted'));
  drupal_goto();
}

/**
 * Render entity.
 *
 * @param array $vars
 *
 * @return array
 */
function dru_tnx_entity_prepare($vars) {
  $entity = [];

  if ($vars['type'] == 'node') {
    $node = node_load($vars['id'], NULL, TRUE);
    $entity = node_view($node);
    $entity['comments'] = comment_node_page_additions($node);
    $entity['#node']->dru_tnx_set_page_true = TRUE;
  }
  elseif ($vars['type'] == 'comment') {
    $comment = comment_load($vars['id'], TRUE);
    $node = node_load($comment->nid);
    $entity = comment_view($comment, $node);
  }

  return $entity;
}
