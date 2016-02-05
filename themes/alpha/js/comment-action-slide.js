/**
 * @file
 * Adjust node details block width for screen size.
 */

(function ($, Drupal) {
  "use strict";

  /**
   * Provide node details affix feature.
   */
  Drupal.behaviors.CommentActionSlide = {
    attach: function (context) {
      $(".comment .actions").once().click(function() {
        $('#comment-links-' + $(this).attr('data-source')).animate({
          width: "toggle"
        }, 200);
        $(this).toggleClass("open");
        
        if($( "div[comment-id='" + $(this).attr('data-source') + "']" ).hasClass("swipedleft")) {
          $( "div[comment-id='" + $(this).attr('data-source') + "']" ).removeClass("swipedleft");
        }else{
          $( "div[comment-id='" + $(this).attr('data-source') + "']" ).addClass("swipedleft");
        }
      });
      
      $( "div.media-body" ).on( "swipeleft",  function() {
        if(!$(this).hasClass("swipedleft")) {
          $('#comment-links-' + $(this).attr('comment-id')).animate({
            width: "toggle"
          }, 200);
          $( "div[data-source='" + $(this).attr('comment-id') + "']" ).toggleClass("open");
          $(this).addClass("swipedleft");
        }
      });
      
      $( "div.media-body" ).on( "swiperight",  function() {
        if($(this).hasClass("swipedleft")) {
          $('#comment-links-' + $(this).attr('comment-id')).animate({
            width: "toggle"
          }, 200);
          $( "div[data-source='" + $(this).attr('comment-id') + "']" ).toggleClass("open");
          $(this).removeClass("swipedleft");
        }
      });

    }
  };


})(jQuery, Drupal);
