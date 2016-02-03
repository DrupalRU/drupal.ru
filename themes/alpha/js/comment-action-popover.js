/**
 * @file
 * Adjust node details block width for screen size.
 */

(function ($, Drupal) {
  "use strict";

  /**
   * Provide node details affix feature.
   */
  Drupal.behaviors.CommentPopover = {
    attach: function (context) {
      $(".comment .actions").click(function() {
        $('#comment-links-' + $(this).attr('data-source')).toggle("slide", { direction: "right" }, 200);
        $(this).toggleClass("open");
      });
    }
  };

})(jQuery, Drupal);
