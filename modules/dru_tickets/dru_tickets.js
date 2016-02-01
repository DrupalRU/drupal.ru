(function($) {
  Drupal.behaviors.druClaim = {
    attach: function (context, settings) {
      $(document).click(function(e) {
        if ($(e.target).closest('#verdict-add #verdict-add-form').length) return;
          $('#verdict-add').remove();
          e.stopPropagation();
      });
    }
  };
})(jQuery);
