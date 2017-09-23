/**
 * @file
 * Common marker highlighting routines.
 */

/**
 * Highlights marker on rollover.
 * Removes highlight on previous marker.
 *
 * Creates a "circle" at the given point
 * Circle is global variable as there is only one highlighted marker at a time
 * and we want to remove the previously placed polygon before placing a new one.
 *
 * Original code "Google Maps JavaScript API Example"
 * JN201304:
 *    converted rpolygons to circles (not using the shapes.js API, should we be?)
 *    move marker highlight events to custom handler here, to handle radius in pixels (note behavior.radiusInMeters to skip geodesic calcs)
 *    removed google.events and moved events to gmaps binds
 *    added overlay object for creating a shape based on pixels instead of meters (seems to be the use case?)
 *    added gmaps binds for marker higlights, and general highlights.
 * JN201305 refactored to use a single overlay.  move functions from draw method to solve zoom problem, and multiple
 *    highlights problem.
 *
 * You can add highlights to a map with:
 *    obj.change('highlightAdd',-1, {latitude:#, longitude:#} );
 * You can highlight a marker with:
 *    obj.change('markerHighlight',-1, marker);
 *      marker: that marker object used when creating the marker.  It can have options set at marker.highlight
 *
 * A Highlight object has to have either a <LatLng>Position or a <Number>latitude and <Number>longitude
 * Note the new highlight options = {
 *       radius: 10, // radius in pixels
 *       color: '#777777',
 *       weight: 2,
 *       opacity: 0.7,
 *       behavior: {
 *          draggable: false,
 *          editable: false,
 *       }
 *       opts: { actual google.maps.Circle opts can be put here for super custom cases }
 * }
 */

Drupal.gmap.factory.highlight = function (options) {
    /** @note it could be argued that we use the shapes library to create a circle,
     * but this requires the shapes library be loaded and it would make all highlights
     * repond to shapes events.
     */
    return new google.maps.Circle(options);
};

Drupal.gmap.addHandler('gmap', function (elem) {
    var obj = this;
    obj.highlights = {};

    /**
     * This is a single overlay that can hold multiple highlight.
     * All highlight shapes will be creted in this overlay, and use
     * it to translate pixel dimensions to meters.
     */
    var highlightOverlay = function () {
        this.highlights = []; // this will hold all of the highlights that we created, in case we need to recalculate/deactivate them
    };
    highlightOverlay.prototype = new google.maps.OverlayView();

    // overlay method for when you .setMap( some map );
    highlightOverlay.prototype.onAdd = function (map) {
    };
    // overlay method for when you .setMap(null);
    highlightOverlay.prototype.onRemove = function () {
        // we have to recalculate radii for all shapes
        var self = this;
        jQuery.each(this.highlights, function (index, highlight) {
            if (highlight.shape.getMap()) { // don't calculate if we don't have a map.
                self.calculateHighlight(highlight); //recalculate all of those radii
            }
        });
    };

    // overlay method executed on any map change methods (zoom/move)
    highlightOverlay.prototype.draw = function () {
        // we have to recalculate radii for all shapes
        var self = this;
        jQuery.each(this.highlights, function (index, highlight) {
            if (highlight.shape.getMap()) { // don't calculate if we don't have a map.
                self.deactivateHighlight(highlight); //recalculate all of those radii
            }
        });
    };

    highlightOverlay.prototype.configHighlight = function (highlight) {
        if (!highlight.opts) {
            highlight.opts = {};
        } // sanity
        if (!highlight.behavior) {
            highlight.behavior = {};
        } // sanity
        if (!highlight.position) {
            highlight.position = new google.maps.LatLng(highlight.latitude, highlight.longitude);
        } // if you have a pos already then use it, otherwise gimme a lat/lon

        jQuery.each({ // collect the options from either the highlight.opts object, from the passed target value, as a behavior or a defaultVal value.
            radius: {target: 'radius', defaultVal: 10}, // radius in pixels
            strokeColor: {target: 'border', defaultVal: '#777777'},
            strokeWeight: {target: 'weight', defaultVal: 2},
            strokeOpacity: {target: 'opacity', defaultVal: 0.7},
            fillColor: {target: 'color', defaultVal: '#777777'},
            fillOpacity: {target: 'opacity', defaultVal: 0.7},
            draggable: {behavior: 'draggable', defaultVal: false},
            editable: {behavior: 'editable', defaultVal: false}
        }, function (key, config) {
            if (highlight.opts[key]) { // options was passed in
                return true;
            }
            else if (config.target && highlight[ config.target ]) { // highight[target] can give us a setting
                highlight.opts[key] = highlight[ config.target ];
            }
            else if (config.behavior && highlight.behavior && highlight.behavior[ config.behavior ]) { // value is a behaviour, should it be enabled?
                highlight.opts[key] = highlight.behavior[ config.behavior ];
            }
            else if (config.defaultVal) { // defaultVal value
                highlight.opts[key] = config.defaultVal;
            }
        });

        highlight.opts.center = highlight.position;
        // note that there is no opts.map, unless you passed one in.  maybe we should make sure that you didn't?

        // add this highlight to our list, so that we can draw it in the draw method (which will also redraw it after map change events.
        this.highlights.push(highlight);
    };
    // determine how big the circle should be in meters (as we were likely passed pixels).  This radius changes on zoom and move events.
    highlightOverlay.prototype.calculateHighlight = function (highlight) { // this nees a better name

        if (highlight.behavior.radiusInMeters) {
            highligh.opts.radiusInMeters = highlight.opts.radius;
        }
        else {
            var mapZoom = this.map.getZoom();
            var projection = this.getProjection();
            var center = projection.fromLatLngToDivPixel(highlight.opts.center, mapZoom);
            var radius = highlight.opts.radius;
            var radial = projection.fromDivPixelToLatLng(new google.maps.Point(center.x, center.y + radius), mapZoom); // find a point that is the radius distance away in pixels from the ccenter point.
            highlight.opts.radiusInMeters = google.maps.geometry.spherical.computeDistanceBetween(highlight.opts.center, radial);
        }

        if (highlight.shape) {
            highlight.shape.setOptions(highlight.opts);
            // we can use this if we don't care about other options changing : highlight.shape.setRadius(highlight.opts.radiusInMeters)
        }
        else {
            highlight.shape = Drupal.gmap.factory.highlight(jQuery.extend({}, highlight.opts, {radius: highlight.opts.radiusInMeters})); // only pass radiusinmeters to g.m.circle.  We keep the pixel radius in case we need to calculate again after a zoom
        }
    };
    highlightOverlay.prototype.activateHighlight = function (highlight) {
        if (!highlight.shape) {
            this.configHighlight(highlight);
            this.calculateHighlight(highlight);
        }
        highlight.shape.setMap(this.map);
    };
    highlightOverlay.prototype.deactivateHighlight = function (highlight) {
        if (highlight.shape) {
            highlight.shape.setMap(null);
        }
    };
    highlightOverlay.prototype.updateHighlight = function (highlight) {
        if (highlight.shape) {
            this.configHighlight(highlight);
            this.calculateHighlight(highlight);
        }
    };

    // prepare a single highlight overlay to be used for all highlights
    obj.bind('init', function (highlight) {
        obj.highlightOverlay = new highlightOverlay(obj.map);
        obj.highlightOverlay.setMap(obj.map); // this will trigger the onAdd() method, and the first draw()
    });

    // set and remove map highlights
    obj.bind('highlightAdd', function (highlight) { // if you activate an activated highlight, nothing happens.
        obj.highlightOverlay.activateHighlight(highlight);
    });
    obj.bind('highlightRemove', function (highlight) {
        obj.highlightOverlay.deactivateHighlight(highlight);
    });
    obj.bind('highlightUpdate', function (highlight) {
        obj.highlightOverlay.updateHighlight(highlight);
    });

    // Marker specific highlight events:
    var highlightedMarkers = []; // remember markers that have been highlighted. so that we can un-highlight them all at one.  The defaultVal behaviour is to allow only 1 marker highlighted at any time.
    obj.bind('markerHighlight', function (marker) {
        highlightedMarkers.push(marker);

        // If the highlight arg option is used in views highlight the marker.
        if (!marker.highlight) {
            marker.highlight = {};
        }
        if (!marker.highlight.color && obj.vars.styles.highlight_color) {
            marker.highlight.color = '#' + obj.vars.styles.highlight_color;
        }
        marker.highlight.position = marker.marker.getPosition();
        obj.change('highlightAdd', -1, marker.highlight);
    });
    obj.bind('markerUnHighlight', function (marker) {
        if (marker.highlight) {
            obj.change('highlightRemove', -1, marker.highlight);
            delete marker.highlight;
        }
    });
    obj.bind('markerUnHighlightActive', function () {
        var marker;
        while (!!(marker = highlightedMarkers.pop())) {
            obj.change('highlightRemove', -1, marker);
        }
    });

    /**
     * Marker Binds
     *
     * Marker highlight code has been moved to this file from the marker.js
     *
     * Note that we rely on the obj.vars.behavior.highlight var to
     * decide if should highlight markers on events.
     * This decision could be made as an outer if conditional, instead
     * of repeated inside each bind, but this arrangement allows for
     * the behaviour to change, at a small cost.
     */
    obj.bind('addmarker', function (marker) {
        if (obj.vars.behavior.highlight) {
            google.maps.event.addListener(marker.marker, 'mouseover', function () {
                obj.change('markerHighlight', -1, marker);
            });
            google.maps.event.addListener(marker.marker, 'mouseout', function () {
                obj.change('markerUnHighlight', -1, marker);
            });
            google.maps.event.addListener(marker.marker, 'mouseout', function () {
                obj.change('markerUnHighlight', -1, marker);
            });
        }
        // If the highlight arg option is used in views highlight the marker.
        if (marker.opts.highlight == 1) {
            obj.change('markerHighlight', -1, marker);
        }
    });

// Originally I moved mouse highlights to the extra event binds before I realized that there is likely a usecase for highlights without enabling extra events
//   obj.bind('mouseovermarker', function(marker) {
//     if (obj.vars.behavior.highlight && marker) {
//       obj.change('markerHighlight',-1,marker);
//     }
//   });
//   obj.bind('mouseoutmarker', function(marker) {
//     if (obj.vars.behavior.highlight && marker) {
//       obj.change('markerUnHighlight',-1,marker);
//     }
//   });

});
