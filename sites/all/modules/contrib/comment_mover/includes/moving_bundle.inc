<?php
/**
 * @file
 * Contains the MovingBundle object for the code for moving a bunch of entities.
 */

/**
 * Incapsulates a data and functions to move comments and nodes around.
 */
class CommentMoverMovingBundle {

  private $cids;
  private $nids;

  /**
   * The MovingBundle has properties: array of cids and nids to move.
   */
  public function __construct($cids, $nids = array()) {
    $this->cids = $cids;
    $this->nids = $nids;
  }

  /**
   * Moves comments to a new parent node or comment.
   *
   * Children of the comments are moving along too.
   *
   * @param object $parent_entity
   *   Entity will be parent for moving comment itself (not for his children).
   * @param int $change_pid
   *   When we move first level of comments (comments themselves), we should
   *   change their pid for an id of parent entity, and then $change_pid = TRUE.
   *   But when we move children of moved comment, we shouldn't change their pid
   *   and then $change_pid = FALSE.
   */
  public function move($parent_entity, $change_pid = FALSE) {
    $updated_nodes = array();
    $comments_to_delete = array();
    // Move comments and their childrens to a new destinations.
    $this->moveMultipleRecursion($this->cids, $parent_entity, $updated_nodes, $comments_to_delete, $change_pid);
    if (count($comments_to_delete) > 0) {
      comment_delete_multiple($comments_to_delete);
    }
    // Updating node's comments count.
    // It updates each node statistics only for moved comments and only 1 time.
    foreach ($updated_nodes as $updated_nid) {
      _comment_update_node_statistics($updated_nid);
    }
  }

  /**
   * Helper recursion function - moves a comment to a new parent entity.
   *
   * Children of the comment are moving along too.
   *
   * @param int $cids
   *   Array if comments ids for moving.
   * @param object $parent_entity
   *   Entity will be parent for moving comment itself (not for his children).
   * @param array $updated_nodes
   *   We need to store all nids of updated comments to refresh its node's
   *   statistics - only one time per node to improve performance.
   * @param int $change_pid
   *   When we move first level of comments (comments themselves), we should
   *   change their pid for an id of parent entity, and then $change_pid = TRUE.
   *   But when we move children of moved comment, we shouldn't change their pid
   *   and then $change_pid = FALSE.
   */
  private function moveMultipleRecursion($cids, $parent_entity, &$updated_nodes, &$comments_to_delete, $change_pid = FALSE) {
    $comments = comment_load_multiple($cids);
    $destination_node_type = node_load($parent_entity->nid)->type;
    // When the comments style is 'flat comments' we should change cid.
    $change_cid = variable_get('comment_mover_flatcomments_change_cid', TRUE)
      && (variable_get('comment_default_mode_' . $destination_node_type, 1) == 0);
    if ($comments) {
      $updated_nodes = array();
      foreach ($comments as $comment) {
        if ($change_pid) {
          // When moving comment itself, we should change it's parents id (pid).
          if (isset($parent_entity->type)) {
            // If parent entity is node:
            $comment->pid = 0;
          }
          else {
            $comment->pid = isset($parent_entity->cid) ? $parent_entity->cid : 0;
          }
        }
        if (!in_array($comment->nid, $updated_nodes)) {
          $updated_nodes[] = $comment->nid;
        }
        if (!in_array($parent_entity->nid, $updated_nodes)) {
          $updated_nodes[] = $parent_entity->nid;
        }
        $comment->nid = $parent_entity->nid;
        $comment->thread = $this->updateCommentThread($comment);
        $child_cids = $this->getChildCidsOfComment($comment);
        // An ability to paste the comments always at the bottom.
        if ($change_cid) {
          $comments_to_delete[] = $comment->cid;
          $comment->cid = NULL;
          $change_pid = TRUE;
        }
        else {
          $change_pid = FALSE;
        }
        // Add an ability to alter a moved comment.
        module_invoke_all('comment_mover_moved_comment_alter', $comment);
        comment_save($comment);
        if ($change_cid) {
          // If we are changing cid, all child comments we should move under
          // that new cid.
          $parent_entity = $comment;
        }
        // Move all child comments, their pid will stay unchanged.
        if (count($child_cids) > 0) {
          $this->moveMultipleRecursion($child_cids, $parent_entity, $updated_nodes, $comments_to_delete, $change_pid);
        }
      }
    }
  }

  /**
   * Updates a "thread" property of the moved comment.
   *
   * TODO: Move the db queries outside the recursion! (important).
   */
  private function updateCommentThread($comment) {
    if ($comment->pid == 0) {
      // This is a comment with no parent comment (depth 0): we start
      // by retrieving the maximum thread level.
      $max = db_query('SELECT MAX(thread) FROM {comment} WHERE nid = :nid', array(':nid' => $comment->nid))->fetchField();
      // Strip the "/" from the end of the thread.
      $max = rtrim($max, '/');
      // We need to get the value at the correct depth.
      $parts = explode('.', $max);
      $firstsegment = $parts[0];
      // Finally, build the thread field for this new comment.
      $thread = int2vancode(vancode2int($firstsegment) + 1) . '/';
    }
    else {
      // This is a comment with a parent comment, so increase the part of the
      // thread value at the proper depth.
      // Get the parent comment:
      $parent = comment_load($comment->pid);
      // Strip the "/" from the end of the parent thread.
      $parent->thread = (string) rtrim((string) $parent->thread, '/');
      // Get the max value in *this* thread.
      $max = db_query("SELECT MAX(thread) FROM {comment} WHERE thread LIKE :thread AND nid = :nid", array(
        ':thread' => $parent->thread . '.%',
        ':nid' => $comment->nid,
      ))->fetchField();

      if ($max == '') {
        // First child of this parent.
        $thread = $parent->thread . '.' . int2vancode(0) . '/';
      }
      else {
        // Strip the "/" at the end of the thread.
        $max = rtrim($max, '/');
        // Get the value at the correct depth.
        $parts = explode('.', $max);
        $parent_depth = count(explode('.', $parent->thread));
        $last = $parts[$parent_depth];
        // Finally, build the thread field for this new comment.
        $thread = $parent->thread . '.' . int2vancode(vancode2int($last) + 1) . '/';
      }
    }
    return $thread;
  }

  /**
   * Returns an array with cid's of comment's child comments.
   */
  private function getChildCidsOfComment($comment) {
    return db_query('SELECT cid FROM {comment} WHERE pid = :cid', array(':cid' => $comment->cid))->fetchCol();
  }

  /**
   * Returns an array with cid's of child comments (only the first level).
   */
  private function getChildCidsOfNode($node) {
    return db_query('SELECT cid FROM {comment} WHERE nid = :nid AND pid = 0', array(':nid' => $node->nid))->fetchCol();
  }

  /**
   * Converts current cids to a given node_type.
   */
  public function convertCommentsToNodes($node_type) {
    $comments = comment_load_multiple($this->cids);
    foreach ($comments as $comment) {
      $this->convertACommentToANode($comment, $node_type);
    }
    return TRUE;
  }

  /**
   * Creates a node from a comment.
   *
   * @param object $comment
   *   The comment to start from.
   * @param string $node_type
   *   Type of node to create from that comment.
   *
   * @return object
   *   Newly created node.
   */
  private function convertACommentToANode($comment, $node_type, $extra = array()) {
    $txn = db_transaction();
    $node = new stdClass();
    $node->nid = NULL;
    $node->vid = NULL;
    $node->uid = $comment->uid;
    $node->type = $node_type;
    $node->title = $comment->subject;
    $node->created = $comment->created;
    $node->changed = REQUEST_TIME;
    $node->status = $comment->status;
    // Allow adding comments to that node.
    $node->comment = 2;
    $field_names = array_merge(array_keys(field_info_instances('comment', $comment->node_type)), $extra);
    foreach ($field_names as $field_name) {
      if (isset($comment->$field_name) && field_info_instance('node', $field_name, $node_type)) {
        $node->$field_name = $comment->$field_name;
      }
    }
    if (isset($comment->comment_body)) {
      $node->body = $comment->comment_body;
    }
    node_save($node);
    // Move child comments to a new node.
    $child_cids = $this->getChildCidsOfComment($comment);
    if (count($child_cids) > 0) {
      comment_mover_move_multiple($child_cids, $node, $change_pid = TRUE);
    }
    comment_delete($comment->cid);
    return $node;
  }

  /**
   * Converts a bunch of nodes to comments under the given entity.
   */
  public function convertNodesToCommentsAndMove($parent_entity) {
    $nodes = entity_load('node', $this->nids);
    foreach ($nodes as $key => $node) {
      $this->convertANodeToAComment($node, $parent_entity);
    }
    if (isset($parent_entity->type)) {
      // If parent is a node we should update its comment's statistics.
      _comment_update_node_statistics($parent_entity->nid);
    }
  }

  /**
   * Creates a comment from a node.
   *
   * @param object $node
   *   The comment to start from.
   * @param string $parent_entity
   *   An entity under which the converted comment will be placed.
   *
   * @return object
   *   Newly created comment.
   */
  private function convertANodeToAComment($node, $parent_entity) {
    $txn = db_transaction();
    $comment = new stdClass();
    $comment->nid = $parent_entity->nid;
    $comment->cid = NULL;
    if (isset($parent_entity->type)) {
      $comment->pid = 0;
    }
    else {
      $comment->pid = $parent_entity->cid;
    }
    $comment->uid = $node->uid;
    $comment->subject = $node->title;
    $comment->created = $node->created;
    $comment->changed = REQUEST_TIME;
    $comment->status = $node->status;
    $field_names = array_merge(array_keys(field_info_instances('node', $node->type)));
    foreach ($field_names as $field_name) {
      if (isset($node->$field_name) && field_info_instance('comment', $field_name, 'comment_node_' . $node->type)) {
        $comment->$field_name = $node->$field_name;
      }
    }
    if (isset($node->body)) {
      $comment->comment_body = $node->body;
    }
    $comment->thread = $this->updateCommentThread($comment);
    // Add an ability to alter a moved comment.
    module_invoke_all('comment_mover_moved_node_alter', $comment);
    comment_save($comment);
    // Move child comments to a new node.
    $child_cids = $this->getChildCidsOfNode($node);
    if (count($child_cids) > 0) {
      comment_mover_move_multiple($child_cids, $comment, $change_pid = TRUE);
    }
    node_delete($node->nid);
    return $comment;
  }

}
