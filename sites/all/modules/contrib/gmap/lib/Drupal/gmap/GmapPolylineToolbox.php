<?php

/**
 * @file
 * Contains GmapPolylineToolbox.php
 * Encoded polyline utilities.
 */

/**
 * References:
 * [1] http://code.google.com/apis/maps/documentation/polylinealgorithm.html
 * [2] http://facstaff.unca.edu/mcmcclur/GoogleMaps/EncodePolyline/
 * [3] http://mathworld.wolfram.com/
 */

namespace Drupal\gmap;


class GmapPolylineToolbox {

  const GMAP_DP_EPSILON = 0.00001;

  const GMAP_ZOOM_LEVELS = 18;

  const GMAP_ZOOM_FACTOR = 2;

  private $latlonNumber;

  private $latlonLevels;

  private $encodedlatlonNumber;

  private $encoded;

  private $startPoint;

  private $endPoint;

  private $distance;

  private $measurePoint;

  /**
   * @var array
   */
  private $points;

  /**
   * @var array
   */
  private $pointWeights;

  private $weight;

  static protected $levels;

  /**
   * @var static Singleton instance
   */
  static protected $gmapInstance;

  /**
   * Do not change.
   */
  private function __construct() {
    self::$levels = $this->getZoomLevels();
  }

  /**
   * Getting instance.
   * @return array
   *   GmapPolylineToolbox SingleTon instance
   */
  static public function getInstance() {
    if (is_null(self::$gmapInstance)) {
      self::$gmapInstance = new self();
    }
    return self::$gmapInstance;
  }


  /**
   * The following  methods will encode numbers.
   *
   * So that they may be used in "Encoded Polylines" on Google Maps.
   * The encoding is described here:
   *   http://code.google.com/apis/maps/documentation/polylinealgorithm.html
   *
   * Numbers for latitudes/longitudes and levels are encoded slightly
   * differently--when generating Encoded Polylines, latitudes and
   * longitudes are encoded with gmap_polyutil_encode_signed(),
   * and "levels" are encoded using gmap_polyutil_encode_unsigned().
   *
   * former gmap_polyutil_encode_latlon($x)
   */
  public function setLatLonNumber($x) {
    $this->latlonNumber = $x;
    return $this;
  }

  /**
   * Former gmap_polyutil_encode_latlon($x).
   *
   * @return string
   *   LatLon.
   */
  public function getEncodedLatLon() {
    $this->latlonNumber = round($this->latlonNumber * 1e5) << 1;
    if ($this->latlonNumber < 0) {
      $this->latlonNumber = ~($this->latlonNumber);
    }
    $this->encodedlatlonNumber = $this->setLatLonNumber($this->latlonNumber)->getEncode();
    return $this->encodedlatlonNumber;
  }

  /**
   * Former gmap_polyutil_encode_latlon($x).
   *
   * @return string
   *   Levels.
   */
  public function getEncodedLevels() {
    $this->latlonLevels = $this->setLatLonNumber(abs($this->latlonNumber))->getEncode();
    return $this->latlonLevels;
  }

  /**
   * Former _gmap_polyutil_encode($x).
   *
   * @return string
   *   Encoded.
   */
  public function getEncode() {
    $this->encoded = '';
    while ($this->latlonNumber >= 32) {
      $this->encoded .= chr((32 | ($this->latlonNumber & 31)) + 63);
      $this->latlonNumber >>= 5;
    }
    $this->encoded .= chr(($this->latlonNumber & 31) + 63);
    return $this->encoded;
  }

  /**
   * Points set from former gmap_polyutil_dist($p1, $p2).
   *
   * @param string $p1
   *   Input 1.
   *
   * @param string $p2
   *   Input 2.
   *
   * @return object
   *   $this.
   */
  public function setLinePoints($p1, $p2) {
    $this->startPoint = $p1;
    $this->endPoint = $p2;
    return $this;
  }

  /**
   * Distance in two dimensions.
   *
   * √((x1-x0)^2 + (y1-y0)^2)
   * former gmap_polyutil_dist($p1, $p2)
   */
  public function getDist() {
    $this->distance = sqrt(pow($this->endPoint[0] - $this->startPoint[0], 2) + pow($this->endPoint[1] - $this->startPoint[1], 2));
    return $this->distance;
  }

  /**
   * Set $this->measurePoint to value.
   *
   * @param string $q
   *   Input string.
   *
   * @return object
   *   $this
   */
  public function setMeasurePoint($q) {
    $this->measurePoint = $q;
    return $this;
  }

  /**
   * Distance between a point and a line segment.
   *
   * former gmap_polyutil_point_line_dist()
   *
   * @return float
   *   Distance.
   */
  public function getPointLineDist() {
    if ($this->startPoint[0] == $this->endPoint[0] && $this->startPoint[1] == $this->endPoint[1]) {
      // lp1 and lp2 are the same point--they don't define a line--so we return
      // the distance between two points.
      return $this->setLinePoints($this->measurePoint, $this->startPoint)->getDist();
    }

    // Use the dot product to find where q lies with respect to the line segment
    // p1p2. For more information, see:
    // http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
    // http://www.codeguru.com/forum/printthread.php?t=194400
    $u = (($this->endPoint[1] - $this->startPoint[1]) * ($this->measurePoint[1] - $this->startPoint[1]) + ($this->endPoint[0] - $this->startPoint[0]) * ($this->measurePoint[0] - $this->startPoint[0])) / (pow($this->endPoint[1] - $this->startPoint[1], 2) + pow($this->endPoint[0] - $this->startPoint[0], 2));

    // Point is not alongside segment, it is further off in $p1's direction.
    if ($u <= 0) {
      return $this->setLinePoints($this->measurePoint, $this->startPoint)->getDist();
    }
    // Point is not alongside segment, it is further off in $p2's direction.
    elseif ($u >= 1) {
      return $this->setLinePoints($this->measurePoint, $this->endPoint)->getDist();
    }
    else {
      // Point is alongside segment.
      // Calculate distance between q and the nearest point on the line segment.
      // Use $u to calculate the nearest point on the line segment:
      // p1 + u*(p2 - p1) => [p1x + u*(p2x - p1x), p1y + u*(p2y - p1y)]
      return $this->setLinePoints($this->measurePoint, array($this->startPoint[0] + $u * ($this->endPoint[0] - $this->startPoint[0]), $this->startPoint[1] + $u * ($this->endPoint[1] - $this->startPoint[1])))->getDist();
    }
  }

  /**
   * Former gmap_polyutil_dp_encode($points).
   *
   * @param array $points
   *   An array of coordinate pairs.
   *
   * @return object
   *   $this
   */
  public function setPoints(array $points) {
    $this->points = $points;
    return $this;
  }

  /**
   * Implementation of the Douglas-Peucker polyline simplification algorithm.
   *
   * See:
   * http://facstaff.unca.edu/mcmcclur/GoogleMaps/EncodePolyline/algorithm.html
   *
   * former gmap_polyutil_dp_encode($points)
   *
   * @return array
   *   An array of keys => weights; the keys correspond with indices of points
   *   in the $points array. Some points may be insignificant according to the
   *   algorithm - they will not have entries in the return array. The "weights"
   *   are actually the point's distance from the line segment that it
   *   subdivides.
   */
  public function getDPEncode() {
    $this->pointWeights = array();
    $max_i = 0;

    if (count($this->points) > 2) {
      // The 'stack' holds line segments to be simplified.
      $stack[] = array(0, count($this->points) - 1);

      while (count($stack) > 0) {
        // Take a line segment to look at.
        $segment = array_pop($stack);

        // Figure out which subdividing point is the furthest off the line segment.
        $max_dist = 0;
        for ($i = $segment[0] + 1; $i < $segment[1]; $i++) {
          $dist = $this
            ->setMeasurePoint($this->points[$i])
            ->setLinePoints($this->points[$segment[0]], $this->points[$segment[1]])
            ->getPointLineDist();
          if ($dist > $max_dist) {
            $max_dist = $dist;
            $max_i = $i;
          }
        }

        // If the subdividing point found above is significantly off the line,
        // segment then we want to simplify further. Add sub-segments to the stack.
        if ($max_dist > self::GMAP_DP_EPSILON) {
          $this->pointWeights[$max_i] = $max_dist;
          array_push($stack, array($segment[0], $max_i));
          array_push($stack, array($max_i, $segment[1]));
        }
      }
    }

    // The first and last points of the line should always be visible.
    $levels = $this->getZoomLevels();
    $this->pointWeights[0] = $levels[0];
    $this->pointWeights[count($this->points) - 1] = $levels[0];

    return $this->pointWeights;
  }

  /**
   * Simplify a set of points and generate an "Encoded Polyline" for GMaps.
   *
   * former gmap_polyutil_polyline($points)
   *
   * @return array
   *   An array containing the point and zoom information necessary to display
   *   encoded polylines on Google Maps:
   *   'points', 'levels', 'numLevels', and 'zoomFactor'.
   */
  public function getPolyline() {
    $points_encoded = '';
    $levels_encoded = '';

    // Simplify the line.
    $weights = $this->getDPEncode();

    $previous = array(0, 0);
    foreach ($this->points as $i => $p) {
      if (isset($weights[$i])) {
        // Encode each simplified point,
        // the deltas between points are encoded, rather than the point values themselves.
        $points_encoded .= $this->setLatLonNumber($p[0] - $previous[0])->getEncodedLatLon() . $this->setLatLonNumber($p[1] - $previous[1])->getEncodedLatLon();
        $levels_encoded .= $this->setLatLonNumber($this->setWeight($weights[$i])->getZoomLevel())->getEncodedLevels();
        $previous = $p;
      }
    }

    return array(
      'points' => $points_encoded,
      'levels' => $levels_encoded,
      'numLevels' => self::GMAP_ZOOM_LEVELS,
      'zoomFactor' => self::GMAP_ZOOM_FACTOR,
    );
  }

  /**
   * Build a logarithmic scale of zoom levels.
   *
   * former _gmap_polyutil_zoom_levels()
   *
   * @return mixed
   *   Zoom levels.
   */
  public function getZoomLevels() {
    if (!isset(self::$levels)) {
      for ($i = 0; $i < self::GMAP_ZOOM_LEVELS; $i++) {
        self::$levels[$i] = self::GMAP_DP_EPSILON * pow(self::GMAP_ZOOM_FACTOR, self::GMAP_ZOOM_LEVELS - $i - 1);
      }
    }
    return self::$levels;
  }

  /**
   * Setting a weight.
   *
   * former _gmap_polyutil_get_zoom_level($weight).
   *
   * @param int $weight
   *   Weight for setting.
   *
   * @return object
   *   $this.
   */
  public function setWeight($weight) {
    $this->weight = $weight;
    return $this;
  }

  /**
   * Place points in levels.
   *
   * former _gmap_polyutil_get_zoom_level($weight)
   *
   * Place points in levels based on their "weight" -- a value
   * derived from distance calculations in the
   * simplification algorithm, gmap_polyutil_dp_encode().
   *
   * @return int
   *   Zoom level.
   */
  public function getZoomLevel() {
    $i = 0;
    while (self::$levels[$i] > $this->weight) {
      $i++;
    }
    return self::GMAP_ZOOM_LEVELS - $i - 1;
  }

}
