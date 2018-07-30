<?php

function druru_alttracker($variables) {
  $comment_icon = druru_icon('comment-o');
  $user_icon = druru_icon('user', NULL, array(
    'class' => array('text-muted'),
  ));
  $items = array();

  foreach ($variables['nodes'] as $node) {
    $item = array();

    $user_name = $node->name ?: variable_get('anonymous');
    $comments = $comment_icon . $node->comment_count;
    $last_page_query = comment_new_page_count($node->comment_count, $node->new_replies, $node);
    $time_ago = format_interval(REQUEST_TIME - $node->changed, 1);
    $node_icon = isset($node->resolved_status) ? $node->resolved_status : NULL;

    // New comments.
    if ($node->new) {
      $comments .= ' / ';
      $comments .= format_plural($node->new_replies, '1 new', '@count new');
    }
    $item[] = array(
      '#type'       => 'html_tag',
      '#tag'        => 'span',
      '#value'      => $comments,
      '#attributes' => array(
        'class' => array(
          'node-item--comments-stat',
          'label',
          $node->new ? 'label-success' : 'label-default',
        ),
      ),
    );

    // Title.
    $item[] = array(
      '#type'       => 'html_tag',
      '#tag'        => 'span',
      '#value'      => $node_icon . check_plain($node->title),
      '#attributes' => array(
        'class' => array(
          'node-item--title',
        ),
        'title' => check_plain($node->title),
      ),
    );

    // User.
    $item[] = array(
      '#type'       => 'html_tag',
      '#tag'        => 'small',
      '#value'      => $user_name,
      '#attributes' => array(
        'class' => array('node-item--author', 'user-picture'),
      ),
    );

    $items[] = array(
      '#theme'   => 'link',
      '#text'    => render($item),
      '#path'    => "node/{$node->nid}",
      '#options' => array(
        'attributes' => array(
          'class' => array('node-item')
        ),
        'html'       => TRUE,
        'query'      => $last_page_query,
        'fragment'   => 'new',
      ),
    );
  }

  return theme('bootstrap_list_group', array(
    'type'       => 'links',
    'items'      => $items,
    'attributes' => array(
      'class' => array('alttracker'),
    ),
  ));
}
