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
          alert($('#comment-links-' . $(this).attr('rel')).html());
          return $('#comment-links-' . $(this).attr('rel')).html();
        }
      });
      
    }
  };

})(jQuery, Drupal);
