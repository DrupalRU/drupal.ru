<?php

/**
 * @file
 * Definition of SearchApiRussianStemmer.
 */

/**
 * Search API russian stemming processor.
 *
 * Allows searches to be case-insensitive.
 */
class SearchApiRussianStemmer extends SearchApiAbstractProcessor {
  protected function process(&$value) {
    $value = rustemmer_search_preprocess($value);
  }
}
