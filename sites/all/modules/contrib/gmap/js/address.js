/**
 * @file
 * Address widget and GMap geocoder routines.
 */

/*global jQuery, Drupal, GClientGeocoder */

/**
 * Provide a shared geocoder.
 * Lazy initialize it so it's not resident until needed.
 */
Drupal.gmap.geocoder = function () {
    var theGeocoder;
    if (!theGeocoder) {
        theGeocoder = new google.maps.Geocoder();
    }
    return theGeocoder;
};

Drupal.gmap.addHandler('gmap', function (elem) {
    var obj = this;

    obj.bind('geocode_pan', function (addr) {
        Drupal.gmap.geocoder().geocode({'address': addr}, function (results, status) {
            if (status == google.maps.GeocoderStatus.OK) {
                obj.vars.latitude = results[0].geometry.location.lat();
                obj.vars.longitude = results[0].geometry.location.lng();
                obj.change("move", -1);
            }
            else {
                // Error condition?
            }
        });
    });

    obj.bind('geocode_panzoom', function (addr) {
        Drupal.gmap.geocoder().geocode({'address': addr}, function (results, status) {
            if (status == google.maps.GeocoderStatus.OK) {
                var place = results[0];
                obj.vars.latitude = results[0].geometry.location.lat();
                obj.vars.longitude = results[0].geometry.location.lng();

                // This is, of course, temporary.

                switch (place.AddressDetails.Accuracy) {
                    case 1: // Country level
                        obj.vars.zoom = 4;
                        break;
                    case 2: // Region (state, province, prefecture, etc.) level
                        obj.vars.zoom = 6;
                        break;
                    case 3: // Sub-region (county, municipality, etc.) level
                        obj.vars.zoom = 8;
                        break;
                    case 4: // Town (city, village) level accuracy. (Since 2.59)
                    case 5: // Post code (zip code) level accuracy. (Since 2.59)
                    case 6: // Street level accuracy. (Since 2.59)
                    case 7: // Intersection level accuracy. (Since 2.59)
                    case 8: // Address level accuracy. (Since 2.59)
                        obj.vars.zoom = 12;
                }
                obj.change('move', -1);
            }
        });
    });

    obj.bind('preparemarker', function (marker) {
        if (marker.address && (!marker.latitude || !marker.longitude)) {
            Drupal.gmap.geocoder().geocode({'address': marker.address}, function (results, status) {
                if (status == google.maps.GeocoderStatus.OK) {
                    marker.latitude = results[0].geometry.lat();
                    marker.longitude = results[0].geometry.lng();
                }
            });
        }
    });

});

////////////////////////////////////////
//         Address widget             //
////////////////////////////////////////
Drupal.gmap.addHandler('address', function (elem) {
    var obj = this;

    // Respond to focus event.
    jQuery(elem).focus(function () {
        this.value = '';
    });

    // Respond to incoming movements.
    // Clear the box when the coords change...
    var binding = obj.bind("move", function () {
        elem.value = 'Enter an address';
    });
    // Send out outgoing movements.
    // This happens ASYNC!!!
    jQuery(elem).change(function () {
        if (elem.value.length > 0) {
            Drupal.gmap.geocoder().geocode({'address': elem.value}, function (results, status) {
                if (status == google.maps.GeocoderStatus.OK) {
                    obj.vars.latitude = results[0].geometry.location.lat();
                    obj.vars.longitude = results[0].geometry.location.lng();
                    obj.change("move", binding);
                }
                else {
                    // Todo: Get translated value using settings.
                    elem.value = 'Geocoder error: Address not found';
                }
            });
        }
        else {
            // Was empty. Ignore.
            elem.value = 'Enter an address';
        }
    });
});


////////////////////////////////////////
//  Locpick address handler (testing) //
////////////////////////////////////////
Drupal.gmap.addHandler('locpick_address', function (elem) {
    var obj = this;

    // Respond to focus event.
    jQuery(elem).focus(function () {
        this.value = '';
    });

    // Respond to incoming movements.
    // Clear the box when the coords change...
    var binding = obj.bind("locpickchange", function () {
        elem.value = 'Enter an address';
    });
    // Send out outgoing movements.
    // This happens ASYNC!!!
    jQuery(elem).change(function () {
        if (elem.value.length > 0) {
            Drupal.gmap.geocoder().geocode({'address': elem.value}, function (results, status) {
                if (status == google.maps.GeocoderStatus.OK) {
                    obj.locpick_coord = results[0];
                    obj.change("locpickchange", binding);
                }
                else {
                    // Todo: Get translated value using settings.
                    elem.value = 'Geocoder error: Address not found';
                }
            });
        }
        else {
            // Was empty. Ignore.
            elem.value = 'Enter an address';
        }
    });
});
