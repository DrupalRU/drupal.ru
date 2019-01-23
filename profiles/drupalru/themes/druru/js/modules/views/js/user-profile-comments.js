(function ($) {

  'use strict';

  Drupal.behaviors.userProfileComments = {
    attach: function (context, settings) {

      var limit = 600;
      var i = 1;
      var expandText = '... развернуть';
      var collapsetext = 'свернуть';

      $('.views-field-title span').each(function () {
        var allstr = $(this).text();

        if (allstr.length > limit) {
          var firstSet = allstr.substring(0, limit);
          var secdHalf = allstr.substring(limit, allstr.length);
          var strtoadd = firstSet + "<div id='comment-" + i + "' class='collapse'>"
              + secdHalf + "</div> <a class='user-comments-collapse-btn' href=# data-toggle='collapse' data-target='#comment-" + i + "'>" + expandText + "</a>";
          $(this).html(strtoadd);
        }

        i++;

      });

      $('a').click(function () {
        if ($(this).text() === expandText) {
          $(this).text(collapsetext);
        }
        else
        if ($(this).text() === collapsetext) {
          $(this).text(expandText);
        }

      });

      $('#user-comments-expand-all').click(function () {
        $('.collapse').collapse('show');
        $('.user-comments-collapse-btn').text(collapsetext);
      });

      $('#user-comments-collapse-all').click(function () {
        $('.collapse').collapse('hide');
        $('.user-comments-collapse-btn').text(expandText);
      });

    }
  };

})(jQuery);
