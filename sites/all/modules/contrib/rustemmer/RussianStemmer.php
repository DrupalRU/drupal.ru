<?php

/**
 * @file
 * Definition of RussianStemmer.
 */

/**
 * Implements Russian Stemming algorithm.
 */
class RussianStemmer {
  var $VERSION = "0.02";
  var $Stem_Caching = 0;
  var $Stem_Cache = array();
  var $VOWEL = '/аеиоуыэюя/u';
  var $PERFECTIVEGROUND = '/((ив|ивши|ившись|ыв|ывши|ывшись)|((?<=[ая])(в|вши|вшись)))$/u';
  var $REFLEXIVE = '/(с[яь])$/u';
  var $ADJECTIVE = '/(ее|ие|ые|ое|ими|ыми|ей|ий|ый|ой|ем|им|ым|ом|его|ого|ему|ому|их|ых|ую|юю|ая|яя|ою|ею)$/u';
  var $PARTICIPLE = '/((ивш|ывш|ующ)|((?<=[ая])(ем|нн|вш|ющ|щ)))$/u';
  var $VERB = '/((ила|ыла|ена|ейте|уйте|ите|или|ыли|ей|уй|ил|ыл|им|ым|ен|ило|ыло|ено|ят|ует|уют|ит|ыт|ены|ить|ыть|ишь|ую|ю)|((?<=[ая])(ла|на|ете|йте|ли|й|л|ем|н|ло|но|ет|ют|ны|ть|ешь|нно)))$/u';
  var $NOUN = '/(а|ев|ов|ие|ье|е|иями|ями|ами|еи|ии|и|ией|ей|ой|ий|й|иям|ям|ием|ем|ам|ом|о|у|ах|иях|ях|ы|ь|ию|ью|ю|ия|ья|я)$/u';
  var $RVRE = '/^(.*?[аеиоуыэюя])(.*)$/u';
  var $DERIVATIONAL = '/[^аеиоуыэюя][аеиоуыэюя]+[^аеиоуыэюя]+[аеиоуыэюя].*(?<=о)сть?$/u';

  function __construct() {
    $Stem_Caching = variable_get('rustemmer_stemcaching', 0);
  }

  function s(&$s, $re, $to) {
    $orig = $s;
    $s = preg_replace($re, $to, $s);
    return $orig !== $s;
  }

  function m($s, $re) {
    return preg_match($re, $s);
  }

  function stem_word($word) {
    $word = drupal_strtolower($word);
    $word = str_replace('ё','е', $word);
    // Check against cache of stemmed words
    if ($this->Stem_Caching && isset($this->Stem_Cache[$word])) {
      return $this->Stem_Cache[$word];
    }
    $stem = $word;

    do {
      if (!preg_match($this->RVRE, $word, $p)) {
        break;
      }
      $start = $p[1];
      $RV = $p[2];
      if (!$RV) {
        break;
      }

      // Step 1
      if (!$this->s($RV, $this->PERFECTIVEGROUND, '')) {
        $this->s($RV, $this->REFLEXIVE, '');

        if ($this->s($RV, $this->ADJECTIVE, '')) {
          $this->s($RV, $this->PARTICIPLE, '');
        }
        else {
          if (!$this->s($RV, $this->VERB, '')) {
            $this->s($RV, $this->NOUN, '');
          }
        }
      }

      // Step 2
      $this->s($RV, '/и$/u', '');

      // Step 3
      if ($this->m($RV, $this->DERIVATIONAL)) {
        $this->s($RV, '/ость?$/u', '');
      }

      // Step 4
      if (!$this->s($RV, '/ь$/u', '')) {
        $this->s($RV, '/ейше?/u', '');
        $this->s($RV, '/нн$/u', 'н');
      }

      $stem = $start . $RV;
    } while(FALSE);

    if ($this->Stem_Caching) {
      $this->Stem_Cache[$word] = $stem;
    }

    return $stem;
  }

}
