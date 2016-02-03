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

      $(".comment .actions").popover({
        html: true,
        content: function() {
          return $('#comment-links-' . $(this).attr('data-id')).html();
        }
      });
      
    }
  };

})(jQuery, Drupal);
