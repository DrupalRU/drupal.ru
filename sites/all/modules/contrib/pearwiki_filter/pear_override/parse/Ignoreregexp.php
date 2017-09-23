<?php

class Text_Wiki_Parse_Ignoreregexp extends Text_Wiki_Parse {

  function Text_Wiki_Parse_Ignoreregexp(&$obj) {
    $this->Text_Wiki_Parse($obj);
    $this->regex = $this->getConf('ignore_regexp');
  }

  var $regex = '/<($tags)(\s[^>]*)?>(.*)<\/\1>/Ums';

  function process(&$matches)
  {
    $options = array('text' => $matches[0]);
    return $this->wiki->addToken($this->rule, $options);
  }
}
