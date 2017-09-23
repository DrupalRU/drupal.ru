
============================
GeSHi Filter (Drupal Module)
============================


DESCRIPTION
-----------
The GeShi Filter is a Drupal module for syntax highlighting of pieces of
source code. It implements a filter that formats and highlights the syntax of
source code between for example <code>...</code>.


DEPENDENCY
----------
This module requires the third-party library GeShi 1.0.x (Generic Syntax
Highlighter, written by Nigel McNie) which can be found at
  http://qbnz.com/highlighter
See installation procedure below for more information.


INSTALLATION
------------
1. Extract the GeSHi Filter module tarball and place the entire geshifilter
  directory into your Drupal setup (e.g. in sites/all/modules).

2. Download the GeSHi library from
  http://sourceforge.net/projects/geshi/files/geshi
  Make sure you download a version of the branch 1.0.x and not a version
  from the branch 1.1.x (also described as geshi-dev), which is not yet
  supported by the GeSHi filter module.
  Place the entire extracted 'geshi' folder (which contains geshi.php)
  in a libraries directory (e.g. as sites/all/libraries/geshi).

3. Enable this module as any other Drupal module by navigating to
  administer > site building > modules


CONFIGURATION
-------------
1. The general GeSHi Filter settings can be found by navigating to:
  administer > site configuration > geshifilter.
  Set the path to the GeSHi library on that page, if it is not detected
  automatically already.
2. Further configuration instructions can be found by following the
  "more help..." link at the top of that general settings page, which leads
  to www.example.com/?q=admin/help/geshifilter . This requires you have the
  'help' module enabled.


USAGE
-----
The basic usage (with the default settings) is:
  <code language="java">
  for (int i; i<10; ++i) {
    dothisdothat(i);
  }
  </code>
When language tags are enabled (like "<java>" for Java) you can also do
  <java>
  for (int i; i<10; ++i) {
    dothisdothat(i);
  }
  </java>
More options and tricks can be found in the filter tips of the text format at
www.example.com/?q=filter/tips .


AUTHORS
-------
Original module by:
  Vincent Filby <vfilby at gmail dot com>

Drupal.org hosted version for Drupal 4.7:
  Vincent Filby <vfilby at gmail dot com>
  Michael Hutchinson (http://compsoc.dur.ac.uk/~mjh/contact)
  Damien Pitard <dpdev00 at gmail dot com>

Port to Drupal 5:
  rötzi (http://drupal.org/user/73064)
  Stefaan Lippens (http://drupal.org/user/41478)
