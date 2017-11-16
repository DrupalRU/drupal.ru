<?php

/**
 * @file
 * Definition of PoDatabaseReader.
 */

/**
 * Gettext PO reader working with the locale module database.
 */
class PoDatabaseReader implements PoReaderInterface {

  /**
   * An associative array indicating which type of strings should be read.
   *
   * Elements of the array:
   *  - not_customized: boolean indicating if not customized strings should be
   *    read.
   *  - customized: boolean indicating if customized strings should be read.
   *  - no_translated: boolean indicating if non-translated should be read.
   *
   * The three options define three distinct sets of strings, which combined
   * cover all strings.
   *
   * @var array
   */
  private $_options;

  /**
   * Language code of the language being read from the database.
   *
   * @var string
   */
  private $_langcode;

  /**
   * Store the result of the query so it can be iterated later.
   *
   * @var resource
   */
  private $_result;

  /**
   * D7: Text group.
   *
   * @var string
   */
  protected $_textgroup;

  /**
   * Database storage to retrieve the strings from.
   *
   * @var StringDatabaseStorage
   */
  protected $storage;

  /**
   * Constructor, initializes with default options.
   */
  public function __construct() {
    $this->setOptions(array());
    $this->storage = new StringDatabaseStorage();
  }

  /**
   * Implements PoMetadataInterface::getLangcode().
   */
  public function getLangcode() {
    return $this->_langcode;
  }

  /**
   * Implements PoMetadataInterface::setLangcode().
   */
  public function setLangcode($langcode) {
    $this->_langcode = $langcode;
  }

  /**
   * Get the options used by the reader.
   */
  public function getOptions() {
    return $this->_options;
  }

  /**
   * Set the options for the current reader.
   */
  public function setOptions(array $options) {
    $options += array(
      'customized' => FALSE,
      'not_customized' => FALSE,
      'not_translated' => FALSE,
    );
    $this->_options = $options;
  }

  /**
   * @param \string $textgroup
   */
  public function setTextgroup($textgroup) {
    $this->_textgroup = $textgroup;
  }

  /**
   * Implements PoMetadataInterface::getHeader().
   */
  public function getHeader() {
    return new PoHeader($this->getLangcode());
  }

  /**
   * Implements PoMetadataInterface::setHeader().
   *
   * @throws Exception
   *   Always, because you cannot set the PO header of a reader.
   */
  public function setHeader(PoHeader $header) {
    throw new \Exception('You cannot set the PO header in a reader.');
  }

  /**
   * Builds and executes a database query based on options set earlier.
   */
  private function loadStrings() {
    $langcode = $this->_langcode;
    $options = $this->_options;
    $textgroup = $this->_textgroup;
    $conditions = array();
    if ($textgroup) {
      $conditions['textgroup'] = $textgroup;
    }

    if (array_sum($options) == 0) {
      // If user asked to not include anything in the translation files,
      // that would not make sense, so just fall back on providing a template.
      $langcode = NULL;
      // Force option to get both translated and untranslated strings.
      $options['not_translated'] = TRUE;
    }
    // Build and execute query to collect source strings and translations.
    if (!empty($langcode)) {
      $conditions['language'] = $langcode;
      // Translate some options into field conditions.
      if ($options['customized']) {
        if (!$options['not_customized']) {
          // Filter for customized strings only.
          $conditions['customized'] = L10N_UPDATE_CUSTOMIZED;
        }
        // Else no filtering needed in this case.
      }
      else {
        if ($options['not_customized']) {
          // Filter for non-customized strings only.
          $conditions['customized'] = L10N_UPDATE_NOT_CUSTOMIZED;
        }
        else {
          // Filter for strings without translation.
          $conditions['translated'] = FALSE;
        }
      }
      if (!$options['not_translated']) {
        // Filter for string with translation.
        $conditions['translated'] = TRUE;
      }
      return $this->storage->getTranslations($conditions);
    }
    else {
      // If no language, we don't need any of the target fields.
      return $this->storage->getStrings($conditions);
    }
  }

  /**
   * Get the database result resource for the given language and options.
   */
  private function readString() {
    if (!isset($this->_result)) {
      $this->_result = $this->loadStrings();
    }
    return array_shift($this->_result);
  }

  /**
   * Implements PoReaderInterface::readItem().
   */
  public function readItem() {
    if ($string = $this->readString()) {
      $values = (array) $string;
      $poItem = new PoItem();
      $poItem->setFromArray($values);
      return $poItem;
    }
  }

}
