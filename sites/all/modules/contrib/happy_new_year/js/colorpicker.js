(function ($) {

  'use strict';

  Drupal.behaviors.colorPicker = {
    attach: function (context, settings) {

      $(document).ready(function () {
        $('#color-picker').farbtastic('#edit-happy-new-year-snowcolor');
      });

    }
  };

})(jQuery);
