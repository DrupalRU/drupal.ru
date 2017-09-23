<?php

class Text_Wiki_Parse_Interwiki extends Text_Wiki_Parse {

    // double-colons wont trip up now
    var $regex = '([A-Za-z0-9_]+):((?!:)[A-Za-z0-9_\/=&~#.:;-<> ]+)';

    function parse()
    {

        // described interwiki links
        $tmp_regex = '/\[\[' . $this->regex . '(\|([^\]]*))?\]\]/';
        $this->wiki->source = preg_replace_callback(
            $tmp_regex,
            array(&$this, 'processDescr'),
            $this->wiki->source
        );

    }

    function processDescr(&$matches)
    {
        $options = array(
            'site' => $matches[1],
            'page' => $matches[2],
            'text' => $matches[4] ? $matches[4] : $matches[2]
        );
        return $this->wiki->addToken($this->rule, $options);
    }
}
