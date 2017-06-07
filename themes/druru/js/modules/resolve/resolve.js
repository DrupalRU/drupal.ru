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
        var $pageTitle = $('h1.page-header');
        if ($pageTitle.length) {
          var $icon = $pageTitle.find('i');
          if ($icon.length) {
            $icon.replaceWith(Drupal.theme('resolved'));
          }
          else {
            $pageTitle.prepend(Drupal.theme('resolved'));
          }
        }
      }
    };
  });
  
  /**
   * Overrides for Drupal.theme.resolved of module "Resolve".
   *
   * @param resolved
   * @returns {string}
   */
  Drupal.theme.prototype.resolved = function (resolved) {
    if (typeof resolved == 'undefined' || !resolved) {
      resolved = true;
    }
    var mark = Drupal.theme('icon', 'check-circle-o', {title: Drupal.t('Resolved')});
    return resolved ? ' ' + mark + ' ' : '';
  };
  
})(jQuery);
