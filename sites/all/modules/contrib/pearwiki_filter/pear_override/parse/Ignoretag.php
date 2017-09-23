<?php

class Text_Wiki_Parse_Ignoretag extends Text_Wiki_Parse {

  function Text_Wiki_Parse_Ignoretag(&$obj) {
    $this->Text_Wiki_Parse($obj);

    $tags = $this->getConf('ignore_tags');
    $tags = implode('|', explode(' ', $tags));
    $this->regex = '/<(' . $tags . ')(\s[^>]*)?(>(.*)<\/\1>|\/>)/Ums';
  }

  function process(&$matches) {
    $options = array('text' => $matches[0]);
    return $this->wiki->addToken($this->rule, $options);
  }
}
