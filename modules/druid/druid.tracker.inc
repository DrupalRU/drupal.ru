<?php

define('DRU_FORUM_VOC', 1);
define('DRU_TRACKER_LINES', 60);
define('GARBAGE_TERM', 3769); // форум "Мусорка"
define('SMOKINGROOM_TERM', 2066); // форум "Курилка"

/**
 * Menu callback. Prints a listing of active nodes on the site.
 */
function druid_tracker_page($account = NULL, $set_title = FALSE) {
  global $user;

  // Add CSS
  drupal_add_css(drupal_get_path('module', 'druid') .'/druid.tracker.css', 'module', 'all', FALSE);

  /* на какой странице пейджера находимся? */
  $page = $_REQUEST['page'] ? $_REQUEST['page'] + 1 : 1;

  $cache_id = 'druid:tracker:user' . $user->uid . ':account' . $account->uid . ':page'. $page;
  if ($cache = cache_get($cache_id)) {
    return $cache->data;
  }

  if ($account) {
    if ($set_title) {
      // When viewed from user/%user/track, display the name of the user
      // as page title -- the tab title remains Track so this needs to be done
      // here and not in the menu definiton.
      drupal_set_title(check_plain($account->name));
    }
  // TODO: These queries are very expensive, see http://drupal.org/node/105639
    $sql = "SELECT DISTINCT(n.nid), n.vid, n.title, n.type, n.changed, n.promote, n.uid, u.name, GREATEST(n.changed, l.last_comment_timestamp) AS last_updated, l.comment_count FROM {node} n INNER JOIN {node_comment_statistics} l ON n.nid = l.nid INNER JOIN {users} u ON n.uid = u.uid LEFT JOIN {comments} c ON n.nid = c.nid AND (c.status = %d OR c.status IS NULL) WHERE n.status = 1 AND (n.uid = %d OR c.uid = %d) AND n.type <> 'changes' ORDER BY last_updated DESC";
    $sql = db_rewrite_sql($sql);
    $sql_count = 'SELECT COUNT(DISTINCT(n.nid)) FROM {node} n LEFT JOIN {comments} c ON n.nid = c.nid AND (c.status = %d OR c.status IS NULL) WHERE n.status = 1 AND (n.uid = %d OR c.uid = %d)';
    $sql_count = db_rewrite_sql($sql_count);
    $result = pager_query($sql, DRU_TRACKER_LINES, 0, $sql_count, COMMENT_PUBLISHED, $account->uid, $account->uid);
  }
  else {
    $sql = "SELECT DISTINCT(n.nid), n.vid, n.title, n.type, n.changed, n.promote, n.uid, u.name, GREATEST(n.changed, l.last_comment_timestamp) AS last_updated, l.comment_count FROM {node} n INNER JOIN {users} u ON n.uid = u.uid INNER JOIN {node_comment_statistics} l ON n.nid = l.nid WHERE n.status = 1 AND n.type <> 'changes' ORDER BY last_updated DESC";
    $sql = db_rewrite_sql($sql);
    $sql_count = 'SELECT COUNT(n.nid) FROM {node} n WHERE n.status = 1';
    $sql_count = db_rewrite_sql($sql_count);
    $result = pager_query($sql, DRU_TRACKER_LINES, 0, $sql_count);
  }

  $rows = array();
  while ($node = db_fetch_object($result)) {
    // Determine the number of comments:
    $comments = 0;
    if ($node->comment_count) {
      $comments = $node->comment_count;

      if ($new = comment_num_new($node->nid)) {
        $comments .= '<br />';
        $comments .= l(format_plural($new, '1 new', '@count new'), "node/$node->nid", array('query' => comment_new_page_count($node->comment_count, $new, $node), 'fragment' => 'new'));
      }
    }

    $username = theme('username', $node);

    // Определяем форум к котором относится нода
    $forum = db_fetch_object(db_query('SELECT t.tid, d.name FROM {term_node} t INNER JOIN {term_data} d ON t.tid = d.tid WHERE t.vid = %d AND t.nid = %d AND d.vid = %d', $node->vid, $node->nid, DRU_FORUM_VOC));
    $where = $forum->tid ? l($forum->name, drupal_get_path_alias('forum/' . $forum->tid)) : l('Блог ' . $node->name, 'blog/' . $node->uid);

		if ($forum->tid == GARBAGE_TERM || $forum->tid == SMOKINGROOM_TERM) continue; // skip форумы "Мусорка" и "Курилка"

    $rows[] = array(
      $where,
      ($node->promote ? '<img src="/files/img/star.png" alt="на главной" title="Материал с главной страницы." align="left" /> ' : ' ') . l($node->title, "node/$node->nid") .' '. theme('mark', node_mark($node->nid, $node->changed)),
      $username,
      array('class' => 'replies', 'data' => $comments),
      t('!time ago', array('!time' => format_interval(time() - $node->last_updated)))
    );
  }

  if (!$rows) {
    $rows[] = array(array('data' => t('No posts available.'), 'colspan' => '5'));
  }

  $header = array('Форум/блог', t('Post'), t('Author'), t('Replies'), t('Last updated'));

  $output = '<div id="tracker">';
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, DRU_TRACKER_LINES, 0);
  $output .= '</div>';

  cache_set($cache_id, $output, 'cache', CACHE_TEMPORARY);
  return $output;
}


/**
 * Только ноды созданные юзером.
 *
 */
function druid_my_tracker_page() {
  global $user;

  return druid_tracker_page($user);
}
