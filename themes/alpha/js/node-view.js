(function ($, Drupal) {
  "use strict";
  
  /**
   * Provide node details affix feature.
   */
  Drupal.behaviors.AlphaNodeAffix = {
    attach: function (context) {

      $("#node-details").width($("#node-details").parent().width());
      
      $("#node-details").affix({
        offset: {top: 200 }
      });
      
    }
  };

})(jQuery, Drupal);
