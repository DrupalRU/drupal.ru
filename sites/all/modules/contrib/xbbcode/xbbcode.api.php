<?php

/**
 * @file
 * Hooks provided by the XBBCode module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define tags that can be used by XBBCode.
 *
 * A tag is uniquely identified by a (lowercase alphabetic) name. It must
 * include a description and a sample, both of which should already be
 * localized. It must include either a markup template or a callback function.
 *
 * If a markup template is used, then the template should contain placeholders
 * that will be replaced with the tag's content and attributes:
 * - {content}: The text between opening and closing tags (non-inclusive),
 *   assuming the tag is not self-closing.
 * - {option}: The single attribute of the tag, if one is entered.
 * - {...}: If a named attribute is entered, it will replace the placeholder
 *   of the same name. Otherwise, the placeholder is removed.
 *
 * For example, if [url=http://www.drupal.org]Drupal[/url] is entered, then
 * {content} will be replaced with "Drupal" and {option} with
 * "http://www.drupal.org". In [img width=60 height=60], {width} and {height}
 * will be replaced with "60".
 *
 *
 * @return
 *   An array keyed by tag name, each element of which must contain these keys:
 *     - EITHER markup: A string of HTML code.
 *     - OR callback: A rendering function to call.
 *       See hook_xbbcode_TAG_render() for details.
 *     - options: An array that can contain any of the following keys.
 *         - nocode: All tags inside the content of this tag will not be parsed
 *         - plain: HTML inside the content of this tag will always be escaped.
 *         - selfclosing: This tag closes itself, as in [img=http://url].
 *     - sample: For the help text, provide an example of the tag in use.
 *       This sample will be displayed along with its rendered output.
 *     - description: A description of the tag.
 *   The "sample" and "description" values should be localized with t().
 */
function hook_xbbcode_info() {
  $tags['url'] = array(
    'markup' => '<a href="{option}">{content}</a>',
    'description' => 'A hyperlink.',
    'sample' => '[url=http://drupal.org/]Drupal[/url]',
  );
  $tags['img'] = array(
    'markup' => '<img src="{option}" />',
    'options' => array(
      'selfclosing' => TRUE,
    ),
    'description' => 'An image',
    'sample' => '[img=http://drupal.org/favicon.ico]',
  );
  $tags['code'] = array(
    'markup' => '<code>{option}</code>',
    'options' => array(
      'nocode' => TRUE,
      'plain' => TRUE,
    ),
    'description' => 'Code',
    'sample' => '[code]if (x <> 3) then y = (x <= 3)[/code]',
  );
  $tags['php'] = array(
    'callback' => 'hook_xbbcode_TAG_render',
    'options' => array(
      'nocode' => TRUE,
      'plain' => TRUE,
    ),
    'description' => 'Highlighed PHP code',
    'sample' => '[php]print "Hello world";[/php]',
  );

  return $tags;
}

/**
 * Sample render callback.
 *
 * Note: This is not really a hook. The function name is manually specified
 * via the 'callback' key in hook_xbbcode_info().
 *
 * @param $tag
 *   The tag to be rendered. This object has the following properties:
 *   - name: Name of the tag
 *   - content: The text between opening and closing tags.
 *   - option: The single argument, if one was entered as in [tag=option].
 *   - attr($name): A function that returns a named attribute's value.
 * @param $xbbcode_filter
 *   The filter object that is processing the text. The process() and
 *   render_tag() functions on this object may be used to generate and render
 *   further text, but care must be taken to avoid an infinite recursion.
 *   The object will also have the following properties:
 *   - filter: Drupal's filter object, including the settings.
 *   - format: The text format object, including a list of its other filters.
 *   - tags: All tags enabled in this filter.
 *
 * @return
 *   HTML markup code. If NULL is returned, the tag will be left unrendered.
 */
function hook_xbbcode_TAG_render($tag, $xbbcode_filter) {
  return highlight_string($tag->content, TRUE);
}

/**
 * @} End of "addtogroup hooks".
 */
