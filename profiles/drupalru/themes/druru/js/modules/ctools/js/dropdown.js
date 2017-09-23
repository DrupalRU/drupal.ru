/**
 * @file
 * Override of Implement a simple, clickable dropdown menu
 * from CTools module.
 */
(function ($) {
  Drupal.behaviors.CToolsDropdown = {
    attach: function () {
      $('div.ctools-dropdown').once(function () {
        var
          $dropdown = $(this),
          // Performs close dropdown menu.
          close = function (target) {
            var $target = null;
            // If we should close all dropdowns, then select all.
            if ('all' == target) {
              $target = $('.ctools-dropdown');
            }
            else {
              $target = (target instanceof $ && target.length > 0) ? target : $dropdown;
            }
            $target.removeClass('opened');
          },
          // Performs toggle state of dropdown menu (show/hide).
          toggle = function () {
            // Before show or close a dropdown menu we need close all dropdowns.
            if(!$dropdown.hasClass('opened')) {
              close('all');
            }
            $dropdown.toggleClass('opened');
            return false;
          };

        // From CTools.
        $dropdown.removeClass('ctools-dropdown-no-js');
        $dropdown.on('click', "a.ctools-dropdown-link", toggle);
        // If user cliced on place out of dropdown link,
        // then we should close all dropdowns.
        $(document).click(function (event) {
          if (!$(event.target).hasClass('ctools-dropdown-link')) {
            close();
          }
        });
      });
    }
  }
})(jQuery);
