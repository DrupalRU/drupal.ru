<?php

/**
 * This class renders wiki links in XHTML.
 *
 * @category   Text
 * @package    Text_Wiki
 * @author     Paul M. Jones <pmjones@php.net>
 * @license    http://www.gnu.org/copyleft/lesser.html  LGPL License 2.1
 * @version    Release: @package_version@
 * @link       http://pear.php.net/package/Text_Wiki
 */
class Text_Wiki_Render_Xhtml_Wikilink extends Text_Wiki_Render {

  var $conf = array(
    'new_text' => '?',
    'new_text_pos' => 'after', // 'before', 'after', or null/false
    'css' => null,
    'css_new' => null,
    'style_new' => null,
    'exists_callback' => null, // call_user_func() callback
    'wikilinks_content_types' => array(),
  );

  /**
   * Returns drupal path for a page title.
   *
   * @param string $title
   *
   * @return string | NULL
   */
  private function pagePath($title) {
    switch ($this->getConf('wikilinks_plugin')) {
      case 'wikitools':
        // When wikitools is enabled, just create a wikitools link.
        if (function_exists('wikitools_wikilink_drupal_path')) {
          return wikitools_wikilink_drupal_path($title);
        }
      break;

      case 'freelinking':
        // When freelinking is enabled, just create a freelinking link.
        if (module_exists('freelinking')) {
          return 'freelinking/' . urlencode($title);
        }
        break;

      default:
        // Try to find the node and link to it directly.
        $nid = db_select('node', 'n')
          ->fields('n', array('nid'))
          ->where('LOWER(title) = LOWER(:title)', array(':title' => $title))
          ->condition('n.type', $this->getConf('wikilinks_content_types'))
          ->execute()->fetchField();

        return $nid ? "node/$nid" : NULL;
    }
  }

  /**
   * Renders a token into XHTML.
   *
   * @access public
   *
   * @param array $options
   *  The "options" portion of the token (second element).
   *
   * @return string
   *  The text rendered from the token options.
   */
  function token($options) {
    $page = str_replace('_', ' ', $options['page']);
    $anchor = $options['anchor'];
    $text = $options['text'];

    if ($link = $this->pagePath($page)) {
      $link_options = array();

      // Does the page exist?
      if (drupal_lookup_path('alias', $link) || drupal_lookup_path('source', $link)) {
        // PAGE EXISTS.
        $link_options['attributes']['class'][] = $this->getConf('css');

        if ($anchor) {
          $link_options['fragment'] = substr($anchor, 1);
        }

        return l($text, $link, $link_options);
      }
      else {
        // PAGE DOES NOT EXIST.
        $link_options['attributes']['class'][] = 'makenew';

        $new = $this->getConf('new_text');

        // What kind of linking are we doing?
        $pos = $this->getConf('new_text_pos');

        if (!$pos || !$new) {
          // No position (or no new_text), use css only on the page name
          return l($text, $link, $link_options);
        }
        elseif ($pos == 'before') {
          // Use the new_text BEFORE the page name.
          return l($new, $link, $link_options) . ' ' . $text;
        }
        else {
          // Default, use the new_text link AFTER the page name.
          return $text . ' ' . l($new, $link, $link_options);
        }
      }
    }
    else {
      // There is no link.
      return $text;
    }
  }
}
