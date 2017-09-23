<?php

/**
 * @file
 * Implementation of the conflict detection feature of the GeSHi filter.
 */

require_once drupal_get_path('module', 'geshifilter') .'/geshifilter.inc';

/**
 * Menu callback for filter conflicts page
 */
function geshifilter_admin_filter_conflicts($check_only=FALSE) {
  global $user;

  // start
  $output = '';
  // check if GeSHi library is available
  $geshi_library = libraries_load('geshi');
  if (!$geshi_library['loaded']) {
    if (!$check_only) {
      drupal_set_message($geshi_library['error message'], 'error');
    }
    return $output;
  }
  $alerts = array();
  $conflict_detectors = array(
    'filter/0' => '_geshifilter_htmlfilter_conflicts',
    'codefilter/0' => '_geshifilter_codefilter_conflicts',
  );
  foreach (filter_formats($user) as $format => $input_format) {
    // Get the filters in this text format
    $filters = filter_list_format($format);
    // look if GeSHi is enabled in this text format
    if (isset($filters['geshifilter/0'])) {
      $geshifilter = $filters['geshifilter/0'];
      // Check if possibly conflicting filters are also present in text format
      foreach ($conflict_detectors as $filter_key => $conflict_detector) {
        // does this filter exist in the text format?
        if (array_key_exists($filter_key, $filters)) {
          $cfilter = $filters[$filter_key];
          $conflicts = $conflict_detector($format, $cfilter, $geshifilter);
          foreach ($conflicts as $conflict) {
            $alerts[] = array(
              l(t($input_format->name), "admin/config/content/formats/$format"),
              $cfilter->name,
              $conflict['description'],
              $conflict['solution'],
            );
          }
        }
      }
    }
  }
  if ($check_only) {
    return count($alerts);
  }
  else {
    // show alerts
    if (count($alerts) == 0) {
      $alerts[] = array(array('data' => t('No known filter conflicts were detected.'), 'colspan' => 4));
    }
    $header = array(t('Input format'), t('Filter'), t('Description'), t('Possible solutions'));
    $output .= theme('table', array('header' => $header, 'rows' => $alerts));
    return $output;
  }
}

/**
 * conflict detection for html filter
 */
function _geshifilter_htmlfilter_conflicts($format, $cfilter, $geshifilter) {
  $conflicts = array();
  // check order
  if ($cfilter->weight >= $geshifilter->weight) {
    $conflicts[] = array(
      'description' => t('%cfilter should not come after %geshifilter to prevent loss of layout and highlighting.',
          array('%cfilter' => $cfilter->name, '%geshifilter' => $geshifilter->name)),
      'solution' => l(t('Rearrange filters'), "admin/config/content/formats/$format"),
    );
  }
  // check tag escaping of html filter
  if (variable_get("filter_html_$format", FILTER_HTML_STRIP) == FILTER_HTML_ESCAPE) {
    $conflicts[] = array(
      'description' => t('%cfilter is configured to "Escape all tags", which is likely to cause problems with %geshifilter.',
        array('%cfilter' => $cfilter->name, '%geshifilter' => $geshifilter->name)),
      'solution' => l(t('Configure HTML filtering to "Strip disallowed tags"'), "admin/config/content/formats/$format", array('html' => TRUE)),
    );
  }
  return $conflicts;
}

/**
 * Conflict detection for codefilter.
 */
function _geshifilter_codefilter_conflicts($format, $cfilter, $geshifilter) {
  $conflicts = array();
  if (in_array(GESHIFILTER_BRACKETS_ANGLE, array_filter(_geshifilter_tag_styles($format)))) {
    list($generic_code_tags, $language_tags, $tag_to_lang) = _geshifilter_get_tags($format);
    if (in_array('code', $generic_code_tags) || in_array('code', $language_tags)) {
      $conflicts[] = array(
        'description' => t('%cfilter and %geshifilter trigger on the same tag "&lt;code&gt;".',
          array('%cfilter' => $cfilter->name, '%geshifilter' => $geshifilter->name)),
        'solution' => t('Remove "code" as generic syntax highlighting tag for %geshifilter, limit %geshifilter to tag style "[foo]" only or disable %cfilter',
          array('%cfilter' => $cfilter->name, '%geshifilter' => $geshifilter->name)),
      );
    }
  }
  return $conflicts;
}
