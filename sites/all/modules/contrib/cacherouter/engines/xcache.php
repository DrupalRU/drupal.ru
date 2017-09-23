<?php

/**
 * @file
 *   Engine file for XCache.
 */

class xcacheCacheRouterEngine extends CacheRouterEngine {
  /**
   * page_fast_cache
   *   This tells CacheRouter to use page_fast_cache.
   *
   *   @return bool TRUE
   */
  function page_fast_cache() {
    return $this->fast_cache;
  }
  
  /**
   * get()
   *   Return item from cache if it is available.
   *
   * @param string $key
   *   The key to fetch.
   * @return mixed object|bool
   *   Returns either the cache object or FALSE on failure
   */
  function get($key) {
    $cache = parent::get($this->key($key));
    if (isset($cache)) {
      return $cache;
    }
    
    // Get item from cache    
    $cache = unserialize(xcache_get($this->key($key)));

    // Update static cache
    parent::set($this->key($key), $cache);
    
    return $cache;
  }

  /**
   * set()
   *   Add item into cache.
   *
   * @param string $key
   *   The key to set.
   * @param string $value
   *   The data to store in the cache.
   * @param string $expire
   *   The time to expire in seconds.
   * @param string $headers
   *   The page headers.
   * @return bool
   *   Returns TRUE on success or FALSE on failure
   */
  function set($key, $value, $expire = CACHE_PERMANENT, $headers = NULL) {
    if ($expire == CACHE_TEMPORARY) {
      $ttl = 1;
    }
    elseif ($expire != CACHE_PERMANENT) {
      $ttl = $expire - REQUEST_TIME;
    }
    // Create new cache object.
    $cache = new stdClass;
    $cache->cid = $key;
    $cache->created = REQUEST_TIME;
    $cache->expire = $expire;
    $cache->headers = $headers;
    $cache->data = $value;
    $cache->serialized = FALSE;

    $data = serialize($cache);

    if (!empty($key) && $this->lock()) {
      // Get lookup table to be able to keep track of bins
      $lookup = unserialize(xcache_get($this->lookup));

      // If the lookup table is empty, initialize table
      if (empty($lookup)) {
        $lookup = array();
      }

      // Set key to 1 so we can keep track of the bin
      $lookup[$this->key($key)] = 1;

      // Attempt to store full key and value
      if ($expire == CACHE_PERMANENT)
          $result = xcache_set($this->key($key), $data);
      else
          $result = xcache_set($this->key($key), $data, $ttl);
      if (!$result) {
        unset($lookup[$this->key($key)]);
        $return = FALSE;
      }
      else {
        // Update static cache
        parent::set($this->key($key), $cache);
        $return = TRUE;
      }
      
      // Resave the lookup table (even on failure)
      $lookup_data = serialize($lookup);
      xcache_set($this->lookup, $lookup_data);

      // Remove lock.
      $this->unlock();
    }

    return $return;
  }
  
  /**
   * delete()
   *   Remove item from cache.
   *
   * @param string $key
   *   The key to set.
   * @return mixed object|bool
   *   Returns either the cache object or FALSE on failure
   */
  function delete($key) {
    // Remove from static array cache.
    parent::delete($this->key($key));
    
    if (substr($key, -1, 1) == '*') {
      $key = substr($key, 0, strlen($key) - 1);
      $lookup = unserialize(xcache_get($this->lookup));
      foreach ($lookup as $k => $v) {
        if (substr($k, 0, strlen($key) - 1) == $key) {
          xcache_unset($k);
          unset($lookup[$k]);  
        }
      }
      if ($this->lock()) {
        $lookup_data = serialize($lookup);
        xcache_set($this->lookup, $lookup_data);
        $this->unlock();
      }
    }
    else {
      if (!empty($key)) {
        if (!xcache_unset($this->key($key))) {
          $return = FALSE;
        }
        else {
          $return = TRUE;
        }
      }
    }
  }

  /**
   * flush()
   *   Flush the entire cache.
   *
   * @param none
   * @return mixed bool
   *   Returns TRUE
   */
  function flush() {
    parent::flush();
    if ($this->lock()) {
      // Get lookup table to be able to keep track of bins
      $lookup = unserialize(xcache_get($this->lookup));

      // If the lookup table is empty, remove lock and return
      if (empty($lookup)) {
        $this->unlock();
        return TRUE;
      }

      // Cycle through keys and remove each entry from the cache
      foreach ($lookup as $k => $v) {
        if (xcache_unset($k)) {
          unset($lookup[$k]);
        }
      }

      // Resave the lookup table (even on failure)
      $lookup_data = serialize($lookup);
      xcache_set($this->lookup, $lookup_data);

      // Remove lock
      $this->unlock();
    }
    return TRUE;
  }

  /**
   * lock()
   *   lock the cache from other writes.
   *
   * @param none
   * @return string
   *   Returns TRUE on success, FALSE on failure
   */
  function lock() {
    // Lock once by trying to add lock file, if we can't get the lock, we will loop
    // for 3 seconds attempting to get lock.  If we still can't get it at that point,
    // then we give up and return FALSE.
    if (xcache_isset($this->lock) === TRUE) {
      $time = time();
      while (xcache_isset($this->lock) === TRUE) {
        if (time() - $time >= 3) {
          return FALSE;
        }
      }
    }
    $data = serialize(TRUE);
    xcache_set($this->lock, $data);
    return TRUE;
  }
  
  /**
   * unlock()
   *   lock the cache from other writes.
   *
   * @param none
   * @return bool
   *   Returns TRUE on success, FALSE on failure
   */
  function unlock() {
    return xcache_unset($this->lock); 
  }
}