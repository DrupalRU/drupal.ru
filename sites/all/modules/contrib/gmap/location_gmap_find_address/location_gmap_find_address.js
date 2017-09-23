(function ($) {
    Drupal.behaviors.mjh_views_alter = {
        attach: function () {

            $("button.location-gmap-find-address-button").click(function (e) {
                e.preventDefault();
                var params = {
                    addressParts: {},
                    separator: ', '
                };
                $("fieldset#" + $(this).val() + " .form-item input[type=text]," +
                "fieldset#" + $(this).val() + " .form-item select > option:selected").each(function () {
                    if (!$(this).hasClass('gmap-control') && $(this).val() !== '') {
                        // Get the html id of the element.
                        var isOption = $(this).is('option');
                        var id;
                        if (isOption) {
                            id = $(this).parent().attr('id');
                        }
                        else {
                            id = $(this).attr('id');
                        }
                        var id_parts = id.split('-');

                        // The last part of the input id, contains the type
                        // of the location field. It can be: name, street,
                        // additional, city, province, postal-code, country.
                        var locationFieldType = id_parts[id_parts.length - 1];

                        // Assign the value of the input to the parts
                        // object.
                        if (isOption) {
                            params.addressParts[locationFieldType] = $(this).text();
                        } else {
                            params.addressParts[locationFieldType] = $(this).val();
                        }
                    }
                });

                // Trigger location_gmap_find_address.address_parts_alter.
                // Allow altering the address parts by custom code.
                var location_field_separator = ', ';
                $(document).trigger('location_gmap_find_address.address_parts_alter', [params]);
                var address_parts_array = [];
                for (var part in params.addressParts) {
                    if (params.addressParts.hasOwnProperty(part)) {
                        address_parts_array.push(params.addressParts[part]);
                    }
                }

                var address_string = address_parts_array.join(location_field_separator);
                var gmap_id = $("fieldset#" + $(this).val() + " .gmap-map").attr('id');
                var geocoder;
                if (google.maps.version !== 'undefined') { // assume Google Maps API v3 as API v2 did not have this variable
                    geocoder = new google.maps.Geocoder();
                    geocoder.geocode({'address': address_string}, function (results, status) {
                        if (status == google.maps.GeocoderStatus.OK) {
                            var m = Drupal.gmap.getMap(gmap_id);
                            m.locpick_coord = results[0].geometry.location;
                            m.change('locpickchange');
                            m.map.setCenter(results[0].geometry.location);
                            m.map.setZoom(14);
                        }
                        else {
                            alert(Drupal.t("Your address was not found."));
                        }
                    });
                }
                else {
                    geocoder = new GClientGeocoder();
                    geocoder.reset(); // Clear the client-side cache
                    geocoder.getLatLng(address_string, function (point) {
                        if (!point) {
                            alert(Drupal.t("Your address was not found."));
                        }
                        else {
                            var m = Drupal.gmap.getMap(gmap_id);
                            m.locpick_coord = point;
                            m.change('locpickchange');
                            m.map.setCenter(point, 14);

                        }
                    });
                }

            });

        }

    };
})(jQuery);

