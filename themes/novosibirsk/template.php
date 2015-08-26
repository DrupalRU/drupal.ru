<?php
/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
//* 
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<p id="navigation">'. implode(' &#8594; ', $breadcrumb) .'</p>';
  }
}
//*/

/**
 * Allow themable wrapping of all comments.
 */
//*
function phptemplate_comment_wrapper($content, $node) {
  if (!$content || $node->type == 'forum') {
    return '<div id="comments">'. $content .'</div>';
  }
  else {
    return '<div id="comments"><h2 class="comments">'. t('Comments') .'</h2></div><br>'. $content .'';
  }
}
//*/


/**
 * Generate the HTML representing a given menu item ID.
 *
 * An implementation of theme_menu_item_link()
 *
 * @param $link
 *   array The menu item to render.
 * @return
 *   string The rendered menu item.
 */
function novosibirsk_menu_item_link($link) {
  if (empty($link['options'])) {
    $link['options'] = array();
  }

  // If an item is a LOCAL TASK, render it as a tab
  if ($link['type'] & MENU_IS_LOCAL_TASK) {
    $link['title'] = '<span class="tab">' . check_plain($link['title']) . '</span>';
    $link['options']['html'] = TRUE;
  }

  if (empty($link['type'])) {
    $true = TRUE;
  }

  return l($link['title'], $link['href'], $link['options']);
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clear-block to tabs.
 */
function novosibirsk_menu_local_tasks() {
  $output = '';

  if ($primary = menu_primary_local_tasks()) {
    $output .= '<ul class="tabs primary clear-block">' . $primary . '</ul>';
  }
  if ($secondary = menu_secondary_local_tasks()) {
    $output .= '<ul class="tabs secondary clear-block">' . $secondary . '</ul>';
  }

  return $output;
}

function novosibirsk_preprocess_page(&$variables) {
  global $user;
 
  // Add a single suggestion.
  if (module_invoke('throttle', 'status') && isset($user->roles[1])) {
    $variables['template_file'] = 'page-busy';
  }

  // Add multiple suggestions.
  if (!empty($user->roles)) {
    foreach ($user->roles as $role) {
      $filter = '![^abcdefghijklmnopqrstuvwxyz0-9-_]+!s';
      $string_clean = preg_replace($filter, '-', drupal_strtolower($role));
      $variables['template_files'][] =  'page-'. $string_clean;
    }
  }
}

/*
function novosibirsk_preprocess_block(&$variables) {
	if($variables['block']->module != "user" && $variables['block']->module != "block")	{
		//Внешний вид юзерского блока у меня задается файлом шаблоном user-login.tpl.php и еще несколькими фиями в этом файле (их названия есть в user-login.tpl.php)
		// этим куском кода я настрааиваю аутпут остальных блоков
  	drupal_set_message("novosibirsk_preprocess_block: " . $variables['block']->module);
	}
}
*/

// Темизация всех списков. По идее, если я не ошибаюсь, такие списки используются только в блоках. По крайней мере модулями ядра.
// Мне нуно ко всем тэгам li добавить параметры odd и even, для раскраски внутренностей блока зеброй
function novosibirsk_item_list($items = array(), $title = NULL, $type = 'ul', $attributes = NULL) {
	$output = '<div class="item-list">';
  if (isset($title)) {
    $output .= '<h3>'. $title .'</h3>';
  }

	
  if (!empty($items)) {
    $output .= "<$type". drupal_attributes($attributes) .'>';
    $num_items = count($items);
    $zebra = " even";
    foreach ($items as $i => $item) {
    	if($zebra == " even")$zebra = " odd";
  		else $zebra = " even";
      
      $attributes = array();
      $children = array();
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
        $data .= theme_item_list($children, NULL, $type, $attributes); // Render nested list
      }
      if ($i == 0) {
        $attributes['class'] = empty($attributes['class']) ? $zebra . ' first' : ($attributes['class'] . $zebra . ' first');
      }
      if ($i == $num_items - 1) {
        $attributes['class'] = empty($attributes['class']) ? $zebra . ' last' : ($attributes['class'] . $zebra . ' last');
      }
      $output .= '<li'. drupal_attributes($attributes) .' class="' . $zebra . '">'. $data ."</li>\n";
    }
    $output .= "</$type>";
  }
  $output .= '</div>';
  return $output;
}

/**
* This snippet will register a theme implementation of the user login form.
*/
function novosibirsk_theme() {
  return array(
    'user_login_block' => array(
      'template' => 'user-login',
      'arguments' => array('form' => NULL),
    ),
    'user_login_form_name_field' => array(
      'arguments' => array('form' => NULL),
    ),
    'user_login_form_pass_field' => array(
      'arguments' => array('form' => NULL),
    ),
    'user_login_form_submit_field' => array(
      'arguments' => array('form' => NULL),
    ),
    'search_block_form' => array(
      'template' => 'search-form',
      'arguments' => array('form' => NULL),
    ),
    'search_form_block_field' => array(
      'arguments' => array('form' => NULL),
    ),    
    'search_form_block_submit' => array(
      'arguments' => array('form' => NULL),
    ),    
  );
}


function novosibirsk_user_login_form_name_field($field) {
	$output = "<fieldset id=\"login_form\"><input id=\"edit-name\" class=\"form-text required\" name=\"name\">";
	return $output;
}

function novosibirsk_user_login_form_pass_field($field) {
	$output = "<input id=\"edit-pass\" class=\"form-text required\" name=\"pass\" type=\"password\">";
	return $output;
}

function novosibirsk_user_login_form_submit_field($field) {	
	$output = "<input id=\"edit-submit\" class=\"form-submit\" type=\"submit\" value=\"Вход\" name=\"op\"/></fieldset>";
	return $output;
}

function novosibirsk_search_form_block_field($field) {
	$output = "<input id=\"edit-search\" class=\"top-search-form-input required\" name=\"search_block_form\">";
	return $output;
}

function novosibirsk_search_form_block_submit($field) {
	$output = "<input id=\"edit-submit\" class=\"top-search-form-button\" type=\"submit\" value=\"\" name=\"op\"/>";
	return $output;
}

function novosibirsk_links($links, $attributes = array('class' => 'links')) {	
  $output = '';

  if (count($links) > 0) {
    $output = '<ul'. drupal_attributes($attributes) .'>';

    $num_links = count($links);
    $i = 1;


		//foreach($links["fasttoggle_sticky"] as $k => $v) drupal_set_message($k . " = " . $v);
    foreach ($links as $key => $link) {
    	//drupal_set_message($key . " = " . $link['title']);
    	
    	/*
    	# comment_delete = удалить
# comment_edit = изменить
# comment_reply = ответить
    	*/
    	
      $class = $key;      
      //drupal_set_message($key);
			// Ссылка "Читать далее" выводится в отдельном параграфе в файле node.tpl.php
			if($key != "node_read_more") {
	      // Add first, last and active classes to the list of links to help out themers.
	      if ($i == 1) {
	        $class .= ' first';
	      }
	      if ($i == $num_links) {
	        $class .= ' last';
	      }
	      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))) {
	        $class .= ' active';
	      }
	      $output .= '<li'. drupal_attributes(array('class' => $class)) .'>';
		
	      if (isset($link['href'])) {
	      	// Обрабатываю "особые" ссылки, например от модуля Fast toggle — promoted, published, sticky и т.п.
	      	if($key == "fasttoggle_status") {
	      		$link['html'] = TRUE;	      		
	      		if($link['title'] == "опубликовано"){	      			
	      			$link['attributes']['title'] = "Опубликовано, снять с публикации?";
	      			$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/card.gif\">", $link['href'], $link);
	      		}	else {
	      			$link['attributes']['title'] = "Не опубликовано, опубликовать?";
	      			$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/card__minus.gif\">", $link['href'], $link);
	      		}
	      	} else if($key == "fasttoggle_sticky") {	      		
	      		$link['html'] = TRUE;	      		
	      		if($link['title'] == "закреплено вверху списков") {
	      			//drupal_set_message("st");
	      			$link['attributes']['title'] = "Закреплено вверху списков, открепить?";
	      			$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/flag.gif\">", $link['href'], $link);
	      		}	else {
	      			//drupal_set_message("not st");
	      			$link['attributes']['title'] = "Не закреплено вверху списков, закрепить?";
	      			$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/flag_minus.gif\">", $link['href'], $link);
	      		}
	      	} else if($key == "fasttoggle_promote") {
	      		$link['html'] = TRUE;	      		
	      		if($link['title'] == "на главной") {
	      			$link['attributes']['title'] = "Пост размещен на главной странице, убрать?";
	      			$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/home.gif\">", $link['href'], $link);
	      		}	else {
	      			$link['attributes']['title'] = "Пост не размещен на главной странице, разместить?";
	      			$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/home_minus.gif\">", $link['href'], $link);
	      		}
	      	} else if($key == "quote") {
	      		$link['html'] = TRUE;
	      		$link['attributes']['title'] = "Ответить с цитированием на отдельной странице";	      		
	      		$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/script__arrow.gif\">", $link['href'], $link);
	      	} else if($key == "click2bookmark_add_bookmark") {
	      		$link['html'] = TRUE;
	      		$link['attributes']['title'] = "Добавить сообщение в закладки?";
	      		$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/bookmark__plus.gif\">", $link['href'], $link);
	      	} else if($key == "click2bookmark_del_bookmark") {
	      		$link['html'] = TRUE;
	      		$link['attributes']['title'] = "Сообщение в ваших закладках, удалить?";
	      		$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/bookmark__minus.gif\">", $link['href'], $link);
					} else if($key == "book_printer") {
	      		$link['html'] = TRUE;
	      		$link['attributes']['title'] = "Версия для печати";
	      		$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/printer.gif\">", $link['href'], $link);
					} else if($key == "book_add_child") {
	      		$link['html'] = TRUE;
	      		$link['attributes']['title'] = "Добавить дочернюю страницу";
	      		$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/receipts_plus.gif\">", $link['href'], $link);
	      	} else if($key == "comment_delete") {
	      		$link['html'] = TRUE;
	      		$link['attributes']['title'] = "Удалить комментарий";
	      		$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/screwdriver_minus.gif\">", $link['href'], $link);
	      	} else if($key == "comment_edit") {
	      		$link['html'] = TRUE;
	      		$link['attributes']['title'] = "Редактировать комментарий";
	      		$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/screwdriver_pencil.gif\">", $link['href'], $link);
	      	} else if($key == "comment_reply") {
	      		$link['html'] = TRUE;
	      		$link['attributes']['title'] = "Ответить на этот комментарий на отдельной странице";
	      		$output .= l("<img src=\"" . base_path() . path_to_theme() . "/img/icons/script__plus.gif\">", $link['href'], $link);
	      	} else {


	        	// Pass in $link as $options, they share the same keys.
	        	$output .= l($link['title'], $link['href'], $link);
	        }
	        
	        // Добавляю кнопку "Цитата", которая вставляет выделенный текст в поле комментария внизу страницы
	        // решил добавить эту кнопку в шаблоне node.tpl.php, ибо кнопка нужна только в полном представлении ноды, а здесь отличить полную версию от тизера я не могу.
	      }
	      else if (!empty($link['title'])) {
	        // Some links are actually not links, but we wrap these in <span> for adding title and class attributes
	        if (empty($link['html'])) {
	          $link['title'] = check_plain($link['title']);
	        }
	        $span_attributes = '';
	        if (isset($link['attributes'])) {
	          $span_attributes = drupal_attributes($link['attributes']);
	        }
	        $output .= '<span'. $span_attributes .'>'. $link['title'] .'</span>';
	      }
	
	      $i++;
	      $output .= "</li>\n";
	    }
    }

    $output .= '</ul>';
  }

  return $output;
}

function novosibirsk_preprocess_user_profile_item(&$variables) {
  if ($variables['title'] == 'Что-нибудь еще о себе' || $variables['title'] == 'Резюме') {
    $variables['value'] = check_markup($variables['value'], 8, FALSE);
  }
}
