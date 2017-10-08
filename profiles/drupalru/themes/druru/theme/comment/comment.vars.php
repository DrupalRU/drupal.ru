<?php

function druru_preprocess_comment(&$vars) {
  $links      = array();
  $link_attrs = array();

  if (isset($vars['content']['links'])) {
    $links = &$vars['content']['links'];
  }
  if (isset($links['#attributes'])) {
    $link_attrs = &$links['#attributes'];
  }

  $vars['attributes_array']['data-comment-id'] = $vars['comment']->cid;

  // Generate bautiful permanent link to comment which
  // includes path to node instead of path to comment.
  $node_url          = entity_uri('node', $vars['node']);
  $comment_fragment  = 'comment-' . $vars['comment']->cid;
  $vars['permalink'] = l(druru_icon('anchor'), $node_url['path'], array(
    'html'       => TRUE,
    'fragment'   => $comment_fragment,
    'attributes' => array(
      'class' => array('permanent-link', 'text-muted'),
      'title' => t('Anchor for this comment'),
    ),
  ));
  $vars['timeago']   = t('@time ago', array(
    '@time' => format_interval(time() - $vars['comment']->changed, 1),
  ));

  // Check published status
  if ($vars['status'] == 'comment-unpublished') {
    $vars['unpublished'] = ' <span class="unpublished-item">';
    $vars['unpublished'] .= druru_get_icon_by_title(t('Unpublished')) . t('Unpublished');
    $vars['unpublished'] .= '</span>';
  }

  // Delete class 'inline" and
  // "list-inline" (will added automatically) from links.
  if (!empty($link_attrs['class'])) {
    $key = array_search('inline', $link_attrs['class']);
    if ($key !== FALSE) {
      unset($link_attrs['class'][$key]);
    }
  }

  // Icons will shown by the order.
  $icons1 = array(
    'dru-tnx'       => 'heart',
    'dru-untnx'     => 'heart-o',
    'quote'         => 'quote-right',
    'comment-reply' => 'reply',
    'dru-claim'     => 'balance-scale',
  );
  $icons2 = array(
    'darkmatter-link'  => 'moon-o',
    'comment-resolve'  => 'flag',
    'comment-unsolved' => 'flag-o',
    'unpublish'        => 'eye-slash',
    'comment-edit'     => 'pencil',
    'comment-delete'   => 'trash',
  );

  if (array_intersect_key($icons2, $links['comment']['#links'])) {
    $links['comment']['#links']['divider'] = array(
      'title' => '&nbsp;',
      'html'  => TRUE,
    );
    $icons1['divider']                     = 'divider';
    $sort                                  = array_merge($icons1, $icons2);
  }
  else {
    $sort = $icons1;
  }

  if (isset($links['comment']['#links'])) {
    foreach ($links['comment']['#links'] as $key => &$link) {
      $classes = $icon_classes = array();
      if (isset($link['attributes']['class'])) {
        $classes = $link['attributes']['class'];
      }

      if ('dru-tnx' == $key) {
        $icon_classes[] = 'text-danger';
      }

      if (array_search('dru-untnx', $classes) !== FALSE) {
        $key = 'dru-untnx';
      }
      // This is like ucfirst for cyrillic symbols.
      $first_letter = mb_strtoupper(mb_substr($link['title'], 0, 1));
      $other_name   = mb_substr($link['title'], 1);

      $icon = '';
      if (isset($sort[$key])) {
        $icon = druru_icon($sort[$key], FALSE, array('class' => $icon_classes));
      }

      $link['title'] = $icon . $first_letter . $other_name;
      $link['html']  = TRUE;

      $weight = array_search($key, array_keys($sort));
      if ($weight !== FALSE) {
        $link['weight'] = $weight;
      }
      $link['attributes']['class'] = $classes;
    }

    uasort($links['comment']['#links'], 'drupal_sort_weight');
  }

  if (isset($vars['comment']->tnx)) {

    $vars['tnx'] = dru_tnx_view($vars['comment'], 'comment');

    /*//$vars['tnx'] = array(
    //  '#type'       => 'container',
    //  '#attributes' => array(
    //    'class' => array(
    //      'tnx-counter',
    //      'counter-' . (isset($vars['comment']->tnx) ? $vars['comment']->tnx : 0),
    //      'dru-tnx-comment-' . $vars['comment']->cid . '-counter',
    //    ),
    //  ),
    //  'tnx'         => array(
    //    '#markup' => druru_icon('heart', FALSE, array(
    //        'class' => array(
    //          'text-danger',
    //        ),
    //      )) . $vars['comment']->tnx,
    //  ),
    //);*/
  }

  if (!_druru_links_access($vars['content'])) {
    unset($vars['content']['links']);
  }
}

function _druru_links_access($content) {
  return (!empty($content['links']) && empty($content['links']['#printed']) && (
        !isset($content['links']['#access']) || $content['links']['#access'])
    ) && $GLOBALS['user']->uid;
}
