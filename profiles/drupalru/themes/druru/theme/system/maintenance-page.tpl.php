<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see     template_preprocess()
 * @see     template_preprocess_maintenance_page()
 *
 * @ingroup themeable
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml"
      xml:lang="<?php print $language->language ?>"
      lang="<?php print $language->language ?>"
      dir="<?php print $language->dir ?>">

  <head>
    <title><?php print $head_title; ?></title>
    <?php print $head; ?>
    <?php print $styles; ?>
    <style>
      .navbar .navbar-brand {
        background-image: url("/<?php echo $directory; ?>/logo.svg");
        background-repeat: no-repeat;
      }
    </style>
  </head>
  <body class="<?php print $classes; ?>">

    <div id="wrapper">
      <header id="navbar" class="navbar navbar-static-top navbar-default">
        <div class="container">
          <div class="navbar-header">
            <a href="/" class="logo navbar-btn pull-left">
              <div class="name navbar-brand">
                <div class="site-name"><?php print $site_name; ?></div>
                <?php if (!empty($site_slogan)): ?>
                  <div class="site-slogan"><?php print $site_slogan; ?></div>
                <?php endif; ?>
              </div>
            </a>
          </div>
        </div>
      </header>

      <div class="main-container container">
        <?php if (!empty($title)): ?>
          <h1 class="page-header"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php if (!empty($messages)): print $messages; endif; ?>
        <?php print $content; ?>
      </div>
    </div>

    <footer class="footer">
      <div class="container">
        <p class="text-right">
          <?php print $site_name; ?>,
          <?php if (!empty($site_slogan)): ?>
            <?php print $site_slogan; ?>
          <?php endif; ?>
          ,
          <?php print date('Y'); ?></p>
      </div>
    </footer>

  </body>
</html>
