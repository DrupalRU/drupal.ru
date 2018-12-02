(function ($) {

  'use strict';

  Drupal.behaviors.snow = {
    attach: function (context, settings) {

      snowStorm.snowColor = Drupal.settings.happy_new_year.snowColor;

    }
  };

})(jQuery);
