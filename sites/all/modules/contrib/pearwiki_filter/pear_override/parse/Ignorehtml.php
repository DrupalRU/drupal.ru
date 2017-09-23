<?php

/**
* 
* Parses for paragraph blocks.
* 
* This class implements a Text_Wiki rule to find sections of the source
* text that are paragraphs.  A para is any line not starting with a token
* delimiter, followed by two newlines.
*
* @category Text
* 
* @package Text_Wiki
* 
* @author Paul M. Jones <pmjones@php.net>
* 
*/

class Text_Wiki_Parse_Ignorehtml extends Text_Wiki_Parse {
  
  /**
  * 
  * The regular expression used to find source text matching this
  * rule.
  * 
  * @access public
  * 
  * @var string
  * 
  */
  
  var $regex = '#</?\w+(\s+\w+="[^"]*")*?/?>#';
  
  /**
  * 
  * Generates a token entry for the matched text.  Token options are:
  * 
  * 'start' => The starting point of the paragraph.
  * 
  * 'end' => The ending point of the paragraph.
  * 
  * @access public
  *
  * @param array &$matches The array of matches from parse().
  *
  * @return A delimited token number to be used as a placeholder in
  * the source text.
  *
  */
  
  function process(&$matches)
  {
    $options = array('text' => $matches[0]);
    return $this->wiki->addToken($this->rule, $options);
  }
}
?>