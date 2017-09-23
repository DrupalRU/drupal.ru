GMap Module
===========

GMap is an API and a related set of modules which allows the integration of
Google Maps with a Drupal site.

gmap.module: The main module. Contains the API, the basic map functions, Views
integration and an text filter to create macros into working maps with minimal
effort.

gmap_location.module: Provides map features for Location.module (v2 and v3).

gmap_macro_builder.module: End-user UI for easily creating GMap macros.

gmap_taxonomy.module: API and utility for changing map markers of for points
from Location.module based on taxonomy terms.

Installation
------------

To install, follow the general directions available at:
https://drupal.org/documentation/install/modules-themes

In order to use this module, you need a (free) Google Maps API key. Here is how
to get one:

1. Login using a Google account to https://code.google.com/apis/console
2. Create project
3. Go to 'Services' and turn on 'Google Maps API v2' and 'Google Maps API v3'.
4. Go to 'API Access' and copy the API Key from the "Simple API Access" section.
5. Paste the API Key into the GMap settings page at admin/config/services/gmap.

You may need to make changes to your theme so that Google Maps can display
correctly. See the section on "Google Maps and Quirks Mode" below.

If you would like to use GMap macros directly in nodes, you will need to add
the GMap Macro filter to an text format (or create a new text format that
includes it). Read http://drupal.org/node/213156 for more information on text
formats.

If you are using the HTML filter, it will need to appear BEFORE the GMap filter;
otherwise the HTML filter will remove the GMap code. To modify the order of
filters in an text format, go to the "Rearrange" tab on the text format's
configuration page (Administer > Configuration > Content authoring > Text
formats, then click "configure" by your format).

If you would like to use third party functionality such as custom marker
libraries read thirdparty/README.txt for download links and instructions.

If you are translating this module to a different language also see the
gmap.strings.php file for further installation instructions.  This is required
for translation of marker type strings to work.


Google Maps and Quirks Mode
---------------------------

Google Maps may have rendering issues when a doctype is not declared on your
site. Specifically, Google Maps will have problems when the browser is using
"quirks mode". To render pages in "standards compliant mode", use either an
HTML5 doctype or XHTML Strict doctype as the first line of your HTML:

HTML5 doctype:

<!DOCTYPE html>

XHTML Strict doctype:

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">


Views Integration
-----------------
The GMap module includes built-in integration with the Views module. You can use
this to turn any list of nodes, users, or other content into a Google Map that
plots each item on a map.

To make a GMap-based view, create or edit a view under Administer > Structure >
Views. Under the "Format" section, change the format by clicking on the current
format type (usually "Unformatted list"). Then change the format type to either
"GMap" or "Extended GMap".

For each view you create that uses a GMap format, you may either use a custom
configuration or inherit the site-wide defaults. To make an advanced map
configuration, you may type or copy/paste a GMap macro into the "Macro" textarea
at the top of the GMap format configuration settings. See the section on
"Macros" for more information about how to customize the configuration of your
map using these powerful tools. If you don't want a custom configuration for
this view, just leave the default value for the Macro field.

The only other critically important setting when configuring a GMap format in
Views is the "Data Source" field. GMap can accept data from several different
sources, but ultimately it comes down to acquiring a latitude and longitude pair
for each item being displayed in the list. GMap module can acquire the lat/long
pair from any of the following sources:

- Location.module
- Geofield.module
- Geolocation.module
- or a pair of simple text or number fields

If you use the pair of text fields, you must also change your view to use a list
of fields. To do this, under the "Format" section of the view configuration,
click on the current row display style (usually the word "Content") and change
it to "Fields". Then under the "Fields" section of the view configuration, add
the fields you're using to store the latitude and longitude (they must be two
separate fields). It may be a good idea to use the "Exclude from display" option
on each of these fields, so they aren't shown in the actual GMap marker popups.


Macros
------
"GMap macros" are text-based representations of Google Maps. A GMap macro can be
created with the GMap Macro Builder tool or by hand. Any map parameter not
specified by a macro will use the defaults from the GMap settings page
(Administer > Site configuration > Web services > GMap). There are several
places where you may use Gmap macros, including:

1) GMap settings, like the GMap Location module's settings page.
2) Any node where the GMap filter is enabled (as part of the text format).

A GMap macro will look something like this (see the advanced help pages (or the
files in help/ in the module folder) for syntax details):
[gmap markers=blue::41.902277040963696,-87.6708984375 |zoom=5 |center=42.94033923363183,-88.857421875 |width=300px |height=200px |control=Small |type=Map]

The GMap Macro Builder is a GUI for creating GMap macros; you may use it to
create a map with points, lines, polygons, and circles, and then copy and paste
the resulting macro text. After you insert the macro into a node, you can edit
it using raw values that you get from elsewhere to create a different set of
points or lines on the map.

If you've enabled the gmap_macro_builder.module, you can access the GMap macro
builder at the 'map/macro' path (there will be "Build a GMap macro" link in your
Navigation menu).

Note that there are many more options you can set on your maps if you are
willing to edit macros by hand. For example, you may add KML or GeoRSS overlays
to GMaps, but this option isn't available through the macro builder. Again, see
the advanced help pages for syntax details.


User and node maps
------------------

User and node location maps are made available by gmap_location.module, and work
in conjunction with location.module. Any user that has the 'view user map' or
'view node map' permission can see the user or node maps, respectively. These
are maps with a marker for every user or node, and are located at the 'map/user'
and 'map/node' paths (links to these maps are placed in the Navigation menu).

Users with the 'view user location details' permission can click markers on the
User map to see information on individual users.

GMap Location also provides two map blocks that work with node locations: a
"Location map" block that displays the location markers associated with the node
being viewed, and an "Author map" block that displays a marker representing the
node author's location.

GMap Location provides an interactive Google Map to the Location module for
setting the latitude and longitude; users must have Location's
"submit latitude/longitude" to use this feature.

Markers
-------

The 'markers' directory contains many useful markers, and you can add custom
markers by placing PNG files (markers must be in PNG format) in the markers
directory and creating a ".ini" file for them. Use the existing .ini files as a
guide--start with "markers/colors.ini".

Support
-------
For feature requests and bug reports, please file an issue in the GMap module
issue tracker at http://drupal.org/project/issues/gmap
