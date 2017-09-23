/**
 * @file
 * GMap Shapes
 * GMap API version / Base case
 *
 * @NOTE This code now depends in various places, on the google.maps.geometry library.
 * 1. points encoding/decoding
 * 2. spherical distance calculations
 * If this library isn't loaded then YOU WILL GET SILENT FAILURE.  There should be some kind
 * of user warning here, if the geometry library hasn't been loaded. I am not sure what the
 * gmaps convention for such reporting is?  is a console.debug acceptable?
 * Note that you can specify to add the geometry library at the php side of things using hook_gmap()
 */

/*global $, Drupal, google.maps, !google.maps.geometry! */
(function ($) {

    Drupal.gmap.addHandler('gmap', function (elem) {
        var obj = this;

        // prepare and build a google.maps shape object
        obj.bind('prepareshape', function (shape) {
            var style;
            var fillstyle = false; // does this shape have a fill option?
            var cargs = {};
            var pa = []; // point array (array of LatLng-objects)

            // positioning determination
            switch (shape.type) {
                case 'circle':
                    if (shape.center.length) {
                        shape.center = new google.maps.LatLng(shape.center[0], shape.center[1]);
                    } // otherwise it should be a LatLng already
                    if (shape.opts.radius) {
                        shape.radius = shape.opts.radius;
                    }
                    else if (shape.point2) {
                        if (shape.point2.length) {
                            shape.point2 = new google.maps.LatLng(shape.point2[0], shape.point2[1]);
                        } // otherwise it should be a LatLng already
                        shape.radius = (google.maps.geometry) ? google.maps.geometry.spherical.computeDistanceBetween(shape.center, shape.point2) : 250;
                    } // if you didn't pass a shape.point2, then you should have passed a shape.radius in meters
                    break;

                case 'rpolygon': /* this is deprecated as we have circle now.  It is left for backwards compatibility */
                    if (shape.center.length) {
                        shape.center = new google.maps.LatLng(shape.center[0], shape.center[1]);
                    } // otherwise it should be a LatLng already
                    if (shape.point2.length) {
                        shape.point2 = new google.maps.LatLng(shape.point2[0], shape.point2[1]);
                    } // otherwise it should be a LatLng already
                    shape.radius = (google.maps.geometry) ? google.maps.geometry.spherical.computeDistanceBetween(shape.center, shape.point2) : 250;
                    if (!shape.numpoints) {
                        shape.numpoints = 20;
                    }
                    pa = obj.poly.calcPolyPoints(shape.center, shape.radius, shape.numpoints);
                    break;

                case 'polygon':
                    fillstyle = true;
                    break;

                case 'line':
                    if (shape.points) {
                        $.each(shape.points, function (i, n) {
                            if (n.length) {
                                n = new google.maps.LatLng(n[0], n[1]);
                            } // otherwise it should be a LatLng already
                            pa.push(n);
                        });
                    }
                    break;

                case 'encoded_polygon':
                    fillstyle = true;
                    break;

                case 'encoded_line':
                    pa = ( google.maps.geometry ) ? google.maps.geometry.encoding.decodePath(shape.path) : []; // this trinary prevents errors if the google.maps gemoetry library wasn't loaded
                    break;
            }

            /**
             * the shapes.style processing is a leftover from the gmaps v2
             * code, and the shapes configuration system, from the php side
             * of things.
             */
            if (shape.style) {
                if (typeof shape.style === 'string') { // the style is an index for one of our vars.styles
                    if (obj.vars.styles[shape.style]) {
                        style = obj.vars.styles[shape.style].slice(); // copy the array
                    }
                }
                else {
                    style = shape.style.slice(); // copy that array
                }
                style[0] = '#' + style[0]; // color
                style[1] = Number(style[1]); // strokewidth
                style[2] = style[2] / 100; // strokeOpacity
                if (fillstyle) {
                    style[3] = '#' + style[3]; // fill colour
                    style[4] = style[4] / 100; // fill opacity
                }

                if (shape.type == 'encoded_line' || shape.type == 'line') {
                    shape.color = style[0];
                    shape.weight = style[1];
                    shape.opacity = style[2];
                }
                else if (shape.type == 'encoded_polygon') {
                    if (shape.polylines) {
                        $.each(shape.polylines, function (i, polyline) {
                            polyline.color = style[0];
                            polyline.weight = style[1];
                            polyline.opacity = style[2];
                        });
                    }
                    shape.fill = true;
                    shape.color = style[3];
                    shape.opacity = style[4];
                    shape.outline = true;
                }
                else if (shape.type == 'polygon') {
                  shape.strokeColor = style[0];
                  shape.strokeWeight = style[1];
                  shape.strokeOpacity = style[2];
                  shape.fillColor = style[3];
                  shape.fillOpacity = style[4];
                }
            }

            // add any options to the configuration
            if (shape.opts) {
                $.extend(cargs, shape.opts);
            }

            /**
             * In general: (inherited concepts.  If you change these, make sure you check them on the php side of things)
             *  shape.color : color used for line and fill
             *  shape.weight : stroke weight in pixels
             *  shape.opacity : fill/stroke opacity in decimal value (0< opactity <1)
             *  shape.fill : boolean direction to fill the shape
             */
            // build the shape with options
            switch (shape.type) {
                case 'circle':
                    cargs = {center: shape.center, radius: shape.radius, strokeColor: shape.color }; // required arges
                    if (shape.color) {
                        cargs.strokeColor = shape.color;
                    } // outline color
                    if (shape.weight) {
                        cargs.strokeWeight = shape.weight;
                    } // boundary line weight
                    if (shape.fill) {
                        cargs.fillColor = shape.color;
                    } // shape fill color
                    if (shape.opacity) {
                        cargs.strokeOpacity = shape.opacity;
                        cargs.fillOpacity = shape.opacity;
                    } // shape opacity
                    shape.shape = new google.maps.Circle(cargs);
                    break;
                case 'rpolygon':
                case 'encoded_polygon':
                    cargs = { path: pa }; // required args
                    if (shape.outline) {
                        cargs.strokeColor = shape.color;
                    }
                    if (shape.weight) {
                        cargs.strokeWeight = shape.weight;
                    }
                    if (shape.fill) {
                        cargs.fillColor = shape.color;
                    }
                    if (shape.opacity) {
                        cargs.strokeOpacity = shape.opacity;
                        cargs.fillOpacity = shape.opacity;
                    }
                    shape.shape = new google.maps.Polygon(cargs);
                    break;
                case 'polygon':
                    cargs = { path: pa };
                    if (shape.strokeColor) {
                      cargs.strokeColor = shape.strokeColor;
                    }
                    if (shape.strokeWeight) {
                      cargs.strokeWeight = shape.strokeWeight;
                    }
                    if (shape.strokeOpacity) {
                      cargs.strokeOpacity = shape.strokeOpacity;
                    }
                    if (shape.fillColor) {
                      cargs.fillColor = shape.fillColor;
                    }
                    if (shape.fillOpacity) {
                      cargs.fillOpacity = shape.fillOpacity;
                    }
                    shape.shape = new google.maps.Polygon(cargs);
                    break;

                case 'line':
                case 'encoded_line':
                    cargs = { path: pa }; // required args
                    if (shape.color) {
                        cargs.strokeColor = shape.color;
                    }
                    if (shape.weight) {
                        cargs.strokeWeight = shape.weight;
                    }
                    if (shape.opacity) {
                        cargs.strokeOpacity = shape.opacity;
                    }
                    shape.shape = new google.maps.Polyline(cargs);
                    break;
            }
        });

        // add a prepared shape to the map
        obj.bind('addshape', function (shape) {
            if (!obj.vars.shapes) {
                obj.vars.shapes = [];
            }
            obj.vars.shapes.push(shape);
            shape.shape.setMap(obj.map);

            if (obj.vars.behavior.clickableshapes) {
                google.maps.event.addListener(shape.shape, 'click', function () {
                    obj.change('clickshape', -1, shape);
                });
            }
            if (obj.vars.behavior.shapesextraevents) {
                google.maps.event.addListener(shape.shape, 'dblclick', function () {
                    obj.change('dblclickshape', -1, shape);
                });
                google.maps.event.addListener(shape.shape, 'mousedown', function () {
                    obj.change('mousedownshape', -1, shape);
                });
                google.maps.event.addListener(shape.shape, 'mouseout', function () {
                    obj.change('mouseoutshape', -1, shape);
                });
                google.maps.event.addListener(shape.shape, 'mouseover', function () {
                    obj.change('mouseovershape', -1, shape);
                });
                google.maps.event.addListener(shape.shape, 'mouseup', function () {
                    obj.change('mouseupshape', -1, shape);
                });
                google.maps.event.addListener(shape.shape, 'mousemove', function () {
                    obj.change('mousemoveshape', -1, shape);
                });
                google.maps.event.addListener(shape.shape, 'rightclick', function () {
                    obj.change('rightclickshape', -1, shape);
                });
            }
        });

        obj.bind('delshape', function (shape) {
            shape.shape.setMap(null);
        });

        obj.bind('clearshapes', function () {
            if (obj.vars.shapes) {
                $.each(obj.vars.shapes, function (i, n) {
                    obj.change('delshape', -1, n);
                });
            }
        });
    });

})(jQuery);
