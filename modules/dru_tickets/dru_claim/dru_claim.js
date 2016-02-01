(function($) {
  Drupal.behaviors.druClaim = {
    attach: function (context, settings) {
      $(document).click(function(e) {
        if ($(e.target).closest('#claim-add #claim-add-form').length) return;
          $('#claim-add').remove();
          e.stopPropagation();
      });
    }
  };
})(jQuery);
