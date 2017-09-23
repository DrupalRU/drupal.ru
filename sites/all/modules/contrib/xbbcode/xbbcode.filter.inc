<?php

/**
 * @file
 * The filtering class. This will be instanced for each filter, and then
 * called to process a piece of text.
 */
class XBBCodeFilter {
  var $tags;

  /**
   * Construct a filter object from a bundle of tags, and the format ID.
   *
   * @param $tags
   *   Tag array.
   * @param $format
   *   Text format ID.
   */
  function __construct($tags, $filter, $format) {
    $this->tags = $tags;
    $this->format = $format;
    $this->filter = $filter;
    $this->autoclose_tags = $filter->settings['autoclose'];
  }

  /**
   * Execute the filter on a particular text.
   *
   * Note: This function makes use of substr() and strlen() instead of Drupal
   * wrappers. This is the correct approach as all offsets are calculated by
   * the PREG_OFFSET_CAPTURE setting of preg_match_all(), which returns
   * byte offsets rather than character offsets.
   *
   * @param $text
   *   The text to be filtered.
   *
   * @return
   *   HTML code.
   */
  function process($text) {
    // Find all opening and closing tags in the text.
    preg_match_all(XBBCODE_RE_TAG, $text, $tags, PREG_SET_ORDER | PREG_OFFSET_CAPTURE);
    if (!$tags) {
      return $text;
    }

    // Initialize the stack with a root tag, and the name tracker.
    $stack = array(new XBBCodeTagMatch());
    $open_by_name = array();
    foreach ($tags as $i => $tag) {
      $tag = $tags[$i] = new XBBCodeTagMatch($tag, $this);
      $open_by_name[$tag->name] = 0;
    }

    foreach ($tags as $tag) {
      // Case 1: The tag is opening, and known to the filter.
      if (!$tag->closing && isset($this->tags[$tag->name])) {
        // Add text before the new tag to the parent, then stack the new tag.
        end($stack)->advance($text, $tag->start);

        // Stack the newly opened tag, or render it if it's selfclosing.
        if ($this->tags[$tag->name]->options->selfclosing) {
          $rendered = $this->render_tag($tag);
          if ($rendered === NULL) {
            $rendered = $tag->element;
          }
          end($stack)->append($rendered, $tag->end);
        }
        else {
          array_push($stack, $tag);
          $open_by_name[$tag->name]++;
        }
      }
      // Case 2: The tag is closing, and an opening tag exists.
      elseif ($tag->closing && !empty($open_by_name[$tag->name])) {
        // Find the last matching opening tag, breaking any unclosed tag since then.
        while (end($stack)->name != $tag->name) {
          $dangling = array_pop($stack);
          end($stack)->break_tag($dangling);
          $open_by_name[$dangling->name]--;
        }
        end($stack)->advance($text, $tag->start);
        $open_by_name[$tag->name]--;

        // If the tag forbids rendering its content, revert to the unrendered text.
        if ($this->tags[$tag->name]->options->nocode) {
          end($stack)->revert($text);
        }
        if ($this->tags[$tag->name]->options->plain) {
          // We will double-encode entities only if non-encoded chars exist.
          if (end($stack)->content != htmlspecialchars(end($stack)->content, ENT_QUOTES, 'UTF-8', FALSE)) {
            end($stack)->content = check_plain(end($stack)->content);
          }
        }

        // Append the rendered HTML to the content of its parent tag.
        $current = array_pop($stack);
        $rendered = $this->render_tag($current);
        if ($rendered === NULL) {
          $rendered = $current->element . $current->content . $tag->element;
        }
        end($stack)->append($rendered, $tag->end);
      }
    }
    end($stack)->content .= substr($text, end($stack)->offset);

    if ($this->autoclose_tags) {
      while (count($stack) > 1) {
        // Render the unclosed tag and pop it off the stack
        $output = $this->render_tag(array_pop($stack));
        end($stack)->content .= $output;
      }
    }
    else {
      while (count($stack) > 1) {
        $current = array_pop($stack);
        $content = $current->element . $current->content;
        end($stack)->content .= $content;
      }
    }

    return end($stack)->content;
  }

  /**
   * Render a single tag.
   *
   * @param $tag
   *   The complete match object, including its name, content and attributes.
   *
   * @return
   *   HTML code to insert in place of the tag and its content.
   */
  function render_tag(XBBCodeTagMatch $tag) {
    if ($callback = $this->tags[$tag->name]->callback) {
      return $callback($tag, $this);
    }
    else {
      $replace['{content}'] = $tag->content;
      $replace['{option}'] = $tag->option;
      foreach ($tag->attrs as $name => $value) {
        $replace['{' . $name . '}'] = $value;
      }

      $markup = str_replace(
        array_keys($replace),
        array_values($replace),
        $this->tags[$tag->name]->markup
      );

      // Make sure that unset placeholders are replaced with empty strings.
      $markup = preg_replace('/{\w+}/', '', $markup);

      return $markup;
    }
  }
}

class XBBCodeTagMatch {
  function __construct($regex_set = NULL) {
    if ($regex_set) {
      $this->closing = $regex_set['closing'][0] == '/';
      $this->name    = strtolower($regex_set['name'][0]);
      $this->attrs   = isset($regex_set['attrs']) ? _xbbcode_parse_attrs($regex_set['attrs'][0]) : array();
      $this->option  = isset($regex_set['option']) ? $regex_set['option'][0] : NULL;
      $this->element = $regex_set[0][0];
      $this->offset  = $regex_set[0][1] + strlen($regex_set[0][0]);
      $this->start   = $regex_set[0][1];
      $this->end     = $regex_set[0][1] + strlen($regex_set[0][0]);
    }
    else {
      $this->offset = 0;
    }
    $this->content = '';
  }

  function attr($name) {
    return isset($this->attrs[$name]) ? $this->attrs[$name] : NULL;
  }

  function break_tag($tag) {
    $this->content .= $tag->element . $tag->content;
    $this->offset = $tag->offset;
  }

  function append($text, $offset) {
    $this->content .= $text;
    $this->offset = $offset;
  }

  function advance($text, $offset) {
    $this->content .= substr($text, $this->offset, $offset - $this->offset);
    $this->offset = $offset;
  }

  function revert($text) {
    $this->content = substr($text, $this->start + strlen($this->element), $this->offset - $this->start - strlen($this->element));
  }
}
