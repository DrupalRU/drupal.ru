(function($) {
  Drupal.behaviors.druTickets = {
    attach: function (context, settings) {
      $(document).click(function(e) {
        if ($(e.target).closest('#verdict-add #verdict-add-form').length) return;
          closeForm(e);
      });
      
      $('#verdict-add-form #close-form').click(function(e) {
        closeForm(e);
      });
      
      function closeForm(e) {
        $('#verdict-add').fadeOut(200);
        setTimeout(function () {
          $('#verdict-add').remove();
        }, 200);
        e.stopPropagation();
      };
    }
  };
})(jQuery);
