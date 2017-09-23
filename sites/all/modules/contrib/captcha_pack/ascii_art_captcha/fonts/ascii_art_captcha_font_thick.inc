<?php
// Font definition based on figlet font "thick" (http://www.figlet.org/)
// as distributed by pyfiglet (http://sourceforge.net/projects/pyfiglet/)

function ASCII_art_captcha_font_thick() {
  $font = array();
  $font['height'] = 5;
  $font['name'] = 'thick';
  $font['comment'] = 'Thick by Randall Ransom 2/94Figlet release 2.0 -- August 5, 1993Date: 5 Mar 1994Explanation of first line:flf2 - "magic number" for file identificationa    - should always be `a\', for now$    - the "hardblank" -- prints as a blank, but can\'t be smushed5    - height of a character4    - height of a character, not including descenders15   - max line length (excluding comment lines) + a fudge factor0    - default smushmode for this font (like "-m 0" on command line)14   - number of comment lines';
  $font['a'] = array(
    '     ',
    '.d88 ',
    '8  8 ',
    '`Y88 ',
    '     ',
  );
  $font['b'] = array(
    '8    ',
    '88b. ',
    '8  8 ',
    '88P\' ',
    '     ',
  );
  $font['c'] = array(
    '     ',
    '.d8b ',
    '8    ',
    '`Y8P ',
    '     ',
  );
  $font['d'] = array(
    '   8 ',
    '.d88 ',
    '8  8 ',
    '`Y88 ',
    '     ',
  );
  $font['e'] = array(
    '      ',
    '.d88b ',
    '8.dP\' ',
    '`Y88P ',
    '      ',
  );
  $font['f'] = array(
    ' d8b ',
    ' 8\'  ',
    'w8ww ',
    ' 8   ',
    '     ',
  );
  $font['g'] = array(
    '     ',
    '.d88 ',
    '8  8 ',
    '`Y88 ',
    'wwdP ',
  );
  $font['h'] = array(
    '8     ',
    '8d8b. ',
    '8P Y8 ',
    '8   8 ',
    '      ',
  );
  $font['i'] = array(
    'w ',
    'w ',
    '8 ',
    '8 ',
    '  ',
  );
  $font['j'] = array(
    '  w ',
    '  w ',
    '  8 ',
    '  8 ',
    'wdP ',
  );
  $font['k'] = array(
    '8    ',
    '8.dP ',
    '88b  ',
    '8 Yb ',
    '     ',
  );
  $font['l'] = array(
    '8 ',
    '8 ',
    '8 ',
    '8 ',
    '  ',
  );
  $font['m'] = array(
    '          ',
    '8d8b.d8b. ',
    '8P Y8P Y8 ',
    '8   8   8 ',
    '          ',
  );
  $font['n'] = array(
    '      ',
    '8d8b. ',
    '8P Y8 ',
    '8   8 ',
    '      ',
  );
  $font['o'] = array(
    '      ',
    '.d8b. ',
    '8\' .8 ',
    '`Y8P\' ',
    '      ',
  );
  $font['p'] = array(
    '     ',
    '88b. ',
    '8  8 ',
    '88P\' ',
    '8    ',
  );
  $font['q'] = array(
    '      ',
    '.d88  ',
    '8  8  ',
    '`Y88  ',
    '   8P ',
  );
  $font['r'] = array(
    '     ',
    '8d8b ',
    '8P   ',
    '8    ',
    '     ',
  );
  $font['s'] = array(
    '     ',
    'd88b ',
    '`Yb. ',
    'Y88P ',
    '     ',
  );
  $font['t'] = array(
    ' w   ',
    'w8ww ',
    ' 8   ',
    ' Y8P ',
    '     ',
  );
  $font['u'] = array(
    '      ',
    '8   8 ',
    '8b d8 ',
    '`Y8P8 ',
    '      ',
  );
  $font['v'] = array(
    '       ',
    'Yb  dP ',
    ' YbdP  ',
    '  YP   ',
    '       ',
  );
  $font['w'] = array(
    '           ',
    'Yb  db  dP ',
    ' YbdPYbdP  ',
    '  YP  YP   ',
    '           ',
  );
  $font['x'] = array(
    '      ',
    'Yb dP ',
    ' `8.  ',
    'dP Yb ',
    '      ',
  );
  $font['y'] = array(
    '       ',
    'Yb  dP ',
    ' YbdP  ',
    '  dP   ',
    ' dP    ',
  );
  $font['z'] = array(
    '     ',
    '888P ',
    ' dP  ',
    'd888 ',
    '     ',
  );
  $font['A'] = array(
    '   db    ',
    '  dPYb   ',
    ' dPwwYb  ',
    'dP    Yb ',
    '         ',
  );
  $font['B'] = array(
    '888b. ',
    '8wwwP ',
    '8   b ',
    '888P\' ',
    '      ',
  );
  $font['C'] = array(
    '.d88b ',
    '8P    ',
    '8b    ',
    '`Y88P ',
    '      ',
  );
  $font['D'] = array(
    '888b. ',
    '8   8 ',
    '8   8 ',
    '888P\' ',
    '      ',
  );
  $font['E'] = array(
    '8888 ',
    '8www ',
    '8    ',
    '8888 ',
    '     ',
  );
  $font['F'] = array(
    '8888 ',
    '8www ',
    '8    ',
    '8    ',
    '     ',
  );
  $font['G'] = array(
    '.d88b  ',
    '8P www ',
    '8b  d8 ',
    '`Y88P\' ',
    '       ',
  );
  $font['H'] = array(
    '8   8 ',
    '8www8 ',
    '8   8 ',
    '8   8 ',
    '      ',
  );
  $font['I'] = array(
    '888 ',
    ' 8  ',
    ' 8  ',
    '888 ',
    '    ',
  );
  $font['J'] = array(
    ' 8888 ',
    '   8  ',
    'w  8  ',
    '`Yw"  ',
    '      ',
  );
  $font['K'] = array(
    '8  dP ',
    '8wdP  ',
    '88Yb  ',
    '8  Yb ',
    '      ',
  );
  $font['L'] = array(
    '8    ',
    '8    ',
    '8    ',
    '8888 ',
    '     ',
  );
  $font['M'] = array(
    '8b   d8 ',
    '8YbmdP8 ',
    '8  "  8 ',
    '8     8 ',
    '        ',
  );
  $font['N'] = array(
    '8b  8 ',
    '8Ybm8 ',
    '8  "8 ',
    '8   8 ',
    '      ',
  );
  $font['O'] = array(
    '.d88b. ',
    '8P  Y8 ',
    '8b  d8 ',
    '`Y88P\' ',
    '       ',
  );
  $font['P'] = array(
    '888b. ',
    '8  .8 ',
    '8wwP\' ',
    '8     ',
    '      ',
  );
  $font['Q'] = array(
    '.d88b. ',
    '8P  Y8 ',
    '8b wd8 ',
    '`Y88Pw ',
    '       ',
  );
  $font['R'] = array(
    '888b. ',
    '8  .8 ',
    '8wwK\' ',
    '8  Yb ',
    '      ',
  );
  $font['S'] = array(
    '.d88b. ',
    'YPwww. ',
    '    d8 ',
    '`Y88P\' ',
    '       ',
  );
  $font['T'] = array(
    '88888 ',
    '  8   ',
    '  8   ',
    '  8   ',
    '      ',
  );
  $font['U'] = array(
    '8    8 ',
    '8    8 ',
    '8b..d8 ',
    '`Y88P\' ',
    '       ',
  );
  $font['V'] = array(
    'Yb    dP ',
    ' Yb  dP  ',
    '  YbdP   ',
    '   YP    ',
    '         ',
  );
  $font['W'] = array(
    'Yb        dP ',
    ' Yb  db  dP  ',
    '  YbdPYbdP   ',
    '   YP  YP    ',
    '             ',
  );
  $font['X'] = array(
    'Yb  dP ',
    ' YbdP  ',
    ' dPYb  ',
    'dP  Yb ',
    '       ',
  );
  $font['Y'] = array(
    'Yb  dP ',
    ' YbdP  ',
    '  YP   ',
    '  88   ',
    '       ',
  );
  $font['Z'] = array(
    '8888P ',
    '  dP  ',
    ' dP   ',
    'd8888 ',
    '      ',
  );
  $font['0'] = array(
    '.d88b. ',
    '8P  Y8 ',
    '8b  d8 ',
    '`Y88P\' ',
    '       ',
  );
  $font['1'] = array(
    'd8 ',
    ' 8 ',
    ' 8 ',
    ' 8 ',
    '   ',
  );
  $font['2'] = array(
    'd88b ',
    '" dP ',
    ' dP  ',
    'd888 ',
    '     ',
  );
  $font['3'] = array(
    'd88b ',
    ' wwP ',
    '   8 ',
    'Y88P ',
    '     ',
  );
  $font['4'] = array(
    '  d8 ',
    ' dP8 ',
    'dPw8 ',
    '   8 ',
    '     ',
  );
  $font['5'] = array(
    '8888 ',
    '8ww. ',
    '  `8 ',
    'Y88P ',
    '     ',
  );
  $font['6'] = array(
    ' d88b  ',
    '8Pwww. ',
    '8b  d8 ',
    '`Y88P\' ',
    '       ',
  );
  $font['7'] = array(
    '8888P ',
    '  dP  ',
    ' dP   ',
    'dP    ',
    '      ',
  );
  $font['8'] = array(
    '.dPYb. ',
    'YbwwdP ',
    'dP""Yb ',
    '`YbdP\' ',
    '       ',
  );
  $font['9'] = array(
    '.d88b ',
    '8   8 ',
    '`8w88 ',
    '    8 ',
    '      ',
  );

  return $font;
}
