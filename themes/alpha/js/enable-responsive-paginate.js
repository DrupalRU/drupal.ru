/**
 * @file
 * Make node images responsive.
 */

(function ($, Drupal) {
  "use strict";

  /**
   * Provide node details affix feature.
   */
  Drupal.behaviors.PaginationResponsive = {
    attach: function (context) {

      $(".pagination").rPage();
    }
  };

})(jQuery, Drupal);
