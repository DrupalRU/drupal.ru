(function ($, Drupal) {
  "use strict";
  
  /**
   * Provide node details affix feature.
   */
  Drupal.behaviors.AlphaResponsive = {
    attach: function (context) {

      $(".field-name-body img").addClass("img-responsive").addClass("img-thumbnail");
      $(".comment .content img").addClass("img-responsive").addClass("img-thumbnail");
    }
  };

})(jQuery, Drupal);
