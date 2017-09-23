<?php
/**
 * @file
 * Contains a Clipboard object to incapsulate the logic with 'Clipboard' block.
 */

/**
 * Class to incapsulate storing data about the cutted entities.
 */
class CommentMoverClipboard {

  /**
   * Checks whether the entity was already copied to the clipboard or not.
   */
  public function isEntityAlreadyInClipboard($entity_type, $entity_id) {
    if (!$this->isEmpty()) {
      if ($entity_type == 'comment') {
        $id_key = 'cid';
      }
      else {
        $id_key = 'nid';
      }
      foreach ($_SESSION['comment_mover']['clipboard'] as $key => $comment_in_clipboard) {
        if (isset($comment_in_clipboard[$id_key]) && $comment_in_clipboard[$id_key] == $entity_id) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Performs a check for existent data about any cutted entity.
   */
  public function isEmpty() {
    return !(isset($_SESSION['comment_mover'])
      && isset($_SESSION['comment_mover']['clipboard'])
      && count($_SESSION['comment_mover']['clipboard']) > 0);
  }

  /**
   * Clears tha data about cutted comments and nodes from $_SESSION.
   */
  public function clearTheClipboard() {
    unset($_SESSION['comment_mover']);
  }

  /**
   * Retrieves an array with IDs of cutted comments or FALSE.
   */
  public function getCuttedCids() {
    if (isset($_SESSION['comment_mover'])
      && isset($_SESSION['comment_mover']['clipboard_cids'])
      && count($_SESSION['comment_mover']['clipboard_cids']) > 0) {
      return $_SESSION['comment_mover']['clipboard_cids'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Retrieves an array with IDs of cutted nodes or FALSE.
   */
  public function getCuttedNids() {
    if (isset($_SESSION['comment_mover'])
      && isset($_SESSION['comment_mover']['clipboard_nids'])
      && count($_SESSION['comment_mover']['clipboard_nids']) > 0) {
      return $_SESSION['comment_mover']['clipboard_nids'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Stores the information about the cutted entity to $_SESSION.
   */
  public function addEntityToClipboard($entity_type, $entity) {
    if (!isset($_SESSION['comment_mover']['clipboard'])) {
      $_SESSION['comment_mover']['clipboard'] = array();
    }
    switch ($entity_type) {
      case 'comment':
        $comment = $entity;
        // Formatting comment's title.
        $comment_title = !empty($comment->subject) ? $comment->subject : $comment->comment_body[$comment->language][0]['value'];
        $comment_title = truncate_utf8(strip_tags($comment_title), 15, TRUE, TRUE);
        // Cutting the comment into clipboard.
        $_SESSION['comment_mover']['clipboard'][] = array(
          'cid' => $comment->cid,
          'title' => $comment_title,
        );
        if (!isset($_SESSION['comment_mover']['clipboard_cids'])) {
          $_SESSION['comment_mover']['clipboard_cids'] = array();
        }
        $_SESSION['comment_mover']['clipboard_cids'][] = $comment->cid;
        break;

      case 'node':
        $node = $entity;
        $node_title = truncate_utf8(strip_tags($node->title), 15, TRUE, TRUE);
        // Cutting the comment into clipboard.
        $_SESSION['comment_mover']['clipboard'][] = array(
          'nid' => $node->nid,
          'title' => $node_title,
        );
        if (!isset($_SESSION['comment_mover']['clipboard_nids'])) {
          $_SESSION['comment_mover']['clipboard_nids'] = array();
        }
        $_SESSION['comment_mover']['clipboard_nids'][] = $node->nid;
        break;

      default:
        break;
    }
  }

  /**
   * Removes information about deleted comment from $_SESSION['comment_mover'].
   *
   * @param string $entity_type
   *   The type of entity to remove from the clipboard.
   *   Could be 'comment' or 'node'.
   * @param int $entity_id
   *   The id of the entity to remove from the clipboard.
   *
   * @todo Maybe we should use https://drupal.org/project/session_api ?
   */
  public function removeEntityFromClipboard($entity_type, $entity_id) {
    if ($entity_type == 'comment') {
      $entity_id_key = 'cid';
    }
    else {
      $entity_id_key = 'nid';
    }
    // First of all, remove the id of the entity from the appropriate id list
    // in the clipboard. For example, 'clipboard_cids' or 'clipboard_nids'.
    $id_list = 'clipboard_' . $entity_id_key . 's';
    $_SESSION['comment_mover'][$id_list] = array_diff($_SESSION['comment_mover'][$id_list], array($entity_id));
    // The next thing to do is to remove it from the text list of
    // the clipboard block.
    foreach ($_SESSION['comment_mover']['clipboard'] as $key => $comment_in_clipboard) {
      if ($comment_in_clipboard[$entity_id_key] == $entity_id) {
        unset($_SESSION['comment_mover']['clipboard'][$key]);
      }
    }
    if (count($_SESSION['comment_mover']['clipboard']) == 0) {
      unset($_SESSION['comment_mover']);
    }
  }

  /**
   * Returns a rendered html with list of comments in the clipboard.
   */
  public function renderClipboardList() {
    if (isset($_SESSION['comment_mover'])) {
      return theme('comment_mover_clipboard_list', array('clipboard_comments' => $_SESSION['comment_mover']['clipboard']));
    }
    else {
      return t('There are no comments yet');
    }
  }

  /**
   * Returns a rendered html with the content of the clipboard block.
   */
  public function renderClipboardBlock() {
    return theme('comment_mover_clipboard_block', array(
        'clipboard_list' => $this->renderClipboardList(),
        'convert_form' => drupal_get_form('comment_mover_convert_form'),
        'clear_link' => l(t('Clear clipboard'), 'comment_mover/clear_clipboard'),
      ));
  }

  /**
   * Checks for conflicts in the cutted comments.
   *
   * The clipboard should contains only the first upper parent of each
   * comment's thread.
   *
   * E.g. if a user cuts comments # 1, 3.1, 10, 3, 5.2 then the clipboard
   * should contains only comments # 1, 3, 5.2 and 10
   * (without the child comment 3.1).
   */
  public function getCidsCleanedFromConflicts($cids) {
    $db_comments = db_query("SELECT cid, thread FROM {comment} WHERE cid IN (" . implode(',', $cids) . ")")->fetchAllKeyed();
    $result_cids = array_keys($db_comments);
    foreach ($db_comments as $parent_cid => $parent_comment_thread) {
      // The 'current' comment will be the same for all comparisons with
      // all other cids, then grab the next 'current' comment and compare
      // it with all others and so on.
      $parent_comment_thread = str_replace('/', '', $parent_comment_thread);
      foreach ($db_comments as $checking_cid => $checking_comment_thread) {
        $checking_thread = str_replace('/', '', $checking_comment_thread);
        // If the current comment is on the upper level then checking comment.
        if (strlen($parent_comment_thread) < strlen($checking_thread)) {
          // If the current comment is the parent to checking comment.
          if (substr($checking_thread, 0, strlen($parent_comment_thread)) == $parent_comment_thread) {
            // That means we found that the 'checking' comment is a child
            // for current 'parent' comment.
            // We need to remove it from the clipboard because it will be
            // moved with it's parent anyway.
            unset($result_cids[$checking_cid]);
          }
        }
      }
    }
    return $result_cids;
  }

  /**
   * Edge situation - a moving comment is a parent to the destination.
   *
   * We can't move the comment to its child, it will cause am infinite
   * recursion (parent to child, child to parent and so on).
   *
   * E.g. if user cuts a comments with thread 01/ and he trying to paste it
   * to the child comment with thread 01.00/ we should show an error.
   */
  public function checkEdgeParentIsChild($cids, $parent_entity) {
    // If the parent entity is comment.
    if (!isset($parent_entity->type)) {
      $parent_thread = str_replace('/', '', $parent_entity->thread);
      $db_comments = db_query("SELECT cid, thread FROM {comment} WHERE nid = :nid AND cid IN (:cids)",
        array(
          ':nid' => $parent_entity->nid,
          ':cids' => implode(',', $cids),
        )
      )->fetchAllAssoc('cid');
      if (count($db_comments) > 0) {
        foreach ($db_comments as $cid => $db_comment) {
          $thread = str_replace('/', '', $db_comment->thread);
          // If the checking comment is on the upper level then the parent
          // comment.
          if (strlen($thread) < strlen($parent_thread)) {
            if (substr($parent_thread, 0, strlen($thread)) == $thread) {
              // When the beginnings of the threads are the same it means that
              // the comment where we want to paste is a child to some pasting
              // comment, and that is wrong (it could cause an infinite
              // recursion).
              return TRUE;
            }
          }
        }
      }
    }
    return FALSE;
  }
}
