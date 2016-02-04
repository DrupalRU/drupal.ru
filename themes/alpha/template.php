<?php

/**
 * @file
 * template.php
 */

/**
 * Implements hook_theme().
 */
function alpha_theme($existing, $type, $theme, $path) {
  return array(
    'menu_user_blog_links' => array(
      'variables' => array('primary' => array(), 'secondary' => array()),
    ),
    'alttracker_node' => array(
      'variables' => array('node' => NULL),
      'template' => 'templates/alttracker_node',
    ),
    'marketplace_random_block' => array(
      'render element' => 'content',
      'template' => 'templates/marketplace--blocklist',
    ),
    'node__simple_event__teaser' => array(
      'render element' => 'content',
      'base hook' => 'node',
      'template' => 'templates/node--simple_event--teaser',
    ),
    'node__simple_event__block' => array(
      'render element' => 'content',
      'base hook' => 'node',
      'template' => 'templates/node--simple_event--block',
    ),
    'node__frontpage' => array(
      'render element' => 'content',
      'base hook' => 'node',
      'template' => 'templates/node--frontpage',
    ),
    'frontpage__list' => array(
      'render element' => 'content',
      'template' => 'templates/frontpage--list',
    ),

  );
}

/**
 * Implements hook_js_alter().
 */
function alpha_js_alter(&$javascript) {
  drupal_add_js('var themeTableHeaderOffset = function() { var offsetheight = jQuery("#navbar").height(); return offsetheight; }', 'inline');
  drupal_add_js(array('tableHeaderOffset' => 'themeTableHeaderOffset'), 'setting');
}

/**
 * Implements hook_preprocess_html().
 */
function alpha_preprocess_html(&$vars) {
  drupal_add_js(drupal_get_path('theme', 'alpha') . '/js/jquery.mobile.custom.min.js');
  $html_tag = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1, maximum-scale=1.0, user-scalable=no',
    ),
  );
  drupal_add_html_head($html_tag, 'viewport');
}


/**
 * Preprocess variables for page.tpl.php.
 *
 * Most themes utilize their own copy of page.tpl.php. The default is located
 * inside "modules/system/page.tpl.php". Look in there for the full list of
 * variables.
 *
 * Uses the arg() function to generate a series of page template suggestions
 * based on the current path.
 *
 * Any changes to variables in this preprocessor should also be changed inside
 * template_preprocess_maintenance_page() to keep all of them consistent.
 *
 * @see drupal_render_page()
 * @see template_process_page()
 * @see page.tpl.php
 */
function alpha_preprocess_page(&$variables) {
  if (isset($variables['node'])) {
    $node = $variables['node'];

    $flag = theme('forum_icon', array('new_posts' => FALSE, 'num_posts' => $node->comment_count, 'comment_mode' => $node->comment, 'sticky' => $node->sticky, 'first_new' => FALSE));

    if ($node->promote) {
      $flag = '<i class="fa fa-star"></i>';
    }
    $variables['title'] = '<div class="flag">' . $flag . '</div>' . drupal_get_title();
  }
  if ($variables['theme_hook_suggestions'][0] == 'page__user') {
    if (isset($variables['page']['content']['system_main']['#account'])) {
      $account = $variables['page']['content']['system_main']['#account'];
    }
    if (isset($variables['page']['content']['system_main']['#user'])) {
      $account = $variables['page']['content']['system_main']['#user'];
    }

    if (isset($variables['page']['content']['system_main']['recipient']['#value'])) {
      $account = $variables['page']['content']['system_main']['recipient']['#value'];
    }

    if (empty($account)) {
      $path_elements = explode("/", $_SERVER['REDIRECT_URL']);
      $uid = $path_elements[2];
      $account = user_load($uid);
    }

    $picture = $account->picture;
    if (!empty($picture)) {
      if (!empty($picture->uri)) {
        $filepath = $picture->uri;
      }
    }
    elseif (variable_get('user_picture_default', '')) {
      $filepath = variable_get('user_picture_default', '');
    }
    if (isset($filepath)) {
      if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style_node', '')) {
        $variables['user_picture'] = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $account->name, 'title' => $account->name, 'attributes' => array('class' => array('img-circle'))));
      }
      else {
        $variables['user_picture'] = theme('image', array('path' => $filepath, 'alt' => $account->name, 'title' => $account->name, 'attributes' => array('class' => array('img-circle'))));
      }
    }
    $tabs = $variables['tabs'];
    $secondary = $tabs['#secondary'];
    unset($tabs['#secondary']);

    $tabs['#theme'] = 'menu_user_blog_links';
    $variables['primary_nav'] = $tabs;

    $variables['tabs'] = array(
      '#theme' => 'menu_local_tasks',
      '#primary' => $secondary,
    );
  }
}

/**
 * Process variables for user-picture.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $account: A user, node or comment object with 'name', 'uid' and 'picture'
 *   fields.
 *
 * @see user-picture.tpl.php
 */
function alpha_preprocess_user_picture(&$variables) {
  $variables['user_picture'] = '';
  if (variable_get('user_pictures', 0)) {
    $account = $variables['account'];
    if (!empty($account->picture)) {
      // @TODO: Ideally this function would only be passed file objects, but
      // since there's a lot of legacy code that JOINs the {users} table to
      // {node} or {comments} and passes the results into this function if we
      // a numeric value in the picture field we'll assume it's a file id
      // and load it for them. Once we've got user_load_multiple() and
      // comment_load_multiple() functions the user module will be able to load
      // the picture files in mass during the object's load process.
      if (is_numeric($account->picture)) {
        $account->picture = file_load($account->picture);
      }
      if (!empty($account->picture->uri)) {
        $filepath = $account->picture->uri;
      }
    }
    elseif (variable_get('user_picture_default', '')) {
      $filepath = variable_get('user_picture_default', '');
    }
    if (isset($filepath)) {
      $alt = t("@user's picture", array('@user' => format_username($account)));
      // If the image does not have a valid Drupal scheme (for eg. HTTP),
      // don't load image styles.
      if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')) {
        $variables['user_picture'] = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => array('img-circle'))));
      }
      else {
        $variables['user_picture'] = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => array('img-circle'))));
      }
      if (!empty($account->uid) && user_access('access user profiles')) {
        $attributes = array('attributes' => array('title' => t('View user profile.')), 'html' => TRUE);
        $variables['user_picture'] = l($variables['user_picture'], "user/$account->uid", $attributes);
      }
    }
  }
}

/**
 * Implements hook_preprocess_comment().
 */
function alpha_preprocess_comment(&$variables) {
  
  drupal_add_js(drupal_get_path('theme', 'alpha') . '/js/' . 'comment-action-slide.js');
  
  $comment = $variables['elements']['#comment'];
  $variables['timeago'] = t('@time ago', array('@time' => format_interval(time() - $comment->changed)));

  $uri = entity_uri('comment', $comment);
  $variables['permalink'] = l('#', $uri['path'], $uri['options']);

  if (isset($variables['content']['links']['comment']['#links']['comment_forbidden'])) {
    unset($variables['content']['links']['comment']['#links']['comment_forbidden']);
  }
  
  // We need to make sure that we have links.
  // If we don't have links we do not display icon "***".
  if(!empty($variables['content']['links']['comment']['#links'])){
    $variables['content']['links']['comment']['#links']['#cid'] = $comment->cid;
  }
}

function alpha_links__comment(&$variables){
  $cid = $variables['links']['#cid'];
  unset($variables['links']['#cid']);
  
  if(!empty($variables['links'])){
    $variables['attributes']['class'] = array('links comment-links');
    return ''
    . '<div id="comment-links-' . $cid . '" class="comment-actions">'
    . theme_links($variables)
    . '</div>';
  }else{
    return '';
  }

}
/**
 * Implements hook_preprocess_node().
 */
function alpha_preprocess_node(&$variables) {
  $node = $variables['elements']['#node'];
  if ($variables['teaser'] && $variables['type'] != 'organization' && $variables['type'] != 'simple_event') {
    // Add a new item into the theme_hook_suggestions array.
    $variables['theme_hook_suggestions'][] = 'node__teaser';
  }

  if ($variables['view_mode'] == 'alttracker') {
    $variables['theme_hook_suggestions'][] = 'node__alttracker';
  }

  $variables['timeago'] = t('@time ago', array('@time' => format_interval(time() - $node->changed)));

  $picture = $node->picture;
  if (!empty($picture)) {
    if (is_numeric($picture)) {
      $picture = file_load($picture);
    }
    if (!empty($picture->uri)) {
      $filepath = $picture->uri;
    }
  }
  elseif (variable_get('user_picture_default', '')) {
    $filepath = variable_get('user_picture_default', '');
  }
  if (isset($filepath)) {
    $alt = $node->name;
    if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style_node', '')) {
      $variables['user_picture'] = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => array('img-circle'))));
    }
    else {
      $variables['user_picture'] = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt, 'attributes' => array('class' => array('img-circle'))));
    }
    if (!empty($node->uid) && user_access('access user profiles')) {
      $attributes = array('attributes' => array('title' => t('View user profile.')), 'html' => TRUE);
      $variables['user_picture'] = l($variables['user_picture'], "user/$node->uid", $attributes);
    }
  }

  $variables['title_attributes'] = 'title';
  if (!$variables['teaser']) {
    drupal_add_js(drupal_get_path('theme', 'alpha') . '/js/' . 'node-view.js');
  }

  drupal_add_js(drupal_get_path('theme', 'alpha') . '/js/' . 'node-img-responsive.js');

  // Delete Log in links from nodes.
  if (isset($variables['elements']['links']['comment']['#links']['comment_forbidden'])) {
    unset($variables['elements']['links']['comment']['#links']['comment_forbidden']);
  }
  if (isset($variables['content']['links']['comment']['#links']['comment_forbidden'])) {
    unset($variables['content']['links']['comment']['#links']['comment_forbidden']);
  }
  
  if (!empty($node->terms)) {
    $terms_links = array();
    foreach ($node->terms as $term) {
      $terms_links[] = array(
        'title' => check_plain($term->name),
        'href' => url("taxonomy/term/" . $term->tid),
        'html' => TRUE,
      );
    }
    $variables['term'] = theme('links', array('links' => $terms_links));
  }
}


/**
 * Process variables for user-profile.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - elements: An associative array containing the user information and any
 *     fields attached to the user. Properties used:
 *     - #account: The user account of the profile being viewed.
 *
 * @see user-profile.tpl.php
 */
function alpha_preprocess_user_profile(&$variables) {
  $account = $variables['elements']['#account'];
  if ($account->data['contact']) {
    // Add contact form link.
  }

  foreach (element_children($variables['elements']) as $key) {

    if (isset($variables['elements'][$key]['#type']) && $variables['elements'][$key]['#type'] == 'user_profile_category') {
      foreach ($variables['elements'][$key] as $item_key => $item_value) {
        if (is_array($item_value) && isset($item_value['#type']) && $item_value['#type'] == 'user_profile_item') {
          if (empty($item_value['#title'])) {
            $variables['elements'][$key][$item_key]['#title'] = $variables['elements'][$key]['#title'];
            // We add title only for first item in the group.
            break;
          }
        }
      }
    }
    $variables['user_profile'][$key] = $variables['elements'][$key];
  }

  // Preprocess fields.
  field_attach_preprocess('user', $account, $variables['elements'], $variables);

  $variables['name'] = $account->name;
  $variables['realname'] = isset($account->realname) ? $account->realname : '';

  if ($account->signature) {
    $variables['signature'] = check_markup($account->signature, $account->signature_format, '', TRUE);
  }

  module_load_include('inc', 'blog', 'blog.pages');
  $variables['blog'] = blog_page_user($account);

  //  print_r($variables);
}


/**
 * Implements hook_file_formatter_table().
 */
function alpha_file_formatter_table($variables) {
  $links = '';
  foreach ($variables['items'] as $delta => $item) {
    $links .= '<li>' . theme('file_link', array('file' => (object) $item)) . '<span class="size">' . format_size($item['filesize']) . '</span>' . '</li>';
  }

  return empty($links) ? '' : '<ul class="file-links" >' . $links . '</ul>';
}

/**
 *
 */
function alpha_menu_user_blog_links($variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<ul class="menu nav navbar-nav">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<ul class="menu nav navbar-nav">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

/**
 * Preprocesses variables for forum-list.tpl.php.
 *
 * @param $variables
 *   An array containing the following elements:
 *   - forums: An array of all forum objects to display for the given taxonomy
 *     term ID. If tid = 0 then all the top-level forums are displayed.
 *   - parents: An array of taxonomy term objects that are ancestors of the
 *     current term ID.
 *   - tid: Taxonomy term ID of the current forum.
 *
 * @see forum-list.tpl.php
 * @see theme_forum_list()
 */
function alpha_preprocess_forum_list(&$variables) {
  foreach ($variables['forums'] as $key => $term) {
    $term_data = taxonomy_term_load($term->tid);
    if ($icon = field_get_items('taxonomy_term', $term_data, 'field_icon')) {
      $variables['forums'][$key]->awesome_icon = $icon[0]['safe_value'];
    }
    $variables['forums'][$key]->time = isset($variables['forums'][$key]->last_post->created) ? format_interval(REQUEST_TIME - $variables['forums'][$key]->last_post->created) : '';
  }
}

/**
 * Preprocesses variables for forum-topic-list.tpl.php.
 *
 * @param $variables
 *   An array containing the following elements:
 *   - tid: Taxonomy term ID of the current forum.
 *   - topics: An array of all the topics in the current forum.
 *   - forum_per_page: The maximum number of topics to display per page.
 *
 * @see forum-topic-list.tpl.php
 * @see theme_forum_topic_list()
 */
function alpha_preprocess_forum_topic_list(&$variables) {
  global $forum_topic_list_header;
  $ts = tablesort_init($forum_topic_list_header);
  $sort_header = '';
  $current_active = '';
  foreach ($forum_topic_list_header as $cell) {
    $html = _forum_tablesort_header($cell, $forum_topic_list_header, $ts);
    $sort_header .= '<li>' . $html['data'] . '</li>';
    if (isset($html['class'])) {
      $title_class = ($html['sort'] == 'asc') ? 'sort-desc' : 'sort-asc';
      $current_active = '<span class="' . $title_class . '">' . $cell['data'] . '</span>';
    }
  }
  $variables['sort_header'] = '<div class="btn-group">
  <button type="button" class="btn btn-info dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">' . $current_active . ' <span class="caret"></span></button>
<ul class="dropdown-menu"> 
' . $sort_header . '
</ul> </div>';

  foreach ($variables['topics'] as $key => $topic) {
    $variables['topics'][$key]->time = format_interval(REQUEST_TIME - $topic->last_comment_timestamp);
  }
}

/**
 *
 */
function _forum_tablesort_header($cell, $header, $ts) {
  // Special formatting for the currently sorted column header.
  if (is_array($cell) && isset($cell['field'])) {
    $title = t('sort by @s', array('@s' => $cell['data']));
    if ($cell['data'] == $ts['name']) {
      $cell['class'][] = 'sort-' . $ts['sort'];
      $cell['class'][] = 'sort-active';
      $ts['sort'] = (($ts['sort'] == 'asc') ? 'desc' : 'asc');
    }
    else {
      // If the user clicks a different header, we want to sort ascending initially.
      $ts['sort'] = 'asc';
    }
    $cell['data'] = l($cell['data'], $_GET['q'], array('attributes' => array('title' => $title, 'class' => $cell['class']), 'query' => array_merge($ts['query'], array('sort' => $ts['sort'], 'order' => $cell['data'])), 'html' => TRUE));

    unset($cell['field']);
    $cell['sort'] = $ts['sort'];
  }
  return $cell;
}

/**
 * Preprocesses variables for forum-submitted.tpl.php.
 *
 * The submission information will be displayed in the forum list and topic
 * list.
 *
 * @param $variables
 *   An array containing the following elements:
 *   - topic: The topic object.
 *
 * @see forum-submitted.tpl.php
 * @see theme_forum_submitted()
 */
function alpha_preprocess_forum_submitted(&$variables) {
  $variables['topic']->time = $variables['time'];
  $variables['topic']->author = $variables['author'];
}


/**
 *
 */
function alpha_alttracker($variables) {
  drupal_add_css(drupal_get_path('module', 'alttracker') . '/alttracker.css');

  $output = '<div class="alttracker">';
  foreach ($variables['nodes'] as $node) {
    $output .= theme('alttracker_node', array('node' => $node));
  }
  $output .= '</div>';
  return $output;
}


/**
 * Process variables for alttracker_node.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $node: Node data.
 *
 * @see alttracker_node.tpl.php
 */
function alpha_preprocess_alttracker_node(&$variables) {
  $node = $variables['node'];
  $variables['timeago'] = t('@time ago', array('@time' => format_interval(time() - $node->changed)));
  $variables['url']     = $node->url;
  $variables['title']   = check_plain($node->title);
  $variables['sticky']  = $node->sticky;
  $variables['promote'] = $node->promote;
  $variables['status']  = $node->status;
  $variables['date']    = format_date($node->created);
  $variables['name']    = theme('username', array('account' => $node));

  if (!empty($node->terms)) {
    $terms_links = array();
    foreach ($node->terms as $term) {
      $terms_links[] = array(
        'title' => check_plain($term->name),
        'href' => url("taxonomy/term/" . $term->tid),
        'html' => TRUE,
      );
    }
    $variables['term'] = theme('links', array('links' => $terms_links));
  }

  $variables['icon'] = theme('alttracker_icon', array('node' => $node));

  // Gather node classes.
  $variables['classes_array'][] = drupal_html_class('node-' . $node->type);
  if ($variables['promote']) {
    $variables['classes_array'][] = 'node-promoted';
  }
  if ($variables['sticky']) {
    $variables['classes_array'][] = 'node-sticky';
  }
  if (!$variables['status']) {
    $variables['classes_array'][] = 'node-unpublished';
  }
}

/**
 *
 */
function alpha_preprocess_pager($variables) {
  drupal_add_js(drupal_get_path('theme', 'alpha') . '/js/' . 'responsive-paginate.js');
  drupal_add_js(drupal_get_path('theme', 'alpha') . '/js/' . 'enable-responsive-paginate.js');
}

/**
 *
 */
function alpha_pager($variables) {
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // Current is the page we are currently paged to.
  $pager_current = $pager_page_array[$element] + 1;
  // First is the first page listed by this pager piece (re quantity).
  $pager_first = $pager_current - $pager_middle + 1;
  // Last is the last page listed by this pager piece (re quantity).
  $pager_last = $pager_current + $quantity - $pager_middle;
  // Max is the maximum page number.
  $pager_max = $pager_total[$element];
  // End of marker calculations.
  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.
  $li_first = theme('pager_first', array('text' => (isset($tags[0]) ? $tags[0] : '<i class="fa fa-angle-double-left"></i>'), 'element' => $element, 'parameters' => $parameters));
  $li_previous = theme('pager_previous', array('text' => (isset($tags[1]) ? $tags[1] : '<i class="fa fa-angle-left"></i>'), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_next = theme('pager_next', array('text' => (isset($tags[3]) ? $tags[3] : '<i class="fa fa-angle-right"></i>'), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_last = theme('pager_last', array('text' => (isset($tags[4]) ? $tags[4] : '<i class="fa fa-angle-double-right"></i>'), 'element' => $element, 'parameters' => $parameters));

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first pagination-prev'),
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array('pager-previous pagination-prev'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($pager_current - $i), 'parameters' => $parameters)),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('active'),
            'data' => '<a href="#">' . $i . '</a>',
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $pager_current), 'parameters' => $parameters)),
          );
        }
      }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'data' => $li_next,
        'class' => array('pagination-next'),
      );
    }
    if ($li_last) {
      $items[] = array(
        'data' => $li_last,
        'class' => array('pagination-next'),
      );
    }

    $list = _alpha_pager_item_list(array(
      'items' => $items,
      'attributes' => array('class' => array('pagination')),
      'type' => 'ul',
    ));

    return '<div class="alpha-pager"><h2 class="element-invisible">' . t('Pages')
    . '</h2>'
    . $list
    . '</div>';
  }
}

/**
 *
 */
function alpha_pager_link($variables) {
  $text = $variables['text'];
  $page_new = $variables['page_new'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $attributes = $variables['attributes'];

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query = drupal_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }

  // Set each pager link title.
  if (!isset($attributes['title'])) {
    static $titles = NULL;
    if (!isset($titles)) {
      $titles = array(
        '<i class="fa fa-angle-double-left"></i>' => t('Go to first page'),
        '<i class="fa fa-angle-left"></i>' => t('Go to previous page'),
        '<i class="fa fa-angle-right"></i>' => t('Go to next page'),
        '<i class="fa fa-angle-double-right"></i>' => t('Go to last page'),
      );
    }
    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    elseif (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }

  // @todo l() cannot be used here, since it adds an 'active' class based on the
  //   path only (which is always the current path for pager links). Apparently,
  //   none of the pager links is active at any time - but it should still be
  //   possible to use l() here.
  // @see http://drupal.org/node/1410574
  $attributes['href'] = url($_GET['q'], array('query' => $query));
  return '<a' . drupal_attributes($attributes) . '>' . $text . '</a>';
}

/**
 *
 */
function _alpha_pager_item_list($variables) {
  $items = $variables['items'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];

  // Only output the list container and title, if there are any list items.
  // Check to see whether the block title exists before adding a header.
  // Empty headers are not semantic and present accessibility challenges.
  $output = '';
  if (isset($variables['title']) && $variables['title'] !== '') {
    $output .= '<h3>' . $variables['title'] . '</h3>';
  }

  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($attributes) . '>';
    $num_items = count($items);
    $i = 0;
    foreach ($items as $item) {
      $attributes = array();
      $children = array();
      $data = '';
      $i++;
      if (is_array($item)) {
        foreach ($item as $key => $value) {
          if ($key == 'data') {
            $data = $value;
          }
          elseif ($key == 'children') {
            $children = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $data = $item;
      }
      if (count($children) > 0) {
        // Render nested list.
        $data .= _alpha_pager_item_list(array('items' => $children, 'title' => NULL, 'type' => $type, 'attributes' => $attributes));
      }
      if ($i == 1) {
        $attributes['class'][] = 'first';
      }
      if ($i == $num_items) {
        $attributes['class'][] = 'last';
      }
      $output .= '<li' . drupal_attributes($attributes) . '>' . $data . "</li>\n";
    }
    $output .= "</$type>";
  }
  $output .= '';
  return $output;
}

/**
 *
 */
function alpha_preprocess_marketplace_random_block(&$variables) {
  $variables['links']['#attributes']['class'][] = 'inline';
  $variables['links']['#links']['add']['attributes']['class'] = array('btn', 'btn-primary');
  $variables['links']['#links']['list']['attributes']['class'] = array('btn', 'btn-success');
}

function alpha_preprocess_simple_events_upcoming_block(&$variables) {
  $variables['links']['#attributes']['class'][] = 'inline';
  $variables['links']['#links']['add']['attributes']['class'] = array('btn', 'btn-primary');
  $variables['links']['#links']['list']['attributes']['class'] = array('btn', 'btn-success');
}

function alpha_preprocess_frontpage__list(&$variables) {
  $variables['links']['#attributes']['class'][] = 'inline';
  $variables['links']['#links']['list']['attributes']['class'] = array('btn', 'btn-success');
}
