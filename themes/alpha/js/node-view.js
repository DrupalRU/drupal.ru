(function ($, Drupal) {
  "use strict";
  
  /**
   * Provide node details affix feature.
   */
  Drupal.behaviors.AlphaNodeAffix = {
    attach: function (context) {
      var $context = $(context);
      var $node_details = $context.find('#node-details');
      
      $("#node-details").affix({
        offset: {top: 100}
      });
      
      $("#node-details").width($("#node-details").parent().width());
    }
  };

})(jQuery, Drupal);
