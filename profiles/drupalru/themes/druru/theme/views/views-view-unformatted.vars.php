<?php
/**
 * @file
 */

/**
 * Display the simple view of rows one after another.
 */
function druru_preprocess_views_view_unformatted(&$vars) {
  $view = $vars['view'];
  $rows = $vars['rows'];
  $vars['classes_array'] = [];
  $vars['classes'] = [];
  $result = $vars['view']->result;

  if (in_array($view->name, ['events_upcoming', 'events_upcoming_block', 'events_past'])) {
    foreach ($rows as $key => $row) {
      $vars['classes_array'][$key] = 'event';
    }
  }

  if (in_array($view->name, ['tracker', 'tracker_new', 'tracker_my', 'tracker_featured'])) {
    // We need to mark 'resolved' nodes.
    // @todo Reimplement 'resolve' with module 'flag' and refactor assigning css class.
    // 1. Build an array of nids of nodes from view results.
    $nids = array_map(function($node) {
      return $node->nid;
    }, $result);
    // 2. Get nids of resolved nodes from list of displayed nodes.
    $query = "SELECT nid FROM {resolved} WHERE nid IN (:nids)";
    $resolved_nodes = db_query($query, array(':nids' => $nids))->fetchCol();

    foreach ($rows as $key => $row) {
      $vars['classes'][$key][] = 'node';
      // 3. Add css class if node is 'resolved'.
      if (in_array($result[$key]->nid, $resolved_nodes)) {
        $vars['classes'][$key][] = 'has-accepted-answer';
      }

      if (isset($result[$key]->node_new_comments)) {
        if ($result[$key]->node_new_comments > 0) {
          $vars['classes'][$key][] = 'has-new-comments';
        }
      }

      $node_mark = node_mark($result[$key]->nid, $result[$key]->node_changed);

      if ($node_mark == MARK_NEW) {
        $vars['classes'][$key][] = 'is-new';
      }

      if ($node_mark == MARK_UPDATED) {
        $vars['classes'][$key][] = 'is-updated';
      }

      if ($result[$key]->node_comment_statistics_comment_count == 0) {
        $vars['classes'][$key][] = 'has-no-comments';
      }

      $vars['classes_array'][$key] = isset($vars['classes'][$key]) ? implode(' ', $vars['classes'][$key]) : '';
    }
  }
}
