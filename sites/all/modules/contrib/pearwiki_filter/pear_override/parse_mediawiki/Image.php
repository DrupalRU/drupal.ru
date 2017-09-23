<?php

class Text_Wiki_Parse_Image extends Text_Wiki_Parse {

    var $regex = '/(\[\[image:)(.+?)(\]\])/i';

    function process(&$matches)
    {
        $pos = strpos($matches[2], '|');
        if ($pos === false) {
            $options = array(
                'src' => $matches[2],
                'attr' => array());
        } else {
            list($image, $attrs) = explode('|', $matches[2], 2);
            $options = array('src' => $image, 'attr' => array());
            $parts = explode('|', $attrs);
            foreach($parts as $part) {
              if (in_array($part, array('thumbnail', 'thumb', 'frame'))) {
                if ($part == 'thumb' || $part == 'thumbnail') {
                  $options['attr']['align'] = 'right';
                  $options['attr']['width'] = '180';
                }
                else {
                
                }
                // add frame
              }
              elseif (in_array($part, array('right', 'left', 'center', 'none'))) {
                if ($part != 'none') {
                  $options['attr']['align'] = $part;
                }
              }
              elseif ($pos = strpos($part, 'px')) {
                if (strpos($part, 'x')) {
                  list($width, $height) = explode('x', substr($part, 0, $pos));
                  $options['attr']['width'] = $width;
                  $options['attr']['height'] = $height;
                }
                else {
                  $options['attr']['width'] = substr($part, 0, $pos);
                }
              }
              else {
                $options['attr']['title'] = $part;
              }
            }
        }
        return $this->wiki->addToken($this->rule, $options);
    }
}
