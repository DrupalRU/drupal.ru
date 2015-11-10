Quote.module
------------

This module adds a 'quote' link below nodes and comments. When clicked, the 
contents of the node or comment are placed into a new comment enclosed with 
special markup that indicates that it is quoted material. 

This module subsequently uses a filter to translate the special markup into HTML
code. When output by Drupal, the quote will be displayed with special formatting
to indicate the material which has been quoted.


Installation
------------

The module should be installed within an appropriate module directory such as
'sites/all/modules/quote'. Once installed, it can be enabled via the module
administration interface at 'admin/modules'.

Filter
------

The Quote filter should be activated for each text format that it needs to be a 
part of. Text formats can be configured via 'admin/config/content/formats'.

For best effect, the Quote filter must be applied *after* any filters that 
replace HTML, and *before* the Line-break filter. Or conversely, if
HTML filters consider <blockquote> and <div> tags to be valid, the quote filter
can be placed before them. Filters can be rearranged by using the weight
selectors within the 'Filter processing order' section.

Additionally, the Quote filter must be applied *before* the BBCode filter if you
have the optional BBcode module installed.

An example of a filter processing order that works well is as follows:

  * HTML Filter
  * URL Filter
  * Quote Filter
  * Line-break filter
  * HTML Corrector filter

As the quote filter accesses the node (being quoted) directly, any content 
within will be displayed without any processing. For example, if a user is 
quoting a page node containing PHP (which by default is evaluated by the PHP 
filter) or any other sensitive code, it is quoted as is without the PHP (or any
other) filter being applied. While the PHP code is never evaluated in a comment,
it is nevertheless possible that sensitive server side information is made 
available to the public. To avoid this situation, quote links can be 
enabled/disabled for the parent node via the settings page. This does not affect
comment quote links.

Settings
--------

The Quote module can be configured further through its settings page at
'admin/config/content/quote'. This page allows the filter to be associated with
specific node types, control if the quote link is displayed for nodes (as 
opposed to comments), and so on.

Format
------

Quoted content can be placed between [quote] tags in order to be displayed as a
quote:

[quote]This is a simple quote.[/quote]

There is an optional attribute which allows quotes to cite the author:

[quote=author's name]This is a quote with an attribution line.[/quote]


Theme
-----

There are two css rules located in "quote.css" which can be overridden to change
the display of the quotes.

'quote-msg' controls the display of the quote content.
'quote-author' controls the display of the attribution line.

The default "quote.css" rules are designed for Drupal's default Bartik theme.
By default, quoted content is placed into an indented box, which has a 
light gray background.

As mentioned above, CSS rules are meant to be overridden. In other words, there
should never be a need to directly edit the CSS (or for that matter, any other)
file in the module's directory.

If the markup used to display the quote needs to be changed, this can be done by
overriding the theme function theme_quote() from within a theme.

Project navigation
------------------

Settings: Administration -> Configuration -> Content authoring -> Quote
Filters : Administration -> Configuration -> Content authoring -> Text formats

Credits
-------
Maintainer: Kálmán Hosszu [http://drupal.org/user/267481]
Maintainer: Zen / |gatsby| / Karthik Kumar [http://drupal.org/user/21209]

Project URL: http://drupal.org/project/quote
