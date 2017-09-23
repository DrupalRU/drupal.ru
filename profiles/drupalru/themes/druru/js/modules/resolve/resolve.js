(function ($) {
  "use strict";

  // Hide these in a ready to ensure that Drupal.ajax is set up first.
  $(function () {
    /**
     * Command for resolve a node.
     *
     * @param ajax
     * @param response
     * @param status
     */
    Drupal.ajax.prototype.commands.resolve = function (ajax, response, status) {
      if (status) {
        var $pageTitle = $(Drupal.resolve.titleSelector);
        $pageTitle.html(Drupal.theme('resolve') + $pageTitle.text());
      }
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
        var $pageTitle = $(Drupal.resolve.titleSelector);
        $pageTitle.html(Drupal.theme('unsolved', false) + $pageTitle.text());
      }
    };

    /**
     * Overrides for Drupal.theme.resolved of module "Resolve".
     *
     * @returns {string}
     */
    Drupal.theme.prototype.resolve = function () {
      return ' ' + Drupal.theme('icon', 'check-circle-o', {title: Drupal.t('Resolved')}) + ' ';
    };

    /**
     * Overrides for Drupal.theme.resolved of module "Resolve".
     *
     * @returns {string}
     */
    Drupal.theme.prototype.unsolved = function () {
      return ' ' + Drupal.theme('icon', 'question-circle-o', {title: Drupal.t('Unsolved')}) + ' ';
    };

  });
})(jQuery);
