<?php
// -*- coding: UTF-8 -*-

/**
 * @file
 * Part of the samui module. Log of the administrative actions on site.
 *
 * @author Axel <axel@drupal.ru>
 * @copyright 2008, Axel <axel@drupal.ru>, Romka <romka@drupal.ru>
 * @li GNU GPL v2
 */


function druid_admin_log() {
	$header = array('Дата', 'Админ', 'Действия', 'Объект', 'Пользователь', 'Вернуть', 'Причина');
	$result = pager_query('SELECT a.*, au.name AS admin, u.name AS user FROM {admin_log} a LEFT JOIN {users} au ON a.auid = au.uid LEFT JOIN {users} u ON a.uid = u.uid ORDER BY stamp DESC', 100);
	$rows = array();
	while($record = db_fetch_object($result)) {
		if ($record->uid) {
      if ($record->action == 'ban') {
			  $object = 'пользователя' . l(' №' . $record->uid, 'adminlog/user/' . $record->uid);
      }
      else {
      	$object = 'пользователя' . l(' №' . $record->uid, 'user/' . $record->uid);
      }
		  $type = 'user';
		}
		if ($record->nid) {
      if ($record->action == 'unp') {
      	$object = 'документ' . l(' №' . $record->nid, 'adminlog/node/' . $record->nid);
      }
      else {
        $object = 'документ' . l(' №' . $record->nid, 'node/' . $record->nid);
      }
			$type = 'node';
		}
    if ($record->cid) {
      if ($record->action == 'unp') {
        $object = 'комментарий' . l(' №' . $record->cid, 'adminlog/node/' . $record->nid . '/' . $record->cid);
      }
      else {
      	$object = 'комментарий' . l(' №' . $record->cid, 'node/' . $record->nid . '/' . $record->cid);
      }
      $type = 'comment';
    }
		$operations = ' ';
    $attributes = '';
		switch ($record->action . ' ' . $type) {
			case 'ban user':
				$operations = l('реанимировать', 'user/' . $record->uid . '/unban');
        //$attributes = 'warning';
				break;
			case 'unb user':
				$operations = l('блокировать', 'user/' . $record->uid . '/ban');
				break;
			case 'pub node':
					$operations = l('депубликовать', 'node/' . $record->nid . '/unpublish');
				break;
			case 'unp node':
				$operations = l('публиковать', 'node/' . $record->nid . '/publish');
				break;
			case 'pub commment':
				$operations = l('депубликовать', 'comment/' . $record->cid . '/toggle/status');
				break;
			case 'unp comment':
				$operations = l('публиковать', 'comment/' . $record->cid . '/toggle/status');
				break;
			case 'del user':
        $attributes = 'error';
				$operations = 'R.I.P.';
				$record->reason = 'Сволочи! Они убили Кенни!';
				break;
			case 'del node':
			case 'del comment':
			  $operations = '-';
				break;
		}
		$rows[] = array('data' => array(format_date($record->stamp), l($record->admin, 'user/' . $record->auid), druid_ban_actions($record->action), $object, $record->user, $operations, $record->reason), 'class' => $attributes);
	}
	if (!$rows) {
		return t('No any administrative actions yet.');
	}
	return theme_table($header, $rows) . theme_pager(array(), 100);
}


function druid_ban_actions($action) {
	// XXX fix html here! need themization
	$actions = array(
	 'pub' => 'опубликовал',
	 'unp' => '<strong>снял с публикации</strong>',
	 'ban' => '<strong>блокировал</strong>',
	 'unb' => 'разблокировал',
	 'pro' => 'поместил на главную',
	 'dem' => 'снял с главной',
	 'del' => '<strong>удалил</strong>',
	 'sti' => 'прикрепил',
	 'uns' => 'открепил',
	);
	return $actions[$action];
}

/**
 * Menu callback: content administration.
 */
function druid_admin_log2($form_state) {
  $form = druid_filter_form();

  $form['#theme'] = 'druid_filter_form';
  $form['admin']  = druid_admin_nodes();

  return $form;
}

/**
 * Form builder: Builds the node administration overview.
 */
function druid_admin_nodes() {

  $filter = druid_build_filter_query();

  $result = pager_query(db_rewrite_sql('SELECT n.*, u.name FROM {node} n '. $filter['join'] .' INNER JOIN {users} u ON n.uid = u.uid '. $filter['where'] .' ORDER BY n.changed DESC'), 50, 0, NULL, $filter['args']);

  // Enable language column if locale is enabled or if we have any node with language
  $count = db_result(db_query("SELECT COUNT(*) FROM {node} n WHERE language != ''"));
  $multilanguage = (module_exists('locale') || $count);

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $options = array();
  $operations = druid_node_operations();
  foreach ($operations as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('node_admin_nodes_submit'),
  );

  $languages = language_list();
  $destination = drupal_get_destination();
  $nodes = array();
  while ($node = db_fetch_object($result)) {
    $nodes[$node->nid] = '';
    $options = empty($node->language) ? array() : array('language' => $languages[$node->language]);
    $form['title'][$node->nid] = array('#value' => l($node->title, 'node/'. $node->nid, $options) .' '. theme('mark', node_mark($node->nid, $node->changed)));
    $form['name'][$node->nid] =  array('#value' => check_plain(node_get_types('name', $node)));
    $form['username'][$node->nid] = array('#value' => theme('username', $node));
    $form['status'][$node->nid] =  array('#value' => ($node->status ? t('published') : t('not published')));
    if ($multilanguage) {
      $form['language'][$node->nid] = array('#value' => empty($node->language) ? t('Language neutral') : t($languages[$node->language]->name));
    }
    $form['operations'][$node->nid] = array('#value' => l(t('edit'), 'node/'. $node->nid .'/edit', array('query' => $destination)));
  }
  $form['nodes'] = array('#type' => 'checkboxes', '#options' => $nodes);
  $form['pager'] = array('#value' => theme('pager', NULL, 50, 0));
  $form['#theme'] = 'node_admin_nodes';
  return $form;
}


/**
 * Implementation of hook_node_operations().
 */
function druid_node_operations() {
  $operations = array(
    'publish' => array(
      'label' => t('Publish'),
      'callback' => 'node_mass_update',
      'callback arguments' => array('updates' => array('status' => 1)),
    ),
    'unpublish' => array(
      'label' => t('Unpublish'),
      'callback' => 'node_mass_update',
      'callback arguments' => array('updates' => array('status' => 0)),
    ),
    'promote' => array(
      'label' => t('Promote to front page'),
      'callback' => 'node_mass_update',
      'callback arguments' => array('updates' => array('status' => 1, 'promote' => 1)),
    ),
    'demote' => array(
      'label' => t('Demote from front page'),
      'callback' => 'node_mass_update',
      'callback arguments' => array('updates' => array('promote' => 0)),
    ),
    'sticky' => array(
      'label' => t('Make sticky'),
      'callback' => 'node_mass_update',
      'callback arguments' => array('updates' => array('status' => 1, 'sticky' => 1)),
    ),
    'unsticky' => array(
      'label' => t('Remove stickiness'),
      'callback' => 'node_mass_update',
      'callback arguments' => array('updates' => array('sticky' => 0)),
    ),
    'delete' => array(
      'label' => t('Delete'),
      'callback' => NULL,
    ),
  );
  return $operations;
}

/**
 * List node administration filters that can be applied.
 */
function druid_filters() {
  // Regular filters
  $filters['status'] = array(
    'title' => t('status'),
    'options' => array(
      'status-1' => t('published'),
      'status-0' => t('not published'),
      'promote-1' => t('promoted'),
      'promote-0' => t('not promoted'),
      'sticky-1' => t('sticky'),
      'sticky-0' => t('not sticky'),
    ),
  );
  // Include translation states if we have this module enabled
  if (module_exists('translation')) {
    $filters['status']['options'] += array(
      'translate-0' => t('Up to date translation'),
      'translate-1' => t('Outdated translation'),
    );
  }

  $filters['type'] = array('title' => t('type'), 'options' => node_get_types('names'));

  // The taxonomy filter
  if ($taxonomy = module_invoke('taxonomy', 'form_all', 1)) {
    $filters['category'] = array('title' => t('category'), 'options' => $taxonomy);
  }
  // Language filter if there is a list of languages
  if ($languages = module_invoke('locale', 'language_list')) {
    $languages = array('' => t('Language neutral')) + $languages;
    $filters['language'] = array('title' => t('language'), 'options' => $languages);
  }
  return $filters;
}

/**
 * Build query for node administration filters based on session.
 */
function druid_build_filter_query() {
  $filters = druid_filters();

  // Build query
  $where = $args = array();
  $join = '';
  foreach ($_SESSION['druid_overview_filter'] as $index => $filter) {
    list($key, $value) = $filter;
    switch ($key) {
      case 'status':
        // Note: no exploitable hole as $key/$value have already been checked when submitted
        list($key, $value) = explode('-', $value, 2);
        $where[] = 'n.'. $key .' = %d';
        break;
      case 'category':
        $table = "tn$index";
        $where[] = "$table.tid = %d";
        $join .= "INNER JOIN {term_node} $table ON n.nid = $table.nid ";
        break;
      case 'type':
        $where[] = "n.type = '%s'";
        break;
      case 'language':
        $where[] = "n.language = '%s'";
        break;
    }
    $args[] = $value;
  }
  $where = count($where) ? 'WHERE '. implode(' AND ', $where) : '';

  return array('where' => $where, 'join' => $join, 'args' => $args);
}

/**
 * Return form for node administration filters.
 */
function druid_filter_form() {
  $session = &$_SESSION['druid_overview_filter'];
  $session = is_array($session) ? $session : array();
  $filters = druid_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'node_filters',
  );
  $form['#submit'][] = 'node_filter_form_submit';
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    if ($type == 'category') {
      // Load term name from DB rather than search and parse options array.
      $value = module_invoke('taxonomy', 'get_term', $value);
      $value = $value->name;
    }
    else if ($type == 'language') {
      $value = empty($value) ? t('Language neutral') : module_invoke('locale', 'language_name', $value);
    }
    else {
      $value = $filters[$type]['options'][$value];
    }
    if ($i++) {
      $form['filters']['current'][] = array('#value' => t('<em>and</em> where <strong>%a</strong> is <strong>%b</strong>', array('%a' => $filters[$type]['title'], '%b' => $value)));
    }
    else {
      $form['filters']['current'][] = array('#value' => t('<strong>%a</strong> is <strong>%b</strong>', array('%a' => $filters[$type]['title'], '%b' => $value)));
    }
    if (in_array($type, array('type', 'language'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }

  foreach ($filters as $key => $filter) {
    $names[$key] = $filter['title'];
    $form['filters']['status'][$key] = array('#type' => 'select', '#options' => $filter['options']);
  }

  $form['filters']['filter'] = array('#type' => 'radios', '#options' => $names, '#default_value' => 'status');
  $form['filters']['buttons']['submit'] = array('#type' => 'submit', '#value' => (count($session) ? t('Refine') : t('Filter')));
  if (count($session)) {
    $form['filters']['buttons']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js', 'core');

  return $form;
}

/**
 * Theme node administration filter form.
 *
 * @ingroup themeable
 */
function theme_druid_filter_form($form) {
  $output = '';
  $output .= '<div id="druid-admin-filter">';
  $output .= drupal_render($form['filters']);
  $output .= '</div>';
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme node administration filter selector.
 *
 * @ingroup themeable
 */
function theme_druid_filters($form) {
  $output = '';
  $output .= '<ul class="clear-block">';
  if (!empty($form['current'])) {
    foreach (element_children($form['current']) as $key) {
      $output .= '<li>'. drupal_render($form['current'][$key]) .'</li>';
    }
  }

  $output .= '<li><dl class="multiselect">'. (!empty($form['current']) ? '<dt><em>'. t('and') .'</em> '. t('where') .'</dt>' : '') .'<dd class="a">';
  foreach (element_children($form['filter']) as $key) {
    $output .= drupal_render($form['filter'][$key]);
  }
  $output .= '</dd>';

  $output .= '<dt>'. t('is') .'</dt><dd class="b">';

  foreach (element_children($form['status']) as $key) {
    $output .= drupal_render($form['status'][$key]);
  }
  $output .= '</dd>';

  $output .= '</dl>';
  $output .= '<div class="container-inline" id="node-admin-buttons">'. drupal_render($form['buttons']) .'</div>';
  $output .= '</li></ul>';

  return $output;
}

/**
 * Process result from node administration filter form.
 */
function druid_filter_form_submit($form, &$form_state) {
  $filters = druid_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      if (isset($form_state['values']['filter'])) {
        $filter = $form_state['values']['filter'];

        // Flatten the options array to accommodate hierarchical/nested options.
        $flat_options = form_options_flatten($filters[$filter]['options']);

        if (isset($flat_options[$form_state['values'][$filter]])) {
          $_SESSION['druid_overview_filter'][] = array($filter, $form_state['values'][$filter]);
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['druid_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['druid_overview_filter'] = array();
      break;
  }
}
