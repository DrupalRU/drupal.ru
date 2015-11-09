(function ($, Drupal) {
  "use strict";
  
  /**
   * Provide node details affix feature.
   */
  Drupal.behaviors.AlphaResponsive = {
    attach: function (context) {

      $(".field-name-body img").addClass("img-responsive");
      $(".comment .content img").addClass("img-responsive");
    }
  };

})(jQuery, Drupal);
