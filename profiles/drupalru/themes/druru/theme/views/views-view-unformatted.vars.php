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
  $result = $vars['view']->result;
  $vars['classes_array'] = [];
  $vars['classes'] = [];
  $vars['node_hrefs'] = [];

  if ($view->name == 'events_upcoming_block') {
    foreach ($rows as $id => $row) {
      $vars['classes_array'][$id] = 'event';
    }
  }

  if (in_array($view->name, ['tracker', 'tracker_new', 'tracker_my', 'featured'])) {
    foreach ($rows as $id => $row) {
      $vars['classes'][$id][] = 'node-item';
      $vars['node_hrefs'][$id] = '/node/' . $result[$id]->nid;

      $node_mark = node_mark($result[$id]->nid, $result[$id]->node_changed);

      if ($node_mark != MARK_NEW && $result[$id]->node_new_comments > 0) {
        $vars['node_hrefs'][$id] .= '#new';
      }

      if ($node_mark == MARK_NEW) {
        $vars['classes'][$id][] = 'is-new';
      }

      if ($node_mark == MARK_UPDATED) {
        $vars['classes'][$id][] = 'is-updated';
      }

      if ($result[$id]->node_new_comments > 0) {
        $vars['classes'][$id][] = 'has-new-comments';
      }

      if ($result[$id]->node_comment_statistics_comment_count == 0) {
        $vars['classes'][$id][] = 'zero-comments';
      }

      $vars['classes_array'][$id] = isset($vars['classes'][$id]) ? implode(' ', $vars['classes'][$id]) : '';
    }
  }
}
