-- SUMMARY --

The Extensible BBCode provides a BBCode parser that can be extended
with custom tag macros. If you install it on your Drupal site, 
it will create a text format 
named "BBCode" that can generate HTML out of user-entered text markup like this:

    This text is [b]bold[/b] and [i]italic[/i].

Activating the "Basic" submodule will add the most commonly available 
markup tags. Beyond that, it is also possible to create new tags
that either generate static output or use PHP code to determine how
the tag is rendered.

-- REQUIREMENTS --

None. However, the core PHP module must be enabled in order to create
tags that use PHP code.

-- UPGRADING FROM BBCODE MODULE --

The BBCode module (https://drupal.org/project/bbcode) provides a subset of
Extensible BBCode's functionality. To upgrade from BBCode to Exensible BBCode:

  * Enable the "xbbcode" and "xbbcode_basic" modules via Drush, or both the
    "Extensible BBCode" and "Basic Tags" modules via the admin/modules page.
  * Go through the text filters and find ones that previously used BBCode.
    * On these text filters, enable the "Extensible BBCode" filter.
    * Sort the "Extensible BBCode" filter so it is either directly before or
      directly after the "Convert BBCode to HTML" filter.
    * It may be worthwhile enabling the "Automatically close tags left open at
      the end of the text" setting.
    * Disable the "Convert BBCode to HTML" filter.
  * If Features is being used, export the text format settings again.
  * Do not disable the BBCode module until the new filters have been activated.

This will not provide a 100% functionality match, but should be close enough for
the majority of sites.
