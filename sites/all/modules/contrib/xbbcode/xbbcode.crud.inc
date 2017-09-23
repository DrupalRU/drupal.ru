<?php

/**
 * @file
 * Data interface for creating, reading, updating and deleting records.
 */

/**
 * Check if a custom tag with a certain name exists.
 *
 * @param $name
 *   The tag name for which to search.
 *
 * @return
 *   1 if it exists, 0 if it does not.
 */
function xbbcode_custom_tag_exists($name) {
  $query = db_select('xbbcode_custom_tag', 'tag')->fields('tag')->condition('name', $name)->countQuery()->execute()->fetchCol(0);
  return $query[0];
}

/**
 * Load a custom tag.
 *
 * @param $name
 *   (optional) if passed, return the custom tag with this name.
 *   Otherwise, return all.
 *
 * @return
 *   Either a data object, or an array of objects indexed by name.
 */
function xbbcode_custom_tag_load($name = NULL) {
  $tags = &drupal_static(__FUNCTION__, array());
  if (empty($tags)) {
    $rows = db_select('xbbcode_custom_tag', 'tag')->fields('tag')->execute()->fetchAll();
    foreach ($rows as $row) {
      $row->options = unserialize($row->options);
      $tags[$row->name] = $row;
    }
  }
  if ($name) {
    return isset($tags[$name]) ? $tags[$name] : NULL;
  }
  else {
    return $tags;
  }
}

/**
 * Delete custom tags from the database.
 *
 * @param $tags
 *   An array of the names of all tags to be deleted.
 */
function xbbcode_custom_tag_delete($tags) {
  db_delete('xbbcode_custom_tag')->condition('name', $tags, 'IN')->execute();
}

/**
 * Save a custom tag.
 *
 * @param $tag
 *   A full tag object, with the options in a single array property.
 *
 * @return
 *   The result of the merge query.
 */
function xbbcode_custom_tag_save($tag) {
  $query = db_merge('xbbcode_custom_tag')->fields(array(
      'markup' => "$tag->markup",
      'description' => "$tag->description",
      'sample' => "$tag->sample",
      'options' => serialize($tag->options),
    ))->key(array('name' => "$tag->name"))->execute();
  return $query;
}

/**
 * List the names of all tags provided by any module.
 *
 * @return
 *   An array of names.
 */
function xbbcode_handler_names() {
  $names = &drupal_static(__FUNCTION__, array());
  if (empty($names)) {
    $query = db_query("SELECT DISTINCT name FROM {xbbcode_handler} ORDER BY name");
    while ($handler = $query->fetch()) {
      $names[$handler->name] = $handler->name;
    }
  }
  return $names;
}

/**
 * Load all handlers for a specific format.
 *
 * @param $format_id
 *   Optional text format ID.
 * @param $disabled
 *   Whether to load disabled handlers as well. Defaults to false.
 *
 * @return
 *   A numerically indexed array of handler objects.
 */
function xbbcode_handlers_load($format_id = XBBCODE_GLOBAL, $disabled = FALSE) {
  if ($format_id != XBBCODE_GLOBAL && !array_key_exists($format_id, xbbcode_formats('specific'))) {
    $format_id = XBBCODE_GLOBAL;
  }
  $cache = &drupal_static(__FUNCTION__, array());
  if (!isset($cache[$format_id]) || $disabled) {
    $query = db_select('xbbcode_handler', 'handler')->fields('handler')->condition('format', $format_id);
    if (!$disabled) {
      $query->condition('enabled', 1);
    }
    $all = $query->execute()->fetchAll();
    $handlers = array();
    foreach ($all as $handler) {
      $handlers[$handler->name] = $handler;
    }
    if ($disabled) {
      return $handlers;
    }
    $cache[$format_id] = $handlers;
  }

  return $cache[$format_id];
}

/**
 * Create or change a handler, assigning it to a new module.
 *
 * @param $handler
 *   A full handler object with a module, enabled, name and weight property.
 * @param $format
 *   An optional text format ID.
 */
function xbbcode_handler_save($handler, $format = XBBCODE_GLOBAL) {
  $query = db_merge('xbbcode_handler')->fields(array(
      'module' => "$handler->module",
      'enabled' => (int)$handler->enabled,
      'format' => $format,
    ))->key(array('name' => "$handler->name", 'format' => $format))->execute();
  return $query;
}

/**
 * Change a tag to a different module.
 *
 * @param $format
 *   Which format to affect (0 for global).
 * @param $name
 *   The tag name.
 * @param $module
 *   The module name to use in the future.
 */
function xbbcode_handler_update($format, $name, $module) {
  return db_update('xbbcode_handler')->fields(array('module' => $module))->condition('format', $format)->condition('name', $name)->execute();
}

/**
 * Delete a set of tags from the handlers table, affecting all formats.
 *
 * @param $tags
 *   An array of tag names to delete.
 */
function xbbcode_handlers_delete_tags($tags) {
  return db_delete('xbbcode_handler')->condition('name', $tags, 'IN')->execute();
}

/**
 * Delete all handlers for a specific format.
 *
 * @param $format_id
 *   The format ID.
 */
function xbbcode_handlers_delete_format($format_id) {
  return db_delete('xbbcode_handler')->condition('format', $format_id)->execute();
}

/**
 * List all text formats that use XBBCode.
 *
 * The formats are grouped into categories according to whether or not they use
 * format-specific settings.
 *
 * @param $type
 *   Optionally, 'specific' or 'global'.
 *
 * @return
 *   If a type argument was passed, return an array of matching formats.
 *   Otherwise, an array with two keys:
 *     - global: All text formats that depend on the global settings.
 *     - specific: All text formats that use specific settings.
 */
function xbbcode_formats($type = NULL) {
  $formats = &drupal_static(__FUNCTION__, array());
  if (empty($formats)) {
    $formats = array('specific' => array(), 'global' => array());
    foreach (filter_formats() as $format) {
      $filters = filter_list_format($format->format);
      if (isset($filters['xbbcode'])) {
        $formats[$filters['xbbcode']->settings['override'] ? 'specific' : 'global'][$format->format] = $format->name;
      }
    }
  }
  return isset($formats[$type]) ? $formats[$type] : $formats;
}
