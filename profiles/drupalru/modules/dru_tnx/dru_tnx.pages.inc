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

  $id  = $entity_type == 'node' ? $entity->nid : $entity->cid;
  $tnx = get_user_tnx($entity_type, $id, $user->uid);

  if ($tnx === ($tnx_type == 'tnx')) {
    $vars = [
      'type'     => $entity_type,
      'id'       => $id,
      'entity'   => $entity,
      'tnx_type' => $tnx_type,
      'tnx'      => $tnx,
      'user'     => $user,
    ];

    return dru_tnx_callback($ajax, $vars);
  }

  drupal_set_message(
    $tnx ?
      t('You can\'t unthanks twice!') : t('You can\'t thanks twice!')
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

    $animate = ['plus', 'minus'];

    $selector = "*[data-{$vars['type']}-id=\"{$vars['id']}\"]";

    $commands   = [];
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
    $node                                   = node_load($vars['id'], NULL,
      TRUE);
    $entity                                 = node_view($node);
    $entity['comments']                     = comment_node_page_additions($node);
    $entity['#node']->dru_tnx_set_page_true = TRUE;
  }
  elseif ($vars['type'] == 'comment') {
    $comment = comment_load($vars['id'], TRUE);
    $node    = node_load($comment->nid);
    $entity  = comment_view($comment, $node);
  }

  return $entity;
}

/**
 * Callback dru_tnx_data
 */
function dru_tnx_data($vars) {

  // Counting tnx by entity
  $tnx_count = $vars['entity']->tnx;

  if ($vars['tnx'] == 'tnx') {
    $tnx_count++;

    // Add tnx
    db_insert('tnx')->fields(
      [
        'entity_id'   => $vars['id'],
        'entity_type' => $vars['type'],
        'uid'         => $vars['user']->uid,
      ]
    )->execute();

    // Count tnx
    db_merge('tnx_stat')
      ->key(['uid' => $vars['entity']->uid])
      ->fields([$vars['type'] => 1])
      ->expression($vars['type'], $vars['type'] . ' + :inc', [':inc' => 1])
      ->execute();

    // Count user tnx
    db_merge('tnx_stat')
      ->key(['uid' => $vars['user']->uid])
      ->fields(['tnx_' . $vars['type'] => 1])
      ->expression('tnx_' . $vars['type'], 'tnx_' . $vars['type'] . ' + :inc',
        [':inc' => 1])
      ->execute();
  }
  else {
    $tnx_count--;

    // Delete tnx
    db_delete('tnx')
      ->condition('entity_id', $vars['id'])
      ->condition('entity_type', $vars['type'])
      ->condition('uid', $vars['user']->uid)
      ->execute();

    if ($vars['entity']->tnx) {
      // Count tnx
      db_merge('tnx_stat')
        ->key(['uid' => $vars['entity']->uid])
        ->fields([$vars['type'] => 0])
        ->expression($vars['type'], $vars['type'] . ' - :inc',
          [':inc' => 1])
        ->execute();
      // Count user tnx
      db_merge('tnx_stat')
        ->key(['uid' => $vars['user']->uid])
        ->fields(['tnx_' . $vars['type'] => 0])
        ->expression('tnx_' . $vars['type'], 'tnx_' . $vars['type'] . ' - :inc',
          [':inc' => 1])
        ->execute();
    }
  }

  // Write tnx count to entity
  $query = db_update($vars['type']);
  $query->condition($vars['type'] == 'node' ? 'nid' : 'cid', $vars['id'])
        ->fields(['tnx' => $tnx_count])
        ->execute();
}
