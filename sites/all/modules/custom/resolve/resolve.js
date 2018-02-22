(function ($) {
  "use strict";

  Drupal.resolve = Drupal.resolve || {
        titleSelector: 'h1.page-header',
        markClass: 'resolved'
      };
  /**
   * Command for make a node as unsolved.
   *
   * @param ajax
   * @param response
   * @param status
   */
  Drupal.ajax.prototype.commands.unsolved = function (ajax, response, status) {
    if (status) {
      $(Drupal.resolve.titleSelector).find('.' + Drupal.resolve.markClass).remove();
    }
  };
  /**
   * Command for resolve a node.
   *
   * @param ajax
   * @param response
   * @param status
   */
  Drupal.ajax.prototype.commands.resolve = function (ajax, response, status) {
    if (status) {
      var $pageTitle = $(Drupal.resolve.titleSelector),
          $children = $pageTitle.children();
      $pageTitle.text(Drupal.theme('resolve') + ' ' + $pageTitle.text()).prepend($children);
    }
  };

  Drupal.theme.prototype.resolve = function () {
    return '<span class="' + Drupal.resolve.markClass + '">[' + Drupal.t('Resolved') + ']</span>';
  };

  Drupal.theme.prototype.unsolved = function () {
    return '<span class="' + Drupal.resolve.markClass + '">[' + Drupal.t('Unsolved') + ']</span>';
  };
})(jQuery);
