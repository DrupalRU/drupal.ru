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
        offset: {top: 250 }
      });

      $(window).on('resize', function(){
        $("#node-details").width($("#node-details").parent().width());
      });
    }
  };

})(jQuery, Drupal);
