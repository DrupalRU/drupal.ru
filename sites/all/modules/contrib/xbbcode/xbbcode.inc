<?php

/**
 * @file
 * General library of internal functions only called by this module.
 */

/**
 * Create or load a parser object.
 *
 * @param $filter
 *   The filter object containing settings for the given format.
 * @param $format
 *   The text format object assigned to the text to be filtered.
 *
 * @return
 *   The XBBCodeFilter object.
 */
function _xbbcode_build_filter($filter, $format) {
  $filters = &drupal_static(__FUNCTION__, array());
  if (!isset($filters[$format->format])) {
    $tags = _xbbcode_build_tags($format->format);
    $filters[$format->format] = new XBBCodeFilter($tags, $filter, $format);
  }
  return $filters[$format->format];
}

/**
 * Discover the handlers by module hook invokation.
 *
 * @return
 *   An array keyed by tag name. Each element is an array.
 *   - the 'modules' key contains a list of public module names (keyed by 
 *     internal name) that provide this tag.
 *   - the 'info' key contains the data returned by each module's hook.
 */
function _xbbcode_build_handlers() {
  $module_names = _xbbcode_module_names();
  $handlers = array();

  foreach ($module_names as $module => $name) {
    $tags = module_invoke($module, 'xbbcode_info');
    foreach ($tags as $tag_name => $info) {
      $handlers[$tag_name]['modules'][$module] = $name;
      $handlers[$tag_name]['info'][$module] = $info;
    }
  }
  ksort($handlers);
  return $handlers;
}

/**
 * Invoke all handlers to get the tags for a certain format.
 *
 * @param $format_id
 *   The name of the format for which tags should be built.
 *
 * @return
 *   An array of tag objects, keyed by name.
 */
function _xbbcode_build_tags($format_id) {
  // First, check if the tags are in cache.
  if ($cache = cache_get("xbbcode_tags:$format_id")) {
    $tags = $cache->data;
  }
  else {
    // Load the database interface.
    module_load_include('inc', 'xbbcode', 'xbbcode.crud');
    // Load the preferred handlers for this text format.
    $handlers = xbbcode_handlers_load($format_id);
    $providers = array();
    foreach ($handlers as $handler) {
      // Build a list of what modules are used, and what to get from each.
      $providers[$handler->module][$handler->name] = $handler->name;
    }

    $default = array(
      'selfclosing' => FALSE,
      'nocode' => FALSE,
      'plain' => FALSE,
    );
    foreach ($providers as $module => $provides) {
      $info = module_invoke($module, 'xbbcode_info');
      foreach ($provides as $tag) {
        if (isset($info[$tag])) {
          if (!isset($info[$tag]['options'])) {
            $info[$tag]['options'] = array();
          }
          $tags[$tag] = (object)array(
            'name'        => $tag,
            'description' => $info[$tag]['description'],
            'sample'      => $info[$tag]['sample'],
            'options'     => (object) ($info[$tag]['options'] + $default),
            'markup'      => isset($info[$tag]['markup']) ? $info[$tag]['markup'] : NULL,
            'callback'    => !isset($info[$tag]['markup']) ? $info[$tag]['callback'] : NULL,
          );
        }
      }
    }
    cache_set("xbbcode_tags:$format_id", $tags);
  }
  return $tags;
}

/**
 * Concatenate a tag's descriptions, making all but one invisible.
 *
 * @param $tag
 *   The name of the tag.
 * @param $handlers
 *   The info from each module that provides the tag, keyed by module name.
 * @param $selected
 *   (optional for a single handler) The name of the active handler.
 *
 * @return
 *   The HTML code.
 */
function _xbbcode_build_descriptions($name, $handlers, $selected = NULL) {
  if (count($handlers) == 1) {
    $selected = key($handlers);
  }

  $descriptions = "<strong>[$name]</strong>\n";
  foreach ($handlers as $module => $info) {
    $class = $selected && ($module == $selected) ? 'visible' : 'invisible';
    if (isset($info['description'])) {
      $descriptions .= "<p class='tag-$name module-$module xbbcode-description-$class'>{$info['description']}</p>\n";
    }
  }

  return $descriptions;
}

/**
 * End-user names of all modules that implement hook_xbbcode_info().
 *
 * These names are displayed on the settings form when choosing a module that
 * should handle each tag. The xbbcode module itself is renamed "Custom" since
 * its tags are the ones entered through the admin interface.
 */
function _xbbcode_module_names() {
  $modules = &drupal_static(__FUNCTION__, array());
  if (!$modules) {
    $info = system_get_info('module');
    foreach (module_implements('xbbcode_info') as $module) {
      $modules[$module] = $info[$module]['name'];
    }
    $modules['xbbcode'] = 'Custom';
  }
  return $modules;
}

/**
 * Parse a string of attribute assignments.
 *
 * @param $string
 *   The string containing the arguments, including initial whitespace.
 *
 * @return
 *   An associative array of all attributes.
 */
function _xbbcode_parse_attrs($string) {
  preg_match_all('/' . XBBCODE_RE_ATTR . '/', $string, $assignments, PREG_SET_ORDER);
  $attrs = array();
  foreach ($assignments as $assignment) {
    $attrs[$assignment['key']] = $assignment['value'];
  }
  return $attrs;
}
