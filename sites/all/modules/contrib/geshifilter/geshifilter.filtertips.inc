<?php

/**
 * @file
 * Implementation of the filter tips.
 */

require_once drupal_get_path('module', 'geshifilter') .'/geshifilter.inc';

/**
 * Provides filter tips.
 */
function _geshifilter_filter_tips($filter, $format, $long = FALSE) {
  // Get the supported tag styles.
  $tag_styles = array_filter(_geshifilter_tag_styles($filter->format));
  $tag_style_examples = array();
  $bracket_open = NULL;
  if (in_array(GESHIFILTER_BRACKETS_ANGLE, $tag_styles)) {
    if (!$bracket_open) {
      $bracket_open = check_plain('<');
      $bracket_close = check_plain('>');
    }
    $tag_style_examples[] = '<code>'. check_plain('<foo>') .'</code>';
  }
  if (in_array(GESHIFILTER_BRACKETS_SQUARE, $tag_styles)) {
    if (!$bracket_open) {
      $bracket_open = check_plain('[');
      $bracket_close = check_plain(']');
    }
    $tag_style_examples[] = '<code>'. check_plain('[foo]') .'</code>';
  }
  if (in_array(GESHIFILTER_BRACKETS_DOUBLESQUARE, $tag_styles)) {
    if (!$bracket_open) {
      $bracket_open = check_plain('[[');
      $bracket_close = check_plain(']]');
    }
    $tag_style_examples[] = '<code>'. check_plain('[[foo]]') .'</code>';
  }
  if (!$bracket_open) {
    drupal_set_message(t('Could not determine a valid tag style for GeSHi filtering.'), 'error');
    $bracket_open = check_plain('<');
    $bracket_close = check_plain('>');
  }

  if ($long) {
    // get the available tags
    list($generic_code_tags, $language_tags, $tag_to_lang) = _geshifilter_get_tags($filter->format);
    // get the available languages
    $languages = _geshifilter_get_enabled_languages();
    $lang_attributes = _geshifilter_whitespace_explode(GESHIFILTER_ATTRIBUTES_LANGUAGE);

    // syntax highlighting tags
    $output = '<p>'. t('Syntax highlighting of source code can be enabled with the following tags:') .'</p>';
    $items = array();
    // generic tags
    $tags = array();
    foreach ($generic_code_tags as $tag) {
      $tags[] = '"<code>'. $bracket_open . $tag . $bracket_close .'</code>"';
    }
    $items[] = t('Generic syntax highlighting tags: !tags.', array('!tags' => implode(', ', $tags)));
    // language tags
    $tags = array();
    foreach ($language_tags as $tag) {
      $tags[] = t('"<code>!tag</code>" for @lang source code', array(
        '!tag' => $bracket_open . $tag . $bracket_close,
        '@lang' => $languages[$tag_to_lang[$tag]])
      );
    }
    $items[] = t('Language specific syntax highlighting tags: !tags.', array('!tags' => implode(', ', $tags)));
    // PHP specific delimiters
    if (in_array(GESHIFILTER_BRACKETS_PHPBLOCK, $tag_styles)) {
      $items[] = t('PHP source code can also be enclosed in &lt;?php ... ?&gt; or &lt;% ... %&gt;, but additional options like line numbering are not possible here.');
    }

    $output .= theme('item_list', array('items' => $items));

    // Options and tips
    $output .= '<p>'. t('Options and tips:') .'</p>';
    $items = array();

    // info about language attribute to language mapping
    $att_to_full = array();
    foreach ($languages as $langcode => $fullname) {
      $att_to_full[$langcode] = $fullname;
    }
    foreach ($tag_to_lang as $tag => $lang) {
      $att_to_full[$tag] = $languages[$lang];
    }
    ksort($att_to_full);
    $att_for_full = array();
    foreach ($att_to_full as $att => $fullname) {
      $att_for_full[] = t('"<code>@langcode</code>" (for @fullname)', array('@langcode' => $att, '@fullname' => $fullname));
    }
    $items[] = t('The language for the generic syntax highlighting tags can be specified with one of the attribute(s): %attributes. The possible values are: !languages.', array('%attributes' => implode(', ', $lang_attributes), '!languages' => implode(', ', $att_for_full)));

    // Tag style options.
    if (count($tag_style_examples) > 1) {
      $items[] = t('The supported tag styles are: !tag_styles.' , array('!tag_styles' => implode(', ', $tag_style_examples)));
    }

    // line numbering options
    $items[] = t('<em>Line numbering</em> can be enabled/disabled with the attribute "%linenumbers". Possible values are: "%off" for no line numbers, "%normal" for normal line numbers and "%fancy" for fancy line numbers (every n<sup>th</sup> line number highlighted). The start line number can be specified with the attribute "%start", which implicitly enables normal line numbering. For fancy line numbering the interval for the highlighted line numbers can be specified with the attribute "%fancy", which implicitly enables fancy line numbering.', array('%linenumbers' => GESHIFILTER_ATTRIBUTE_LINE_NUMBERING, '%off' => 'off', '%normal' => 'normal', '%fancy' => 'fancy', '%start' => GESHIFILTER_ATTRIBUTE_LINE_NUMBERING_START, '%fancy' => GESHIFILTER_ATTRIBUTE_FANCY_N));

    // block versus inline
    $items[] = t('If the source code between the tags contains a newline (e.g. immediatly after the opening tag), the highlighted source code will be displayed as a code block. Otherwise it will be displayed inline.');

    // Code block title
    $items[] = t('A title can be added to a code block with the attribute "%title".', array(
      '%title' => GESHIFILTER_ATTRIBUTE_TITLE,
    ));


    $output .= theme('item_list', array('items' => $items));

    // Defaults
    $output .= '<p>'. t('Defaults:') .'</p>';
    $items = array();
    $default_highlighting = variable_get('geshifilter_default_highlighting', GESHIFILTER_DEFAULT_PLAINTEXT);
    switch ($default_highlighting) {
      case GESHIFILTER_DEFAULT_DONOTHING:
        $description =  t('when no language attribute is specified the code block won\'t be processed by the GeSHi filter');
        break;
      case GESHIFILTER_DEFAULT_PLAINTEXT:
        $description = t('when no language attribute is specified, no syntax highlighting will be done');
        break;
      default:
        $description = t('the default language used for syntax highlighting is "%default_lang"', array('%default_lang' => $default_highlighting));
        break;
    }
    $items[] = t('Default highlighting mode for generic syntax highlighting tags: !description.', array('!description' => $description));
    $default_line_numbering = variable_get('geshifilter_default_line_numbering', GESHIFILTER_LINE_NUMBERS_DEFAULT_NONE);
    switch ($default_line_numbering) {
      case GESHIFILTER_LINE_NUMBERS_DEFAULT_NONE:
        $description = t('no line numbers');
        break;
      case GESHIFILTER_LINE_NUMBERS_DEFAULT_NORMAL:
        $description = t('normal line numbers');
        break;
      default:
        $description = t('fancy line numbers (every @n lines)', array('@n' => $default_line_numbering));
        break;
    }
    $items[] = t('Default line numbering: !description.', array('!description' => $description));
    $output .= theme('item_list', array('items' => $items));

    // Examples
    $output .= '<p>'. t('Examples:') .'</p>';
    $header = array(t('You type'), t('You get'));
    $rows = array();
    if (count($generic_code_tags)) {
      $generic_code_tag = $generic_code_tags[0];
      $lang = array_rand($languages);
      $generic_code_tag_open = $bracket_open . $generic_code_tag;
      $generic_code_tag_close = $bracket_open .'/'. $generic_code_tag . $bracket_close;
      $rows[] = array(
        '<code>'. $generic_code_tag_open . $bracket_close .'foo = "bar";'. $generic_code_tag_close .'</code>',
        t('Inline code with the default syntax highlighting mode.'),
      );
      $rows[] = array(
        '<code>'. $generic_code_tag_open . $bracket_close .'<br />foo = "bar";<br />baz = "foz";<br />'. $generic_code_tag_close .'</code>',
        t('Code block with the default syntax highlighting mode.'),
      );
      $rows[] = array(
        '<code>'. $generic_code_tag_open .' '. $lang_attributes[1 % count($lang_attributes)] .'="'. $lang .'" '. GESHIFILTER_ATTRIBUTE_LINE_NUMBERING .'="normal"'. $bracket_close .'<br />foo = "bar";<br />baz = "foz";<br />'. $generic_code_tag_close .'</code>',
        t('Code block with syntax highlighting for @lang source code<br /> and normal line numbers.', array('@lang' => $languages[$lang])),
      );
      $rows[] = array(
        '<code>'. $generic_code_tag_open .' '. $lang_attributes[2 % count($lang_attributes)] .'="'. $lang .'" '. GESHIFILTER_ATTRIBUTE_LINE_NUMBERING_START .'="23" '. GESHIFILTER_ATTRIBUTE_FANCY_N .'="7"'. $bracket_close .'<br />foo = "bar";<br />baz = "foz";<br />'. $generic_code_tag_close .'</code>',
        t('Code block with syntax highlighting for @lang source code,<br />line numbers starting from 23<br /> and highlighted line numbers every 7<sup>th</sup> line.', array('@lang' => $languages[$lang])),
      );
    }
    if (count($language_tags)) {
      $language_tag = $language_tags[0];
      $rows[] = array(
        '<code>'. $bracket_open . $language_tag . $bracket_close .'<br />foo = "bar";<br />baz = "foz";<br />'. $bracket_open .'/'. $language_tag . $bracket_close .'</code>',
        t('Code block with syntax highlighting for @lang source code.', array('@lang' => $languages[$tag_to_lang[$language_tag]])),
      );
      $rows[] = array(
        '<code>'. $bracket_open . $language_tag .' '. GESHIFILTER_ATTRIBUTE_LINE_NUMBERING_START .'="23" '. GESHIFILTER_ATTRIBUTE_FANCY_N .'="7"'. $bracket_close .'<br />foo = "bar";<br />baz = "foz";<br />'. $bracket_open . $language_tag . $bracket_close .'</code>',
        t('Code block with syntax highlighting for @lang source code,<br />line numbers starting from 23<br /> and highlighted line numbers every 7<sup>th</sup> line.', array('@lang' => $languages[$tag_to_lang[$language_tag]])),
      );
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
    return $output;
  }
  else {
    // get the available tags
    list($generic_code_tags, $language_tags, $tag_to_lang) = _geshifilter_get_tags($filter->format);
    $tags = array();
    foreach ($generic_code_tags as $tag) {
      $tags[] = '<code>'. $bracket_open . $tag . $bracket_close .'</code>';
    }
    foreach ($language_tags as $tag) {
      $tags[] = '<code>'. $bracket_open . $tag . $bracket_close .'</code>';
    }
    $output = t('You can enable syntax highlighting of source code with the following tags: !tags.', array('!tags' => implode(', ', $tags)));
    // Tag style options.
    if (count($tag_style_examples) > 1) {
      $output .= ' '. t('The supported tag styles are: !tag_styles.' , array('!tag_styles' => implode(', ', $tag_style_examples)));
    }
    if (in_array(GESHIFILTER_BRACKETS_PHPBLOCK, $tag_styles)) {
      $output .= ' '. t('PHP source code can also be enclosed in &lt;?php ... ?&gt; or &lt;% ... %&gt;.');
    }
    return $output;
  }
}
