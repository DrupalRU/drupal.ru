/**
 * @file
 * Adjust node details block width for screen size.
 */

(function ($, Drupal) {
  "use strict";

  /**
   * Provide node details affix feature.
   */
  Drupal.behaviors.AlphaNodeAffix = {
    attach: function (context) {

      $("#node-details").width($("#node-details").parent().width());

      $("#node-details").affix({
        offset: {
          top: 250,
          bottom: function () { 
            return (this.bottom = $('footer').outerHeight(true) + 20);
          }
        }
      });

      $(window).on('resize', function(){
        $("#node-details").width($("#node-details").parent().width());
      });
    }
  };

})(jQuery, Drupal);
