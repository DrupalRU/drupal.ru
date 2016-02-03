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
        $('#comment-links-' + $(this).attr('data-source')).animate({
          width: "toggle"
        });
        $(this).toggleClass("open");
      });
      $( "div.media-body" ).on( "swipeleft",  swipeLeftHandler);
    }
  };
  
  function swipeLeftHandler(){
    alert("test");
    $('#comment-links-' + $(this).attr('comment-id')).animate({
      width: "toggle"
    });
  }


})(jQuery, Drupal);
