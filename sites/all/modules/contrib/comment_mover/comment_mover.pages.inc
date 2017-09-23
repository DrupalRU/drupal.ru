<?php
/**
 * @file
 * This file contains callbacks for the hook_menu implementation.
 */

module_load_include('inc', 'comment_mover', 'includes/clipboard');
module_load_include('inc', 'comment_mover', 'includes/moving_bundle');

/**
 * A callback for the "cut" link.
 *
 * Adds an information about the cutted comment to the
 * $_SESSION['comment_mover'] variable.
 *
 * @param string $entity_type
 *   A type of the cutted entity: 'node' or 'comment'.
 * @param object $entity_id
 *   An ID of the entity for cutting into the clipboard
 * @param string $mode
 *   A flag to indicate that javascript is enabled ($mode = 'ajax' when enabled,
 *   $mode = 'nojs' otherwise)
 */
function comment_mover_cut_comment_controller($entity_type, $entity_id, $mode = NULL) {
  $clipboard = new CommentMoverClipboard();
  $entities = entity_load($entity_type, array($entity_id));
  $entity = array_pop($entities);
  // If the comment is already in the clipboard, don't process this function.
  if (!$clipboard->isEntityAlreadyInClipboard($entity_type, $entity_id)) {
    $clipboard->addEntityToClipboard($entity_type, $entity);
    drupal_set_message(t('The @entity_type was cutted to the clipboard', array('@entity_type' => $entity_type)));
  }
  else {
    drupal_set_message(t('Sorry, this @entity_type is already in the clipboard', array('@entity_type' => $entity_type)), 'error');
  }
  // If javascript is disabled, redirecting to a previous page.
  if ($mode != 'ajax') {
    drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
  }
  // Print messages after the comment's links.
  if ($entity_type == 'comment') {
    $messages_inserting_place = variable_get('comment_mover_messages_inserting_place', '.comment-id-%cid');
    $messages_inserting_place = str_replace('%cid', $entity->cid, $messages_inserting_place);
  }
  else {
    $messages_inserting_place = variable_get('comment_mover_messages_inserting_place_node', '#node-%nid .link-wrapper');
    $messages_inserting_place = str_replace('%nid', $entity->nid, $messages_inserting_place);
  }
  $commands[] = ajax_command_after($messages_inserting_place, theme('status_messages'));
  // Refresh 'Comments clipboard' block.
  $commands[] = ajax_command_html('.clipboard_list', $clipboard->renderClipboardList());
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * A callback for the "X" (remove from clipboard) link.
 *
 * Removes information of deleted comment from $_SESSION['comment_mover']
 * variable and refreshing the clipboard's block if ajax is enabled.
 */
function comment_mover_remove_from_clipboard_controller($entity_type, $entity_id, $mode = NULL) {
  $clipboard = new CommentMoverClipboard();
  if (!$clipboard->isEmpty()) {
    $clipboard->removeEntityFromClipboard($entity_type, $entity_id);
  }
  else {
    drupal_set_message(t('The clipboard is empty.'));
  }
  // If javascript is disabled, redirecting to a previous page.
  if (isset($mode) && $mode != 'ajax') {
    drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
  }
  // Refresh 'Comments clipboard' block.
  $commands[] = ajax_command_html('.clipboard_list', $clipboard->renderClipboardList());
  return array('#type' => 'ajax', '#commands' => $commands);
}


/**
 * A callback for the "paste" link.
 *
 * Moves all comments from the clipboard as children to a selected entity.
 *
 * @param string $entity_type
 *   Type of entity which will be parent for moving comment.
 * @param int $parent_entity_id
 *   Id of parent entity (nid for node and cid for comment).
 * @param void $mode
 *   Reserved.
 */
function comment_mover_paste_from_clipboard_controller($entity_type, $parent_entity_id, $mode = NULL) {
  $parent_entities = entity_load($entity_type, array($parent_entity_id));
  $parent_entity = array_pop($parent_entities);
  $clipboard = new CommentMoverClipboard();
  if (!$clipboard->isEmpty()) {
    // Add an ability to prepare a moved comments.
    module_invoke_all('comment_mover_prepare_to_paste', $clipboard);
    // First of all we need to paste cutted nodes with all of their comments.
    if ($nids = $clipboard->getCuttedNids()) {
      comment_mover_move_nodes($nids, $parent_entity);
    }
    // Then we can move all cutted comments.
    if ($cids = $clipboard->getCuttedCids()) {
      comment_mover_move_multiple($cids, $parent_entity, $change_pid = TRUE);
    }
    $clipboard->clearTheClipboard();
    drupal_set_message(t('All entities were pasted from the clipboard.'));
  }
  else {
    drupal_set_message(t('Sorry, the clipboard is empty'), 'error');
  }
  drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
}


/**
 * A callback for ajax to clear the clipboard.
 */
function comment_mover_clear_clipboard_controller() {
  $clipboard = new CommentMoverClipboard();
  $clipboard->clearTheClipboard();
  if ($clipboard->isEmpty()) {
    drupal_set_message(t('The clipboard is empty now.'));
  }
  drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
}
