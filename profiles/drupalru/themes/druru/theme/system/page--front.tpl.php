<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the left sidebar.
 * - $page['sidebar_second']: Items for the right sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 * - $page['site_first']: Items for the top site region.
 * - $page['site_last']: Items for the bottom site region.
 * - $page['page_first']: Items for the top page region.
 * - $page['page_last']: Items for the bottom page region.
 * - $page['content_first']: Items for the top content region.
 * - $page['content_second']: Items for the middle content region.
 * - $page['content_last']: Items for the bottom content region.
 *
 * @see druru_preprocess_page()
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see druru_process_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>
<?php if ($page['site_first']): ?>
  <?php print render($page['site_first']); ?>
<?php endif; ?>
<div id="wrapper">
  <header id="navbar" role="banner" class="<?php print $navbar_classes; ?>">
    <div class="container">
      <div class="navbar-header">
        <?php if ($logo): print $logo; endif; ?>
        <!-- .btn-navbar is used as the toggle for collapsed navbar content -->
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
          <span class="sr-only">Toggle navigation</span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <?php if (_druru_count_unread_messages()):?>
            <i class="fa fa-circle small whistle-blower"></i>
          <?php endif; ?>
        </button>
      </div>

      <?php if ($primary_nav || $secondary_nav || $page['between_menus']): ?>
        <div class="navbar-collapse collapse">
          <nav role="navigation">
            <?php
            // We need use this method for exclude unnecessary spaces between regions.
            if ($primary_nav) {
              print trim(render($primary_nav));
            }
            if ($page['between_menus']) {
              print trim(render($page['between_menus']));
            }
            if ($secondary_nav) {
              print trim(render($secondary_nav));
            }
            ?>
          </nav>
        </div>
      <?php endif; ?>
    </div>

    <?php if ($page['navigation']): ?>
      <div class="navbar-second navbar-collapse collapse">
        <div class="container">
          <?php print trim(render($page['navigation'])); ?>
        </div>
      </div>
    <?php endif; ?>
  </header>

  <div class="main-container">
    <?php if ($page['header']): ?>
      <header role="banner" id="page-header" class="jumbotron">
        <div class="container">
          <?php print render($page['header']); ?>
        </div>
      </header> <!-- /#page-header -->
    <?php endif; ?>

    <?php if ($page['page_first']): ?>
      <?php print render($page['page_first']); ?>
    <?php endif; ?>

    <div class="container">
      <?php if ($page['content_first']): ?>
        <?php print render($page['content_first']); ?>
      <?php endif; ?>

      <a id="main-content"></a>
      <div class="main-content">
        <section<?php print drupal_attributes($content_column_attributes); ?>>
          <?php print render($title_prefix); ?>
          <?php if ($title || is_numeric($title)): ?>
            <h1 class="page-header"><?php print $title; ?></h1>
          <?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php print $messages; ?>
          <?php if ($tabs): ?>
            <?php print render($tabs); ?>
          <?php endif; ?>
          <?php if ($page['help']): ?>
            <?php print render($page['help']); ?>
          <?php endif; ?>
          <?php if ($action_links): ?>
            <ul class="action-links"><?php print render($action_links); ?></ul>
          <?php endif; ?>
          <?php print render($page['frontpage']); ?>
        </section>

        <?php if ($page['sidebar_first']): ?>
          <aside<?php print drupal_attributes($sidebar_first_attributes); ?>>
            <?php print render($page['sidebar_first']); ?>
          </aside>  <!-- /#sidebar-first -->
        <?php endif; ?>

        <?php if ($page['sidebar_second']): ?>
          <aside <?php print drupal_attributes($sidebar_second_attributes); ?>>
            <?php print render($page['sidebar_second']); ?>
          </aside>  <!-- /#sidebar-second -->
        <?php endif; ?>
      </div>
    </div>
  </div>
</div>

<div class="footer-wrapper">
  <div class="container">
    <?php if ($page['content_last']): ?>
      <?php print render($page['content_last']); ?>
    <?php endif; ?>
  </div>

  <?php if ($page['page_last']): ?>
    <?php print render($page['page_last']); ?>
  <?php endif; ?>

  <div class="footer">
    <div class="container">
      <?php print render($page['footer']); ?>
    </div>
  </div>

  <?php if ($page['site_last']): ?>
    <?php print render($page['site_last']); ?>
  <?php endif; ?>
</div>
