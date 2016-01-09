(function($) {
  Drupal.behaviors.druClaim = {
    attach: function (context, settings) {
      $(document).click(function(event) {
        if ($(event.target).closest('#claim-add #claim-add-form').length) return;
          $('#claim-add').remove();
          event.stopPropagation();
      });
    }
  };
})(jQuery);