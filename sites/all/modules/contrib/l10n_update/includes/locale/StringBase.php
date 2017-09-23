<?php

/**
 * @file
 * Definition of StringBase.
 */

/**
 * Defines the locale string base class.
 *
 * This is the base class to be used for locale string objects and contains
 * the common properties and methods for source and translation strings.
 */
abstract class StringBase implements StringInterface {

  /**
   * The string identifier.
   *
   * @var integer
   */
  public $lid;

  /**
   * The parent string identifier for plural translations.
   *
   * @var integer
   */
  public $plid;

  /**
   * Plural index in case of plural string.
   *
   * @var integer
   */
  public $plural;

  /**
   * The string locations indexed by type.
   *
   * @var string
   */
  public $locations;

  /**
   * The source string.
   *
   * @var string
   */
  public $source;

  /**
   * The string context.
   *
   * @var string
   */
  public $context;

  /**
   * The string group.
   *
   * @var string
   */
  public $textgroup;

  /**
   * The string version.
   *
   * @var string
   */
  public $version;

  /**
   * The locale storage this string comes from or is to be saved to.
   *
   * @var StringStorageInterface
   */
  protected $storage;

  /**
   * Constructs a new locale string object.
   *
   * @param object|array $values
   *   Object or array with initial values.
   */
  public function __construct($values = array()) {
    $this->setValues((array) $values);
  }

  /**
   * Implements StringInterface::getId().
   */
  public function getId() {
    return isset($this->lid) ? $this->lid : NULL;
  }

  /**
   * Implements StringInterface::setId().
   */
  public function setId($lid) {
    $this->lid = $lid;
    return $this;
  }

  /**
   * Implements StringInterface::getParentId().
   */
  public function getParentId() {
    return isset($this->plid) ? $this->plid : 0;
  }

  /**
   * Implements StringInterface::setParentId().
   */
  public function setParentId($plid) {
    $this->plid = $plid;
    return $this;
  }

  /**
   * Implements StringInterface::getVersion().
   */
  public function getVersion() {
    return isset($this->version) ? $this->version : NULL;
  }

  /**
   * Implements StringInterface::setVersion().
   */
  public function setVersion($version) {
    $this->version = $version;
    return $this;
  }

  /**
   * Implements StringInterface::getStorage().
   */
  public function getStorage() {
    return isset($this->storage) ? $this->storage : NULL;
  }

  /**
   * Implements StringInterface::setStorage().
   */
  public function setStorage(StringStorageInterface $storage) {
    $this->storage = $storage;
    return $this;
  }

  /**
   * Implements StringInterface::setValues().
   */
  public function setValues(array $values, $override = TRUE) {
    foreach ($values as $key => $value) {
      if (property_exists($this, $key) && ($override || !isset($this->$key))) {
        $this->$key = $value;
      }
    }
    return $this;
  }

  /**
   * Implements StringInterface::getValues().
   */
  public function getValues(array $fields) {
    $values = array();
    foreach ($fields as $field) {
      if (isset($this->$field)) {
        $values[$field] = $this->$field;
      }
    }
    return $values;
  }

  /**
   * Implements StringInterface::getTextgroup().
   */
  public function getTextgroup() {
    return empty($this->textgroup) ? 'default' : $this->textgroup;
  }

  /**
   * Implements StringInterface::setTextgroup().
   */
  public function setTextgroup($textgroup) {
    $this->textgroup = $textgroup;
  }

  /**
   * Implements LocaleString::save().
   */
  public function save() {
    if ($storage = $this->getStorage()) {
      $storage->save($this);
    }
    else {
      throw new StringStorageException(format_string('The string cannot be saved because its not bound to a storage: @string', array(
        '@string' => $this->getString(),
      )));
    }
    return $this;
  }

  /**
   * Implements LocaleString::delete().
   */
  public function delete() {
    if (!$this->isNew()) {
      if ($storage = $this->getStorage()) {
        $storage->delete($this);
      }
      else {
        throw new StringStorageException(format_string('The string cannot be deleted because its not bound to a storage: @string', array(
          '@string' => $this->getString(),
        )));
      }
    }
    return $this;
  }

}
