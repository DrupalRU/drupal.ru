(function ($) {

  'use strict';

  Drupal.behaviors.garland = {
    attach: function (context, settings) {

      var num = 0;
      function garland() {
        $('#garland').css('backgroundPosition', '0 -' + num + 'px');
        if (num > 72) {
          num = 36;
        }
        else {
          num = num + 36;
        }
      }

      $('body').prepend('<div id="garland"></div>');

      // if core toolbar exists
      if ($('body').hasClass('toolbar')) {
        var toolbarHeight = $('#toolbar').height();
        $('#garland').css('top', toolbarHeight + 'px');
      }

      // if bootstrap navbar fixed exists
      if ($('header').is('.navbar-fixed-top')) {
        var navbarHeight = $('.navbar-fixed-top').height();
        var navbarTop = $('.navbar-fixed-top').position().top;
        $('#garland').css('top', navbarTop + navbarHeight + 'px');
      }

      // if admin_menu exists
      if ($('body').hasClass('admin-menu')) {
        setTimeout(function () {
          var adminMenuHeight = $('#admin-menu').height();
          $('#garland').css('top', adminMenuHeight + 'px');
          $('#garland').css('zIndex', '998');
        }, 500);
      }

      setInterval(function () {
        garland();
      }, 500);

    }
  };

})(jQuery);
