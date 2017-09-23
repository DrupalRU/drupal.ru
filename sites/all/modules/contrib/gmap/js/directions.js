/**
 * Google DirectionsService Integration
 *
 * obj.vars.directions.items [ <directionsRoute> ]
 * <directionsRoute> : {
 *   [title: <String> ].  <-- title used in wrapped display
 *   [origin: {latitude:<Number>,longitude:<Number>} ],        <-- if you don't pass this in then the first waypoint is the origin
 *   [waypoints: <Array>{latitude:<Number>,longitude:<Number>} ], <-- not necessary, but can be used to provide origin and destination
 *   [destination: {latitude:<Number>,longitude:<Number>} ],   <-- if you don't pass this in then the last waypoint is the destination
 * }
 * Each obj.vars.directions.items is considered a directions request.
 *
 * Events:
 * obj.change('destinationsdirections', function(route) : run a directions request
 * obj.change('directionsFail', -1, route ); : directions request failed.  check route.message
 * obj.change('directionsSuccess', -1, function(route) : route directions request succeeded
 * obj.change('directionsPanelInit', -1, function(route) : initializing directions result panel
 * obj.change('directionsPanelBoot', -1, function(route) : retrieving directions
 * obj.change('directionsPanelReady', -1, function(route) : directions panel is rendered and ready
 * obj.change('directionsPanelClick', -1, function(route) : directions panel is clicked
 * obj.change('directionsPanelActivate', -1, function(route) : directions panel is activated/deactivated
 *
 * Behaviors:
 * obj.vars.behavior.addDirectionsPanel : Automatically add a div after the map, to contain directions (otherwise you will have to manually have a gmap-control object)
 * obj.vars.behavior.directionsCustomSuccess : don't run the directionsPanelInit, directionsPanelBoot and directionsPanelReady events on success
 * obj.vars.behavior.directionsPanelWrap : wrap each directions list in it's own div panel
 * obj.vars.behavior.directionsCustomRenderer : don't automatically use the google DirectionsRenderer to convert directions to HTML in the directions result panel
 * obj.vars.behavior.directionsClickable : Make the directions results panel clickable
 *
 */
/*jshint -W069 */

Drupal.gmap.addHandler('gmap', function (element) {
    var obj = this;

    obj.bind('init', function () {
        if (obj.vars.behavior.addDirectionsPanel) {
            // try to recover the mapid : taken directly from gmap.js
            var map = obj.map.getDiv();
            var mapid = map.id.split('-');
            if (Drupal.settings.gmap_remap_widgets) {
                if (Drupal.settings['gmap_remap_widgets'][obj.id]) {
                    jQuery.each(Drupal.settings['gmap_remap_widgets'][obj.id].classes, function () {
                        jQuery(obj).addClass(this);
                    });
                    mapid = Drupal.settings['gmap_remap_widgets'][obj.id].id.split('-');
                }
            }
            var instanceid = mapid.pop();
            mapid.shift();
            mapid = mapid.join('-');
            // end of recover mapid

            obj.directions = $('<div id="gmaps-' + mapid + '-directions0" class="gmap-control gmap-control-directions" />'); // this assumes that directions0 isn't taken already
            $(map).after(obj.directions);
            Drupal.gmap.setup.call(obj.directions[0]);
        }
    });

});

Drupal.gmap.addHandler('directions', function (element) {
    var obj = this;

    obj.bind('ready', function () {
        obj.directionsservice = new google.maps.DirectionsService();
        obj.directionspanel = $(element);

        if (obj.vars && obj.vars.directions) {
            $.each(obj.vars.directions.items, function (directionsIndex, route) {
                route.index = directionsIndex;
                obj.change('destinationsdirections', 'all', route);
            });
        }

    });

    obj.bind('destinationsdirections', function (route) {
        var directionsRequest = {waypoints: []}; // this has to match the google.maps.DirectionsRequest

        if (route.origin) {
            if (route.origin.latitude) {
                route.origin = new google.maps.LatLng(route.origin.latitude, route.origin.longitude);
            }
            directionsRequest.origin = route.origin;
        }
        $.each(route.waypoints, function (routeIndex, waypoint) {
            waypoint.waypointIndex = routeIndex;
            if (waypoint.latitude) {
                waypoint.location = new google.maps.LatLng(waypoint.latitude, waypoint.longitude);
            }
            var requestWaypoint = {
                location: waypoint.location,
                stopover: true  // false would be better, but it makes the directions fail
            };
            $.extend(waypoint, requestWaypoint);
            if (!directionsRequest.origin) { // if no origin was passed, then take the first point
                directionsRequest.origin = waypoint.location;
            }
            else {
                directionsRequest.waypoints.push(requestWaypoint);
            }
        });
        if (route.destination) {
            if (route.destination.latitude) {
                route.destination = new google.maps.LatLng(route.destination.latitude, route.destination.longitude);
            }
            directionsRequest.destination = route.destination;
        }
        else if (directionsRequest.waypoints.length > 0) {
            directionsRequest.destination = directionsRequest.waypoints.pop().location;
        } // if no destintion was passed, then take the last point
        else {
            route.message = Drupal.t('Not enough point');
            route.status = false;
            obj.change('directionsfail', 'all', route);
        }

        //parvietosanas veids
        switch (route.type) {
            case 1:
                directionsRequest.travelMode = google.maps.TravelMode.WALKING;
                break;
            default:
                directionsRequest.travelMode = google.maps.TravelMode.DRIVING;
        }

        if (directionsRequest.waypoints.length === 0) {
            delete directionsRequest.waypoints;
        } // don't send waypoints if there aren't any
        obj.directionsservice.route(directionsRequest, function (directions, status) {
            route.status = status;
            if (status == google.maps.DirectionsStatus.OK) {
                route.directions = directions;
                obj.change('directionsSuccess', 'all', route);
            }
            else {
                obj.change('directionsFail', 'all', route);
            }
        });

    });
    obj.bind('directionsSuccess', function (route) {
        if (!obj.vars.behavior.directionsCustomSuccess) {
            obj.change('directionsPanelInit', 'all', route);
            obj.change('directionsPanelBoot', 'all', route);
            obj.change('directionsPanelReady', 'all', route);
        }
    });

    obj.bind('directionsPanelInit', function (route) {
        if (obj.vars.behavior.directionsPanelWrap) {
            route.panel = $('<div class="gmap-control-directionsPanel" /></div>');
            if (route.title) {
                route.panel.append('<h3 class="directions-title">' + route.title + '</h3>');
            }
            obj.directionspanel.append(route.panel);
        }
        else {
            route.panel = obj.directionspanel;
        }
    });
    obj.bind('directionsPanelBoot', function (route) {
        if (!obj.vars.behavior.directionsCustomRenderer) {
            route.renderer = new google.maps.DirectionsRenderer({map: obj.map, panel: route.panel[0]});
            google.maps.event.addListener(route.renderer, 'directions_changed', function (event) {
                obj.change('directionsPanelChanged', 'All', route);
            });
            route.renderer.setDirections(route.directions);
        }
    });

    obj.bind('directionsPanelReady', function (route) {
        /* Reformat the table to include some of the point titles */
        /* @TODO */
    });
    obj.bind('directionsPanelReady', function (route) {
        if (obj.vars.behavior.directionsClickable) {
            route.obj = obj;
            route.panel.bind('click', route, function (event) {
                var obj = event.data.obj; // not sure if this is necessary
                obj.change('directionsPanelClick', 'all', event.route);
            });

            if (obj.vars.behavior.directionsClickableDefaultClicked) {
                route.panel.click();
            }
        }
    });

    obj.bind('directionsPanelClick', function (route) { // just do some class stuff for css
        route.panel.toggleClass('directions-active');
    });
    obj.bind('directionsPanelClick', function (route) { // activate and deactivate the directions on the map
        if (obj.vars.behavior.directionsPanelActivateOnClick) { // this behaviour may be undesirable
            if (panel.hasClass('directions-active')) {
                if (!obj.vars.behavior.directionsPanelActivateSingle && obj.map.data('activeDirectionsPanel')) {
                    obj.map.data('activeDirectionsPanel').click();
                }
                route.renderer.setMap(obj.map);
            }
            else {
                route.renderer.setMap(null);
            }
        }
    });

});
