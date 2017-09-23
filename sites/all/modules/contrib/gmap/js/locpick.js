/**
 * @file
 * Location chooser interface.
 */

/*global $, Drupal, google.maps */

(function ($) {
    Drupal.gmap.addHandler('gmap', function (elem) {
        var obj = this;

        var binding = obj.bind("locpickchange", function () {
            obj.locpick_invalid = !(obj.locpick_coord && obj.locpick_coord.lat && obj.locpick_coord.lng);// has a proper coord has been set since we last checked
            if (obj.locpick_invalid) {
                return; // invalid coord
            }

            if (!obj.locpick_point) {
                obj.locpick_point = new google.maps.Marker({ // should we use obj.bind('addmarker',-1,{position:obj.locpick_coord}); ?
                    position: obj.locpick_coord,
                    map: obj.map,
                    draggable: true
                });

                google.maps.event.addListener(obj.locpick_point, 'drag', function () {
                    obj.locpick_coord = obj.locpick_point.getPosition();
                    obj.change('locpickchange', binding);
                });
                google.maps.event.addListener(obj.locpick_point, 'dragend', function () {
                    obj.locpick_coord = obj.locpick_point.getPosition();
                    obj.map.panTo(obj.locpick_coord);
                    obj.change('locpickchange', binding);
                });
                obj.map.panTo(obj.locpick_coord);
                obj.change('locpickchange', binding);
            }
            else {
                obj.locpick_point.setPosition(obj.locpick_coord);
            }
        });

        obj.bind("locpickremove", function () {
            if (obj.locpick_point) obj.locpick_point.setMap(null);
            obj.locpick_point = null;
            obj.locpick_coord = null;
            obj.change('locpickchange', -1);
        });

        obj.bind("init", function () {
            if (obj.vars.behavior.locpick) {
                obj.locpick_coord = new google.maps.LatLng(obj.vars.latitude, obj.vars.longitude);

                google.maps.event.addListener(obj.map, "click", function (event) {
                    google.maps.event.trigger(obj.map, "resize");
                    if (event) {
                        obj.locpick_coord = event.latLng;
                        obj.change('locpickchange');
                    }
                    else {
                        // Unsetting the location
                        obj.change('locpickremove');
                    }
                });
            }
        });

        obj.bind("ready", function () {
            // Fake a click to set the initial point, if one was set.
            if (obj.vars.behavior.locpick) {
                if (!obj.locpick_invalid) {
                    obj.locpick_coord = new google.maps.LatLng(obj.vars.latitude, obj.vars.longitude);
                    obj.change('locpickchange');
                }
            }
        });

    });

    Drupal.gmap.addHandler('locpick_latitude', function (elem) {
        var obj = this;

        obj.bind("init", function () {
            if (elem.value !== '') {
                obj.vars.latitude = Number(elem.value);
                obj.locpick_coord = new google.maps.LatLng(obj.vars.latitude, obj.vars.longitude);
            }
            else {
                obj.locpick_coord = null;
                obj.locpick_invalid = true;
            }
        });

        var binding = obj.bind("locpickchange", function () {
            if (obj.locpick_coord) {
                elem.value = obj.locpick_coord.lat();
            }
            else {
                elem.value = '';
            }
        });

        $(elem).change(function () {
            if (elem.value !== '') {
                if (obj.locpick_coord) {
                    obj.locpick_coord = new google.maps.LatLng(Number(elem.value), obj.locpick_coord.lng());
                    obj.change('locpickchange', binding);
                }
                else {
                    obj.locpick_coord = new google.maps.LatLng(Number(elem.value), 0.0);
                }
            }
            else {
                obj.change('locpickremove', -1);
            }
        });
    });

    Drupal.gmap.addHandler('locpick_longitude', function (elem) {
        var obj = this;

        obj.bind("init", function () {
            if (elem.value !== '') {
                obj.vars.longitude = Number(elem.value);
                //obj.locpick_coord = new GLatLng(obj.vars.latitude, obj.vars.longitude);
                obj.locpick_coord = new google.maps.LatLng(obj.vars.latitude, obj.vars.longitude);
            }
            else {
                obj.locpick_invalid = true;
            }
        });

        var binding = obj.bind("locpickchange", function () {
            if (obj.locpick_coord) {
                elem.value = obj.locpick_coord.lng();
            }
            else {
                elem.value = '';
            }
        });

        $(elem).change(function () {
            if (elem.value !== '') {
                if (obj.locpick_coord) {
                    obj.locpick_coord = new google.maps.LatLng(obj.locpick_coord.lat(), Number(elem.value));
                    obj.change('locpickchange', binding);
                }
                else {
                    obj.locpick_coord = new google.maps.LatLng(0.0, Number(elem.value));
                }
            }
            else {
                obj.change('locpickremove');
            }
        });
    });
})(jQuery);
