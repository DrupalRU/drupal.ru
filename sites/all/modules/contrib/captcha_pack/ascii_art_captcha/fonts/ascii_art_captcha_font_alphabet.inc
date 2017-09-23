<?php
// Font definition based on figlet font "alphabet" (http://www.figlet.org/)
// as distributed by pyfiglet (http://sourceforge.net/projects/pyfiglet/)

function ASCII_art_captcha_font_alphabet() {
  $font = array();
  $font['height'] = 7;
  $font['name'] = 'alphabet';
  $font['comment'] = 'Alphabet by Wendell Hicken 11/93 (whicken@parasoft.com)Explanation of first line:flf2 - "magic number" for file identificationa    - should always be `a\', for now$    - the "hardblank" -- prints as a blank, but can\'t be smushed6    - height of a character5    - height of a character, not including descenders20   - max line length (excluding comment lines) + a fudge factor15   - default smushmode for this font (like "-m 15" on command line)12   - number of comment lines';
  $font['a'] = array(
    '    ',
    '    ',
    ' aa ',
    'a a ',
    'aaa ',
    '    ',
    '    ',
  );
  $font['b'] = array(
    'b    ',
    'b    ',
    'bbb  ',
    'b  b ',
    'bbb  ',
    '     ',
    '     ',
  );
  $font['c'] = array(
    '     ',
    '     ',
    ' ccc ',
    'c    ',
    ' ccc ',
    '     ',
    '     ',
  );
  $font['d'] = array(
    '   d ',
    '   d ',
    ' ddd ',
    'd  d ',
    ' ddd ',
    '     ',
    '     ',
  );
  $font['e'] = array(
    '    ',
    '    ',
    'eee ',
    'e e ',
    'ee  ',
    '    ',
    '    ',
  );
  $font['f'] = array(
    ' ff ',
    ' f  ',
    'fff ',
    ' f  ',
    ' f  ',
    '    ',
    '    ',
  );
  $font['g'] = array(
    '    ',
    '    ',
    'ggg ',
    'g g ',
    'ggg ',
    '  g ',
    'ggg ',
  );
  $font['h'] = array(
    'h    ',
    'h    ',
    'hhh  ',
    'h  h ',
    'h  h ',
    '     ',
    '     ',
  );
  $font['i'] = array(
    '   ',
    'ii ',
    '   ',
    'ii ',
    'ii ',
    '   ',
    '   ',
  );
  $font['j'] = array(
    '     ',
    '   j ',
    '     ',
    '   j ',
    '   j ',
    'j  j ',
    ' jj  ',
  );
  $font['k'] = array(
    'k    ',
    'k k  ',
    'kk   ',
    'k k  ',
    'k  k ',
    '     ',
    '     ',
  );
  $font['l'] = array(
    'l ',
    'l ',
    'l ',
    'l ',
    'l ',
    '  ',
    '  ',
  );
  $font['m'] = array(
    '      ',
    '      ',
    'mmmm  ',
    'm m m ',
    'm m m ',
    '      ',
    '      ',
  );
  $font['n'] = array(
    '     ',
    '     ',
    'nnn  ',
    'n  n ',
    'n  n ',
    '     ',
    '     ',
  );
  $font['o'] = array(
    '    ',
    '    ',
    'ooo ',
    'o o ',
    'ooo ',
    '    ',
    '    ',
  );
  $font['p'] = array(
    '     ',
    '     ',
    'ppp  ',
    'p  p ',
    'ppp  ',
    'p    ',
    'p    ',
  );
  $font['q'] = array(
    '      ',
    '      ',
    ' qqq  ',
    'q  q  ',
    ' qqq  ',
    '   q  ',
    '   qq ',
  );
  $font['r'] = array(
    '    ',
    '    ',
    'rrr ',
    'r   ',
    'r   ',
    '    ',
    '    ',
  );
  $font['s'] = array(
    '    ',
    '    ',
    ' ss ',
    ' s  ',
    'ss  ',
    '    ',
    '    ',
  );
  $font['t'] = array(
    ' t  ',
    ' t  ',
    'ttt ',
    ' t  ',
    ' tt ',
    '    ',
    '    ',
  );
  $font['u'] = array(
    '     ',
    '     ',
    'u  u ',
    'u  u ',
    ' uuu ',
    '     ',
    '     ',
  );
  $font['v'] = array(
    '    ',
    '    ',
    'v v ',
    'v v ',
    ' v  ',
    '    ',
    '    ',
  );
  $font['w'] = array(
    '      ',
    '      ',
    'w   w ',
    'w w w ',
    ' w w  ',
    '      ',
    '      ',
  );
  $font['x'] = array(
    '    ',
    '    ',
    'x x ',
    ' x  ',
    'x x ',
    '    ',
    '    ',
  );
  $font['y'] = array(
    '     ',
    '     ',
    'y  y ',
    'y  y ',
    ' yyy ',
    '   y ',
    'yyy  ',
  );
  $font['z'] = array(
    '    ',
    '    ',
    'zz  ',
    ' z  ',
    ' zz ',
    '    ',
    '    ',
  );
  $font['A'] = array(
    ' AA  ',
    'A  A ',
    'AAAA ',
    'A  A ',
    'A  A ',
    '     ',
    '     ',
  );
  $font['B'] = array(
    'BBBB  ',
    'B   B ',
    'BBBB  ',
    'B   B ',
    'BBBB  ',
    '      ',
    '      ',
  );
  $font['C'] = array(
    ' CCC ',
    'C    ',
    'C    ',
    'C    ',
    ' CCC ',
    '     ',
    '     ',
  );
  $font['D'] = array(
    'DDD  ',
    'D  D ',
    'D  D ',
    'D  D ',
    'DDD  ',
    '     ',
    '     ',
  );
  $font['E'] = array(
    'EEEE ',
    'E    ',
    'EEE  ',
    'E    ',
    'EEEE ',
    '     ',
    '     ',
  );
  $font['F'] = array(
    'FFFF ',
    'F    ',
    'FFF  ',
    'F    ',
    'F    ',
    '     ',
    '     ',
  );
  $font['G'] = array(
    ' GGG  ',
    'G     ',
    'G  GG ',
    'G   G ',
    ' GGG  ',
    '      ',
    '      ',
  );
  $font['H'] = array(
    'H  H ',
    'H  H ',
    'HHHH ',
    'H  H ',
    'H  H ',
    '     ',
    '     ',
  );
  $font['I'] = array(
    'III ',
    ' I  ',
    ' I  ',
    ' I  ',
    'III ',
    '    ',
    '    ',
  );
  $font['J'] = array(
    '    J ',
    '    J ',
    '    J ',
    'J   J ',
    ' JJJ  ',
    '      ',
    '      ',
  );
  $font['K'] = array(
    'K  K ',
    'K K  ',
    'KK   ',
    'K K  ',
    'K  K ',
    '     ',
    '     ',
  );
  $font['L'] = array(
    'L    ',
    'L    ',
    'L    ',
    'L    ',
    'LLLL ',
    '     ',
    '     ',
  );
  $font['M'] = array(
    'M   M ',
    'MM MM ',
    'M M M ',
    'M   M ',
    'M   M ',
    '      ',
    '      ',
  );
  $font['N'] = array(
    'N   N ',
    'NN  N ',
    'N N N ',
    'N  NN ',
    'N   N ',
    '      ',
    '      ',
  );
  $font['O'] = array(
    ' OOO  ',
    'O   O ',
    'O   O ',
    'O   O ',
    ' OOO  ',
    '      ',
    '      ',
  );
  $font['P'] = array(
    'PPPP  ',
    'P   P ',
    'PPPP  ',
    'P     ',
    'P     ',
    '      ',
    '      ',
  );
  $font['Q'] = array(
    ' QQQ   ',
    'Q   Q  ',
    'Q   Q  ',
    'Q  QQ  ',
    ' QQQQ  ',
    '     Q ',
    '       ',
  );
  $font['R'] = array(
    'RRRR  ',
    'R   R ',
    'RRRR  ',
    'R R   ',
    'R  RR ',
    '      ',
    '      ',
  );
  $font['S'] = array(
    ' SSS  ',
    'S     ',
    ' SSS  ',
    '    S ',
    'SSSS  ',
    '      ',
    '      ',
  );
  $font['T'] = array(
    'TTTTTT ',
    '  TT   ',
    '  TT   ',
    '  TT   ',
    '  TT   ',
    '       ',
    '       ',
  );
  $font['U'] = array(
    'U   U ',
    'U   U ',
    'U   U ',
    'U   U ',
    ' UUU  ',
    '      ',
    '      ',
  );
  $font['V'] = array(
    'V     V ',
    'V     V ',
    ' V   V  ',
    '  V V   ',
    '   V    ',
    '        ',
    '        ',
  );
  $font['W'] = array(
    'W     W ',
    'W     W ',
    'W  W  W ',
    ' W W W  ',
    '  W W   ',
    '        ',
    '        ',
  );
  $font['X'] = array(
    'X   X ',
    ' X X  ',
    '  X   ',
    ' X X  ',
    'X   X ',
    '      ',
    '      ',
  );
  $font['Y'] = array(
    'Y   Y ',
    ' Y Y  ',
    '  Y   ',
    '  Y   ',
    '  Y   ',
    '      ',
    '      ',
  );
  $font['Z'] = array(
    'ZZZZZ ',
    '   Z  ',
    '  Z   ',
    ' Z    ',
    'ZZZZZ ',
    '      ',
    '      ',
  );
  $font['0'] = array(
    ' 000  ',
    '0  00 ',
    '0 0 0 ',
    '00  0 ',
    ' 000  ',
    '      ',
    '      ',
  );
  $font['1'] = array(
    ' 11  ',
    '111  ',
    ' 11  ',
    ' 11  ',
    '11l1 ',
    '     ',
    '     ',
  );
  $font['2'] = array(
    ' 22  ',
    '2  2 ',
    '  2  ',
    ' 2   ',
    '2222 ',
    '     ',
    '     ',
  );
  $font['3'] = array(
    '333  ',
    '   3 ',
    ' 33  ',
    '   3 ',
    '333  ',
    '     ',
    '     ',
  );
  $font['4'] = array(
    '4  4 ',
    '4  4 ',
    '4444 ',
    '   4 ',
    '   4 ',
    '     ',
    '     ',
  );
  $font['5'] = array(
    '5555 ',
    '5    ',
    '555  ',
    '   5 ',
    '555  ',
    '     ',
    '     ',
  );
  $font['6'] = array(
    '  6   ',
    ' 6    ',
    '6666  ',
    '6   6 ',
    ' 666  ',
    '      ',
    '      ',
  );
  $font['7'] = array(
    '77777 ',
    '   7  ',
    '  7   ',
    '  7   ',
    '  7   ',
    '      ',
    '      ',
  );
  $font['8'] = array(
    ' 888  ',
    '8   8 ',
    ' 888  ',
    '8   8 ',
    ' 888  ',
    '      ',
    '      ',
  );
  $font['9'] = array(
    ' 9999 ',
    '9   9 ',
    ' 9999 ',
    '   9  ',
    '  9   ',
    '      ',
    '      ',
  );

  return $font;
}
