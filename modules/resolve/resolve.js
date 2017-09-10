(function ($) {
  "use strict";

  /**
   * Command for resolve a node.
   *
   * @param ajax
   * @param response
   * @param status
   */
  Drupal.ajax.prototype.commands.resolve = function (ajax, response, status) {
    if (status) {
      var $pageTitle = $('h1.page-header'), $children = $pageTitle.children();
      $pageTitle.text('[' + Drupal.t('Resolved') + '] '+ $pageTitle.text()).prepend($children);
    }
  };

  Drupal.theme.prototype.resolved = function (resolved) {
    if (typeof resolved == 'undefined' || !resolved) {
      resolved = true;
    }
    return resolved ? '[' + Drupal.t('Resolved') + ']' : '';
  };
})(jQuery);
