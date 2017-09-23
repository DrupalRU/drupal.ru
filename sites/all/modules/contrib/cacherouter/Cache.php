<?php

/**
 * @file
 *   Defines the base class for cache engines
 */

class CacheRouterEngine {
  var $settings = array();
  var $content = array();
  var $prefix = '';
  var $name = '';
  var $lookup = '';
  var $lock = '';
  var $fast_cache = TRUE;
  var $static = FALSE;
  var $lifetime = 0;
  
  function __construct($bin, $options, $default_options) {
    
    $this->name = $bin;
    // Setup our prefixes so that we can prefix a particular bin, or if not set use the default prefix.
    if (isset($options['prefix'])) {
      $this->prefix = $options['prefix'] .'-';
    }
    else if (!empty($default_options['prefix'])) {
      $this->prefix = $default_options['prefix'] .'-';
    }
    
    // This allows us to turn off fast_cache for cache_page so that we can get anonymous statistics.
    if (isset($options['fast_cache'])) {
      $this->fast_cache = $default_options['fast_cache'];
    }
    else if (isset($default_options['fast_cache'])) {
      $this->fast_cache = $default_options['fast_cache'];
    }
    
    // This allows us to turn off static content caching for modules/bins that are already doing this.
    if (isset($options['static'])) {
      $this->static = $options['static'];
    }
    else if (isset($default_options['static'])) {
      $this->static = $default_options['static'];
    }
    
    // Setup our prefixed lookup and lock table names for shared storage.
    $this->lookup = $this->prefix . $this->name .'_lookup';
    $this->lock = $this->prefix . $this->name .'_lock';
  }
  
  function get($key) {
    if (isset($this->content[$key]) && $this->static) {
      return $this->content[$key];
    }
  }
  
  function set($key, $value) {
    if ($this->static) {
      $this->content[$key] = $value;
    }
  }
  
  function delete($key) {
    if ($this->static) {
      unset($this->content[$key]);
    }
  }
  
  function flush() {
    if ($this->static) {
      $this->content = array();
    }
  }
  
  /**
   * key()
   *   Get the full key of the item
   *
   * @param string $key
   *   The key to set.
   * @return string
   *   Returns the full key of the cache item.
   */
  function key($key) {
    return urlencode($this->prefix . $this->name .'-'. $key);
  }
}
