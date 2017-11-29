/**
 * @file
 *
 */
(function ($) {
  "use strict";

  Drupal.behaviors.acal = {
    attach: function (context, settings) {
      if (typeof Drupal.settings.acal !== 'undefined') {
        $('.acal-holder').each(function (idx, obj) {
          var uid = $(obj).data('uid');
          if (typeof settings.acal.users['user' + uid] !== 'undefined') {
            new Calendar({
              container: 'acal-' + uid,
              num_weeks: 52,
              day_size: 10,
              data: settings.acal.users['user' + uid],
              color: settings.acal.color,
              intervals: settings.acal.intervals
            });
          }
        });
      }
    }
  };
})(jQuery);
