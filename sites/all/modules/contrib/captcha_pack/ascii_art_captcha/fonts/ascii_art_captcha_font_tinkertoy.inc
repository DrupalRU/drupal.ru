<?php
// Font definition based on figlet font "tinker-toy" (http://www.figlet.org/)
// as distributed by pyfiglet (http://sourceforge.net/projects/pyfiglet/)

function ASCII_art_captcha_font_tinkertoy() {
  $font = array();
  $font['height'] = 7;
  $font['name'] = 'tinker-toy';
  $font['comment'] = 'Tinker-toy by Wendell Hicken 11/93 (whicken@parasoft.com)Explanation of first line:flf2 - "magic number" for file identificationa    - should always be `a\', for now$    - the "hardblank" -- prints as a blank, but can\'t be smushed7    - height of a character5    - height of a character, not including descenders5    - max line length (excluding comment lines) + a fudge factor15   - default smushmode for this font (like "-m 15" on command line)12   - number of comment lines';
  $font['a'] = array(
    '    ',
    '    ',
    ' oo ',
    '| | ',
    'o-o-',
    '    ',
    '    ',
  );
  $font['b'] = array(
    'o    ',
    '|    ',
    'O-o  ',
    '|  | ',
    'o-o  ',
    '     ',
    '     ',
  );
  $font['c'] = array(
    '     ',
    '     ',
    ' o-o ',
    '|    ',
    ' o-o ',
    '     ',
    '     ',
  );
  $font['d'] = array(
    '   o ',
    '   | ',
    ' o-O ',
    '|  | ',
    ' o-o ',
    '     ',
    '     ',
  );
  $font['e'] = array(
    '    ',
    '    ',
    'o-o ',
    '|-\' ',
    'o-o ',
    '    ',
    '    ',
  );
  $font['f'] = array(
    ' o-o',
    ' |  ',
    '-O- ',
    ' |  ',
    ' o  ',
    '    ',
    '    ',
  );
  $font['g'] = array(
    '     ',
    '     ',
    'o--o ',
    '|  | ',
    'o--O ',
    '   | ',
    'o--o ',
  );
  $font['h'] = array(
    'o    ',
    '|    ',
    'O--o ',
    '|  | ',
    'o  o ',
    '     ',
    '     ',
  );
  $font['i'] = array(
    '  ',
    'o ',
    '  ',
    '| ',
    '| ',
    '  ',
    '  ',
  );
  $font['j'] = array(
    '      ',
    '    o ',
    '      ',
    '    o ',
    '    | ',
    'o   o ',
    ' o-o  ',
  );
  $font['k'] = array(
    'o    ',
    '| /  ',
    'OO   ',
    '| \\  ',
    'o  o ',
    '     ',
    '     ',
  );
  $font['l'] = array(
    'o ',
    '| ',
    '| ',
    '| ',
    'o ',
    '  ',
    '  ',
  );
  $font['m'] = array(
    '      ',
    '      ',
    'o-O-o ',
    '| | | ',
    'o o o ',
    '      ',
    '      ',
  );
  $font['n'] = array(
    '     ',
    '     ',
    'o-o  ',
    '|  | ',
    'o  o ',
    '     ',
    '     ',
  );
  $font['o'] = array(
    '    ',
    '    ',
    'o-o ',
    '| | ',
    'o-o ',
    '    ',
    '    ',
  );
  $font['p'] = array(
    '     ',
    '     ',
    'o-o  ',
    '|  | ',
    'O-o  ',
    '|    ',
    'o    ',
  );
  $font['q'] = array(
    '     ',
    '     ',
    ' o-o ',
    '|  | ',
    ' o-O ',
    '   | ',
    '   o ',
  );
  $font['r'] = array(
    '    ',
    '    ',
    'o-o ',
    '|   ',
    'o   ',
    '    ',
    '    ',
  );
  $font['s'] = array(
    '    ',
    '    ',
    'o-o ',
    ' \\  ',
    'o-o ',
    '    ',
    '    ',
  );
  $font['t'] = array(
    ' o  ',
    ' |  ',
    '-o- ',
    ' |  ',
    ' o  ',
    '    ',
    '    ',
  );
  $font['u'] = array(
    '     ',
    '     ',
    'o  o ',
    '|  | ',
    'o--o ',
    '     ',
    '     ',
  );
  $font['v'] = array(
    '      ',
    '      ',
    'o   o ',
    ' \\ /  ',
    '  o   ',
    '      ',
    '      ',
  );
  $font['w'] = array(
    '         ',
    '         ',
    'o   o   o',
    ' \\ / \\ / ',
    '  o   o  ',
    '         ',
    '         ',
  );
  $font['x'] = array(
    '    ',
    '    ',
    '\\ / ',
    ' o  ',
    '/ \\ ',
    '    ',
    '    ',
  );
  $font['y'] = array(
    '     ',
    '     ',
    'o  o ',
    '|  | ',
    'o--O ',
    '   | ',
    'o--o ',
  );
  $font['z'] = array(
    '    ',
    '    ',
    'o-o ',
    ' /  ',
    'o-o ',
    '    ',
    '    ',
  );
  $font['A'] = array(
    '  O  ',
    ' / \\ ',
    'o---o',
    '|   |',
    'o   o',
    '     ',
    '     ',
  );
  $font['B'] = array(
    'o--o  ',
    '|   | ',
    'O--o  ',
    '|   | ',
    'o--o  ',
    '      ',
    '      ',
  );
  $font['C'] = array(
    '  o-o ',
    ' /    ',
    'O     ',
    ' \\    ',
    '  o-o ',
    '      ',
    '      ',
  );
  $font['D'] = array(
    'o-o   ',
    '|  \\  ',
    '|   O ',
    '|  /  ',
    'o-o   ',
    '      ',
    '      ',
  );
  $font['E'] = array(
    'o--o ',
    '|    ',
    'O-o  ',
    '|    ',
    'o--o ',
    '     ',
    '     ',
  );
  $font['F'] = array(
    'o--o ',
    '|    ',
    'O-o  ',
    '|    ',
    'o    ',
    '     ',
    '     ',
  );
  $font['G'] = array(
    ' o-o  ',
    'o     ',
    '|  -o ',
    'o   | ',
    ' o-o  ',
    '      ',
    '      ',
  );
  $font['H'] = array(
    'o  o ',
    '|  | ',
    'O--O ',
    '|  | ',
    'o  o ',
    '     ',
    '     ',
  );
  $font['I'] = array(
    'o-O-o ',
    '  |   ',
    '  |   ',
    '  |   ',
    'o-O-o ',
    '      ',
    '      ',
  );
  $font['J'] = array(
    '    o ',
    '    | ',
    '    | ',
    '\\   o ',
    ' o-o  ',
    '      ',
    '      ',
  );
  $font['K'] = array(
    'o  o ',
    '| /  ',
    'OO   ',
    '| \\  ',
    'o  o ',
    '     ',
    '     ',
  );
  $font['L'] = array(
    'o    ',
    '|    ',
    '|    ',
    '|    ',
    'O---o',
    '     ',
    '     ',
  );
  $font['M'] = array(
    'o   o ',
    '|\\ /| ',
    '| O | ',
    '|   | ',
    'o   o ',
    '      ',
    '      ',
  );
  $font['N'] = array(
    'o   o ',
    '|\\  | ',
    '| \\ | ',
    '|  \\| ',
    'o   o ',
    '      ',
    '      ',
  );
  $font['O'] = array(
    ' o-o  ',
    'o   o ',
    '|   | ',
    'o   o ',
    ' o-o  ',
    '      ',
    '      ',
  );
  $font['P'] = array(
    'o--o  ',
    '|   | ',
    'O--o  ',
    '|     ',
    'o     ',
    '      ',
    '      ',
  );
  $font['Q'] = array(
    ' o-o  ',
    'o   o ',
    '|   | ',
    'o   O ',
    ' o-O\\ ',
    '      ',
    '      ',
  );
  $font['R'] = array(
    'o--o  ',
    '|   | ',
    'O-Oo  ',
    '|  \\  ',
    'o   o ',
    '      ',
    '      ',
  );
  $font['S'] = array(
    ' o-o  ',
    '|     ',
    ' o-o  ',
    '    | ',
    'o--o  ',
    '      ',
    '      ',
  );
  $font['T'] = array(
    'o-O-o ',
    '  |   ',
    '  |   ',
    '  |   ',
    '  o   ',
    '      ',
    '      ',
  );
  $font['U'] = array(
    'o   o ',
    '|   | ',
    '|   | ',
    '|   | ',
    ' o-o  ',
    '      ',
    '      ',
  );
  $font['V'] = array(
    'o   o ',
    '|   | ',
    'o   o ',
    ' \\ /  ',
    '  o   ',
    '      ',
    '      ',
  );
  $font['W'] = array(
    'o       o ',
    '|       | ',
    'o   o   o ',
    ' \\ / \\ /  ',
    '  o   o   ',
    '          ',
    '          ',
  );
  $font['X'] = array(
    'o   o ',
    ' \\ /  ',
    '  O   ',
    ' / \\  ',
    'o   o ',
    '      ',
    '      ',
  );
  $font['Y'] = array(
    'o   o ',
    ' \\ /  ',
    '  O   ',
    '  |   ',
    '  o   ',
    '      ',
    '      ',
  );
  $font['Z'] = array(
    'o---o ',
    '   /  ',
    ' -O-  ',
    ' /    ',
    'o---o ',
    '      ',
    '      ',
  );
  $font['0'] = array(
    ' o-o  ',
    'o  /o ',
    '| / | ',
    'o/  o ',
    ' o-o  ',
    '      ',
    '      ',
  );
  $font['1'] = array(
    '  0   ',
    ' /|   ',
    'o |   ',
    '  |   ',
    'o-o-o ',
    '      ',
    '      ',
  );
  $font['2'] = array(
    ' --  ',
    'o  o ',
    '  /  ',
    ' /   ',
    'o--o ',
    '     ',
    '     ',
  );
  $font['3'] = array(
    'o-o  ',
    '   | ',
    ' oo  ',
    '   | ',
    'o-o  ',
    '     ',
    '     ',
  );
  $font['4'] = array(
    'o  o ',
    '|  | ',
    'o--O ',
    '   | ',
    '   o ',
    '     ',
    '     ',
  );
  $font['5'] = array(
    'o--o ',
    '|    ',
    'o-o  ',
    '   | ',
    'o-o  ',
    '     ',
    '     ',
  );
  $font['6'] = array(
    '  o   ',
    ' /    ',
    'O--o  ',
    'o   | ',
    ' o-o  ',
    '      ',
    '      ',
  );
  $font['7'] = array(
    'o---o ',
    '   /  ',
    '  o   ',
    '  |   ',
    '  o   ',
    '      ',
    '      ',
  );
  $font['8'] = array(
    ' o-o  ',
    '|   | ',
    ' o-o  ',
    '|   | ',
    ' o-o  ',
    '      ',
    '      ',
  );
  $font['9'] = array(
    ' o-o  ',
    '|   o ',
    ' o--O ',
    '   /  ',
    '  o   ',
    '      ',
    '      ',
  );

  return $font;
}
