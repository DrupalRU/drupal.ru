<?php

/**
 * @file
 * Definition of Drupal\Component\Gettext\PoItem.
 */

/**
 * PoItem handles one translation.
 */
class PoItem {

  /**
   * The language code this translation is in.
   *
   * @var string
   */
  private $_langcode;

  /**
   * The context this translation belongs to.
   *
   * @var string
   */
  private $_context = '';

  /**
   * The source string or array of strings if it has plurals.
   *
   * @var string|array
   *
   * @see $_plural
   */
  private $_source;

  /**
   * Flag indicating if this translation has plurals.
   *
   * @var boolean
   */
  private $_plural;

  /**
   * The comment of this translation.
   *
   * @var string
   */
  private $_comment;

  /**
   * The text group of this translation.
   *
   * @var string
   */
  private $_textgroup;

  /**
   * The translation string or array of strings if it has plurals.
   *
   * @var string|array
   *
   * @see $_plural
   */
  private $_translation;

  /**
   * Get the language code of the currently used language.
   *
   * @return string
   *   The translation language code.
   */
  public function getLangcode() {
    return $this->_langcode;
  }

  /**
   * Set the language code of the current language.
   *
   * @param string $langcode
   *   The translation language code.
   */
  public function setLangcode($langcode) {
    $this->_langcode = $langcode;
  }

  /**
   * Get the translation group of this translation.
   *
   * @return string
   *   The translation text group.
   */
  public function getTextgroup() {
    return empty($this->_textgroup) ? 'default' : $this->_textgroup;
  }

  /**
   * Set the translation group of this translation.
   *
   * @param string $textgroup
   *   The translation text group.
   */
  public function setTextgroup($textgroup) {
    $this->_textgroup = $textgroup;
  }

  /**
   * Get the context this translation belongs to.
   *
   * @return string
   *   The translation context.
   */
  public function getContext() {
    return $this->_context;
  }

  /**
   * Set the context this translation belongs to.
   */
  public function setContext($context) {
    $this->_context = $context;
  }

  /**
   * Get the source string(s) if the translation has plurals.
   *
   * @return string|array
   *   Translation source string(s).
   */
  public function getSource() {
    return $this->_source;
  }

  /**
   * Set the source string(s) if the translation has plurals.
   *
   * @param string|array
   *   Translation source string(s).
   */
  public function setSource($source) {
    $this->_source = $source;
  }

  /**
   * Get the translation string(s) if the translation has plurals.
   *
   * @return string|array
   *   Translation string(s).
   */
  public function getTranslation() {
    return $this->_translation;
  }

  /**
   * Set the translation string(s) if the translation has plurals.
   */
  public function setTranslation($translation) {
    $this->_translation = $translation;
  }

  /**
   * Set if the translation has plural values.
   *
   * @param bool $plural
   *   The translation plural flag.
   */
  public function setPlural($plural) {
    $this->_plural = $plural;
  }

  /**
   * Get if the translation has plural values.
   *
   * @return integer $plural
   *   The translation plural flag.
   */
  public function isPlural() {
    return $this->_plural;
  }

  /**
   * Get the comment of this translation.
   *
   * @return string
   *   The translation comment.
   */
  public function getComment() {
    return $this->_comment;
  }

  /**
   * Set the comment of this translation.
   *
   * @param string $comment
   *   The translation comment.
   */
  public function setComment($comment) {
    $this->_comment = $comment;
  }

  /**
   * Create the PoItem from a structured array.
   *
   * @param array $values
   *   Keyed array with translation data.
   */
  public function setFromArray(array $values = array()) {
    if (isset($values['context'])) {
      $this->setContext($values['context']);
    }
    if (isset($values['source'])) {
      $this->setSource($values['source']);
    }
    if (isset($values['translation'])) {
      $this->setTranslation($values['translation']);
    }
    if (isset($values['comment'])) {
      $this->setComment($values['comment']);
    }
    if (isset($this->_source) && count($this->_source) > 1) {
      $this->setPlural(count($this->_translation) > 1);
    }
  }

  /**
   * Output the PoItem as a string.
   *
   * @return string
   *   PO item string value.
   */
  public function __toString() {
    return $this->formatItem();
  }

  /**
   * Format the POItem as a string.
   *
   * @return string
   *   Formatted PO item.
   */
  private function formatItem() {
    $output = '';

    // Format string context.
    if (!empty($this->_context)) {
      $output .= 'msgctxt ' . $this->formatString($this->_context);
    }

    // Format translation.
    if ($this->_plural) {
      $output .= $this->formatPlural();
    }
    else {
      $output .= $this->formatSingular();
    }

    // Add one empty line to separate the translations.
    $output .= "\n";

    return $output;
  }

  /**
   * Formats a plural translation.
   *
   * @return string
   *   Gettext formatted plural translation.
   */
  private function formatPlural() {
    $output = '';

    // Format source strings.
    $output .= 'msgid ' . $this->formatString($this->_source[0]);
    $output .= 'msgid_plural ' . $this->formatString($this->_source[1]);

    foreach ($this->_translation as $i => $trans) {
      if (isset($this->_translation[$i])) {
        $output .= 'msgstr[' . $i . '] ' . $this->formatString($trans);
      }
      else {
        $output .= 'msgstr[' . $i . '] ""' . "\n";
      }
    }

    return $output;
  }

  /**
   * Formats a singular translation.
   *
   * @return string
   *   Gettext formatted singular translation.
   */
  private function formatSingular() {
    $output = '';
    $output .= 'msgid ' . $this->formatString($this->_source);
    $output .= 'msgstr ' . (isset($this->_translation) ? $this->formatString($this->_translation) : '""');
    return $output;
  }

  /**
   * Formats a string for output on multiple lines.
   *
   * @param string $string
   *   A string.
   *
   * @return string
   *   Gettext formatted multi-line string.
   */
  private function formatString($string) {
    // Escape characters for processing.
    $string = addcslashes($string, "\0..\37\\\"");

    // Always include a line break after the explicit \n line breaks from
    // the source string. Otherwise wrap at 70 chars to accommodate the extra
    // format overhead too.
    $parts = explode("\n", wordwrap(str_replace('\n', "\\n\n", $string), 70, " \n"));

    // Multiline string should be exported starting with a "" and newline to
    // have all lines aligned on the same column.
    if (count($parts) > 1) {
      return "\"\"\n\"" . implode("\"\n\"", $parts) . "\"\n";
    }
    // Single line strings are output on the same line.
    else {
      return "\"$parts[0]\"\n";
    }
  }

}
