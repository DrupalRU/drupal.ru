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
        if ($pageTitle.length > 0) {
          if ($pageTitle.children().length > 0) {
            var $children = $pageTitle.children(),
              $lastTag = $($children[$children.length - 1]);
            $lastTag.after(Drupal.theme('resolved'));
          }
          else {
            $pageTitle.prepend(Drupal.theme('resolved'));
          }
        }
      }
    };
  });
  
  Drupal.theme.prototype.resolved = function (resolved) {
    if (typeof resolved == 'undefined' || !resolved) {
      resolved = true;
    }
    return resolved ? '[' + Drupal.t('Resolved') + ']' : '';
  };
})(jQuery);
