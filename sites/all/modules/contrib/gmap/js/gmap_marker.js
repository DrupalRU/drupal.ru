/**
 * @file
 * GMap Markers
 * GMap API version -- No manager
 */

/*global Drupal, GMarker */

// Replace to override marker creation
Drupal.gmap.factory.marker = function (opts) {
    return new google.maps.Marker(opts);
};

Drupal.gmap.addHandler('gmap', function (elem) {
    var obj = this;

    obj.bind('addmarker', function (marker) {
        if (!obj.map.markers) obj.map.markers = [];
        marker.marker.setMap(obj.map);
        obj.map.markers.push(marker.marker);
    });

    obj.bind('delmarker', function (marker) {
        marker.marker.setMap(null);
    });

    obj.bind('clearmarkers', function () {
        // @@@ Maybe don't nuke ALL overlays?
        if (obj.map.markers) {
            for (var i = 0; i < obj.map.markers.length; i++) {
                obj.map.markers[i].setMap(null);
            }
        }
    });
});