#Bootstrap-lite - Backdrop bootstrap based theme 
It's clean and minimal backdrop oriented bootstrap based theme inspired by [Drupal Bootstrap theme](https://www.drupal.org/project/bootstrap). 
Bootstrap-lite is separated project from [Drupal Bootstrap theme](https://www.drupal.org/project/bootstrap) with no guarantied drupal bootstrap theme compatibility.

Please try [Demo site](http://bootstrap.backdrop.expert).

##Features
1. [BootstrapCDN](http://bootstrapcdn.com/) based.
2. [Bootswatch](http://bootswatch.com) support (via BootstrapCDN) included. Easy to pick a Bootswatch free theme.
3. [Font awesome](https://fortawesome.github.io/Font-Awesome/) support included.
4. Other tweaks:
  - Navbar settings (fixed, static, top, bottom). 
  - Navbar user menu with cog icon.
  - Breadcrumbs tweaks.
  - Ability to use fluid or fixed width.
  - "XX time ago" for nodes and comments instead of regular time.
  
## Excluded from original drupal bootstrap code
  - Starter kit. But you still can create sub theme. See [Developing themes](https://api.backdropcms.org/developing-themes)
  - Tooltip. Feature is here, but you need follow [documentation](http://getbootstrap.com/javascript/#tooltips) to make it work.
  - Popovers.  Feature is here, but you need follow [documentation](http://getbootstrap.com/javascript/#popovers) to make it work.
  - Anchors settings. I believe this one need to be done via module.
  - Well settings.

##Installation
  - Install this theme using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/themes

##HOWTO
  - navbar content controlled by layout block - "Header block". By changing settings for "Header block" you can control menu, logo, sitename and site slogan visibility.

##Roadmap
  1.x-1.3.5.3
    - Tooltops, Popovers implementation in a best way to integrate with backdrop.
    - Add ability to have different container settings for different layout. For example: fluid for Default Layout and fixed for Default Administrative Layout.

##License
This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

##Thanks to drupal module Authors
http://drupal.org/node/259843/committers

## Drupal bootstrap
[Drupal Bootstrap theme](https://www.drupal.org/project/bootstrap)
If you interested in drupal bootstrap theme backdrop port, please see: [Backdrop port issue](https://www.drupal.org/node/2483391)

