/**
 * @file
 * When using views with ajax enabled, the use of ajaxified
 * exposed filters breaks the gmap javascript.
 * This file is part of the solution to this problem.
 */

(function ($) {
    Drupal.ajax.prototype.commands.gmapAjaxViewsFix = function (ajax, response, status) {
        var $view = $(response.target);

        if (response.settings) {
            var i = 0;
            var gmap = {};

            for (i = 0; i < response.settings.length; i++) {
                if (typeof(response.settings[i].gmap) == 'object') {
                    gmap = response.settings[i].gmap;
                }
            }

            $view.find('.gmap-map').each(function () {
                var id = '#' + $(this).attr("id");
                var t = id.split('-');
                var mapid = t[1];
                Drupal.gmap.unloadMap(mapid);
                if (gmap && gmap[mapid]) {
                    Drupal.settings.gmap[mapid] = gmap[mapid];
                }
                $(id).empty().each(Drupal.gmap.setup);
            });
        }
    };
})(jQuery);