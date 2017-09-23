/**
 * @file
 * Add dynamic behavior to the xbbcode_list settings pages.
 */

(function ($) {
  Drupal.behaviors.xbbcode = {
    attach: function() {
      $('input[name=ol\\[style\\]]').click(function() {
        if ($(this).attr('value') == 'hierarchy') {
          $('#xbbocde_list_sample_1').attr('class', 'numeric');
          $('#xbbocde_list_sample_2').attr('class', 'lower-alpha');
          $('#xbbocde_list_sample_3').attr('class', 'lower-roman');
        } else {
          $('#xbbocde_list_sample_1').attr('class', 'sectioned');
          $('#xbbocde_list_sample_2').attr('class', 'sectioned');
          $('#xbbocde_list_sample_3').attr('class', 'sectioned');
        }
      });
    }
  }
})(jQuery);
