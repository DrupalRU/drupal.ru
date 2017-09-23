<?php

/**
 * @file
 *   Engine file for eAccelerator.
 */

class eaccCacheRouterEngine extends CacheRouterEngine {
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

    $cache = eaccelerator_get($this->key($key));
    if (!empty($cache)) {
      $cache = unserialize($cache);
    }
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
    $ttl = $expire;
    if ($expire == CACHE_TEMPORARY) {
      $ttl = 180;
    }
    // Create new cache object.
    $cache = new stdClass;
    $cache->cid = $key;
    $cache->created = REQUEST_TIME;
    $cache->expire = $expire;
    $cache->headers = $headers;

    $cache->serialized = FALSE;
    $cache->data = $value;

    if (!empty($key) && $this->lock()) {
      // Get lookup table to be able to keep track of bins
      $lookup = unserialize(eaccelerator_get($this->lookup));

      // If the lookup table is empty, initialize table
      if (empty($lookup)) {
        $lookup = array();
      }

      // Set key to $expire so we can keep track of the bin
      $lookup[$this->key($key)] = $expire;

      // Attempt to store full key and value
      if (!eaccelerator_put($this->key($key), serialize($cache), $ttl)) {
        unset($lookup[$this->key($key)]);
        $return = FALSE;
      }
      else {
        // Update static cache
        parent::set($this->key($key), $cache);
        $return = TRUE;
      }

      // Resave the lookup table (even on failure)
      eaccelerator_put($this->lookup, serialize($lookup), 0);

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
   *   The key to delete.
   * @return mixed object|bool
   *   Returns either the cache object or FALSE on failure
   */
  function delete($key) {
    // Remove from static array cache.
    parent::flush();

    if (substr($key, strlen($key) - 1, 1) == '*') {
      $key = $this->key(substr($key, 0, strlen($key) - 1));
      $lookup = unserialize(eaccelerator_get($this->lookup));
      if (!empty($lookup) && is_array($lookup)) {
        foreach ($lookup as $k => $v) {
          if (substr($k, 0, strlen($key)) == $key) {
            eaccelerator_rm($k);
            unset($lookup[$k]);
          }
        }
      }
      if ($this->lock()) {
        eaccelerator_put($this->lookup, serialize($lookup));
        $this->unlock();
      }
    }
    else {
      if (!empty($key)) {
        if (!eaccelerator_rm($this->key($key))) {
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
      $lookup = eaccelerator_get($this->lookup);

      // If the lookup table is empty, remove lock and return
      if (empty($lookup)) {
        $this->unlock();
        return TRUE;
      }
      $lookup = unserialize($lookup);

      // Cycle through keys and remove each entry from the cache
      if (is_array($lookup)) {
        foreach ($lookup as $k => $v) {
          if ($v == CACHE_TEMPORARY || is_null(eaccelerator_get($k))) {
            if (eaccelerator_rm($k)) {
              unset($lookup[$k]);
            }
          }
        }
      }
      else {
      	$lookup = array();
      }

      // Resave the lookup table (even on failure)
      eaccelerator_put($this->lookup, serialize($lookup));

      // Remove lock
      $this->unlock();
      eaccelerator_gc();
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
    return eaccelerator_lock($this->lock);
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
    return  eaccelerator_unlock($this->lock);
  }

  function stats() {
    $eacc_stats = eaccelerator_info();
    $stats = array(
      'uptime' => time(),
      'bytes_used' => $eacc_stats['memoryAllocated'],
      'bytes_total' => $eacc_stats['memorySize'],
      'gets' => 0,
      'sets' => 0,
      'hits' => 0,
      'misses' => 0,
      'req_rate' => 0,
      'hit_rate' => 0,
      'miss_rate' => 0,
      'set_rate' => 0,
    );
    return $stats;
  }

}
