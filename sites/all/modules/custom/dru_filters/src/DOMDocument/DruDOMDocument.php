<?php

/**
 * @file
 *      Extend DOMDocument and rewrite saveHTML method for cut DTD.
 */

namespace Drupal\dru_filters\DOMDocument;

use DOMDocument;
use DOMNode;

/**
 * Class DruDOMDocument
 *
 * @package Drupal\dru_filters\DOMDocument
 */
class DruDOMDocument extends DOMDocument {

  public function saveHTML(DOMNode $node = null) {
    return preg_replace('/^<!DOCTYPE.+?>/',
      '',
      str_replace(['<html>', '</html>', '<body>', '</body>'], ['', '', '', ''], parent::saveHTML()));
  }
}
