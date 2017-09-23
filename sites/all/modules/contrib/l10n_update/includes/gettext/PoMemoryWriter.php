<?php

/**
 * @file
 * Definition of Drupal\Component\Gettext\PoMemoryWriter.
 */

/**
 * Defines a Gettext PO memory writer, to be used by the installer.
 */
class PoMemoryWriter implements PoWriterInterface {

  /**
   * Array to hold all PoItem elements.
   *
   * @var array
   */
  private $_items;

  /**
   * Constructor, initialize empty items.
   */
  public function __construct() {
    $this->_items = array();
  }

  /**
   * Implements PoWriterInterface::writeItem().
   */
  public function writeItem(PoItem $item) {
    $context = $item->getContext();
    $context = $context != NULL ? $context : '';

    if ($item->isPlural()) {
      $sources = $item->getSource();
      $translations = $item->getTranslation();

      // Build additional source strings for plurals.
      $entries = array_keys($translations);
      for ($i = 3; $i <= count($entries); $i++) {
        $sources[] = $sources[1];
      }
      $translations = array_map('_locale_import_append_plural', $translations, $entries);
      $sources = array_map('_locale_import_append_plural', $sources, $entries);

      foreach ($entries as $index) {
        $this->_items[][$sources[$index]] = $translations[$index];
      }
    }
    else {
      $this->_items[$context][$item->getSource()] = $item->getTranslation();
    }
  }

  /**
   * Implements PoWriterInterface::writeItems().
   */
  public function writeItems(PoReaderInterface $reader, $count = -1) {
    $forever = $count == -1;
    while (($count-- > 0 || $forever) && ($item = $reader->readItem())) {
      $this->writeItem($item);
    }
  }

  /**
   * Get all stored PoItem's.
   *
   * @return array
   *   Array of PO item's.
   */
  public function getData() {
    return $this->_items;
  }

  /**
   * Implements Drupal\Component\Gettext\PoMetadataInterface:setLangcode().
   *
   * Not implemented. Not relevant for the MemoryWriter.
   */
  public function setLangcode($langcode) {
  }

  /**
   * Implements Drupal\Component\Gettext\PoMetadataInterface:getLangcode().
   *
   * Not implemented. Not relevant for the MemoryWriter.
   */
  public function getLangcode() {
  }

  /**
   * Implements Drupal\Component\Gettext\PoMetadataInterface:getHeader().
   *
   * Not implemented. Not relevant for the MemoryWriter.
   */
  public function getHeader() {
  }

  /**
   * Implements Drupal\Component\Gettext\PoMetadataInterface:setHeader().
   *
   * Not implemented. Not relevant for the MemoryWriter.
   */
  public function setHeader(PoHeader $header) {
  }

}
