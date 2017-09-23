===============================================================================
CacheRouter
===============================================================================

-------------------------------------------------------------------------------
- About -
-------------------------------------------------------------------------------

CacheRouter is a caching system for Drupal allowing you to assign individual
cache tables to specific cache technology.  CacheRouter also utilizes the
page_fast_cache part of Drupal in order to reduce the amount of resources
needed for serving pages to anonymous users.


-------------------------------------------------------------------------------
- Installation -
-------------------------------------------------------------------------------

1.  Enable the module in admin/build/modules.
2.  Setup your settings.php


-------------------------------------------------------------------------------
- Configuration -
-------------------------------------------------------------------------------

CacheRouter has some pretty sane defaults, and it usually won't hurt to leave
it in the default mode.  With that said there are a few tweaks that are 
critical especially if you are running multiple sites.

Add the following lines to your settings.php:

$conf['cache_inc'] = './sites/all/modules/cacherouter/cacherouter.inc';
$conf['cacherouter'] = array(
  'default' => array(
    'engine' => 'db',
    'server' => array(),
    'shared' => TRUE,
    'prefix' => '',
  ),
);

default is for the default caching engine.  All valid cache tables or "bins"
can be added in addition, but you must have a default if you skip any bins.

For engine, the current available options are: apc, db, file, memcache and xcache.

server is only used in memcache and should be an array of host:port combinations.
(e.g. 'server' => array('localhost:11211', 'localhost:11212'))

shared is only used on memcache as well.  This allows memcache to be used with
a single process and still handle flushing correctly.

prefix is for unique site names usually when running multiple sites.


-------------------------------------------------------------------------------
- TODO -
-------------------------------------------------------------------------------

A few things I would like to have done.  

1.  Allow cache "chaining", for example, it would be nice to have default and a
"backup" cache so for any critical cache (not that there should be critical
caches), you could have memcache backed by db or file backed by db or memcache
backed by apc backed by db.  Not sure how I would like to implement this yet.

2.  I would love to see this tested on large production sites and work out the
bugs and get it into core.  I don't see why this caching shouldn't be part of
Drupal core.

3.  Any ideas for additional caching types.  

4.  I would love to get a really nice, clean web stats thing going with pretty
pictures and everything.


-------------------------------------------------------------------------------
- Maintainer -
-------------------------------------------------------------------------------

CacheRouter is maintained by Steve Rude.

Drupal.org: http://drupal.org/user/73183
Blog: http://slantview.com/
Twitter: http://twitter.com/slantview
Email: steve [at] slantview.com


-------------------------------------------------------------------------------
- Thanks -
-------------------------------------------------------------------------------

Just wanted to say thanks to whoever wrote the CakePHP caching stuff.  I 
didn't actually use any code from that, but I was somewhat inspired from
it.  So, thank you.  Go OSS.