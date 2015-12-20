(function ($) {
  "use strict";

  /**
   * Command for resolve a node.
   *
   * @param ajax
   * @param response
   * @param status
   */
  Drupal.ajax.prototype.commands.dru_resolve = function (ajax, response, status) {
    if (status) {
      var $pageTitle = $('h1.page-header');
      if ($pageTitle.length > 0) {
        if ($pageTitle.children().length > 0) {
          var $children = $pageTitle.children(),
            $lastTag = $($children[$children.length - 1])
            ;
          $lastTag.after('[' + Drupal.t('Resolved') + '] ');
        }
        else{
          $pageTitle.prepend('[' + Drupal.t('Resolved') + '] ');
        }
        console.log(ajax);
        $(ajax.element).closest('.comment').addClass('best-comment');
        $('.comment-resolve').remove();
      }
    }
  };
})(jQuery);