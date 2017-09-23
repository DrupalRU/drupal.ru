<?php
/**
 * @file
 * Textimage - Font textbox management class.
 */

/**
 * Textimage - Font textbox management class.
 */
class TextimageTextbox {

  protected $points = array();
  protected $width = 0;
  protected $height = 0;
  protected $basepoint = array(0, 0);
  protected $topLeftCornerPosition = array(0, 0);

  /**
   * Property getter.
   *
   * @param string $property
   *   property name
   *
   * @return mixed
   *   property value
   */
  public function get($property) {
    return $this->$property;
  }

  /**
   * Property setter.
   *
   * @param string $property
   *   property name
   * @param mixed $value
   *   property value
   * @param array $options
   *   optional input
   */
  public function set($property, $value, $options = array()) {
    $this->$property = $value;
    switch ($property) {
      case 'width':
        $this->points[2] = $this->points[4] = $this->points[0] + $value - 1;
        break;

      case 'height':
        $this->points[1] = $this->points[3] = $this->points[7] + $value - 1;
        break;

      case 'points':
        $this->basepoint = array(0, 0);
        // Note: there is no check whether the points are actually representing
        // a rectangle.
        // Translate box to top left = 0,0.
        $this->translateBoxPoints(array(-$this->points[6], -$this->points[7]));
        // Set width and height.
        $this->width = $this->points[2] + 1;
        $this->height = $this->points[3] + 1;
        break;

    }
  }

  /**
   * Get a rotated box object.
   *
   * @param float $angle
   *   rotation angle
   * @param array $offset
   *   offset requested before rotation
   * @param array $top_left_corner_position
   *   position of top left corner for rotation
   */
  public function getTranslatedBox($angle, $offset = NULL, $top_left_corner_position = NULL) {
    $output = clone $this;
    if ($offset) {
      $output->translateBoxPoints($offset);
    }
    if ($angle) {
      $output->rotateBoxPoints($angle, $top_left_corner_position);
    }
    return $output;
  }

  /**
   * Rotate the box.
   *
   * @param float $angle
   *   rotation angle
   * @param array $top_left_corner_position
   *   box translation offset array (x, y) to reposition the box
   */
  protected function rotateBoxPoints($angle, $top_left_corner_position) {
    for ($i = 0; $i < 7; $i += 2) {
      list($this->points[$i], $this->points[$i + 1]) = self::rotatePoint($this->points[$i], $this->points[$i + 1], $angle, $top_left_corner_position);
    }
    list($this->basepoint[0], $this->basepoint[1]) = self::rotatePoint($this->basepoint[0], $this->basepoint[1], $angle, $top_left_corner_position);
    if (!$top_left_corner_position) {
      $min_x = min(
        array(
          $this->points[0],
          $this->points[2],
          $this->points[4],
          $this->points[6],
        )
      );
      $min_y = min(
        array(
          $this->points[1],
          $this->points[3],
          $this->points[5],
          $this->points[7],
        )
      );
      $max_x = max(
        array(
          $this->points[0],
          $this->points[2],
          $this->points[4],
          $this->points[6],
        )
      );
      $max_y = max(
        array(
          $this->points[1],
          $this->points[3],
          $this->points[5],
          $this->points[7],
        )
      );
      $this->width = $max_x - $min_x + 1;
      $this->height = $max_y - $min_y + 1;
      $this->topLeftCornerPosition = array(-$min_x,  -$min_y);
      $this->translateBoxPoints($this->topLeftCornerPosition);
    }
  }

  /**
   * Translate the box by an offset.
   *
   * @param array $offset
   *   offset array (x, y)
   */
  protected function translateBoxPoints($offset) {
    for ($i = 0; $i < 7; $i += 2) {
      $this->points[$i] += $offset[0];
      $this->points[$i + 1] += $offset[1];
    }
    $this->basepoint[0] += $offset[0];
    $this->basepoint[1] += $offset[1];
  }

  /**
   * Translate a point, by an offset and a rotation angle.
   *
   * @param int $x
   *   x coordinate of the point
   * @param int $y
   *   y coordinate of the point
   * @param float $angle
   *   rotation angle
   * @param array $offset
   *   offset array (x, y)
   *
   * @return array
   *   array with x,y translated coordinates
   */
  protected static function rotatePoint($x, $y, $angle, $offset = NULL) {
    $rad = deg2rad($angle);
    $sin = sin($rad);
    $cos = cos($rad);
    $tx = round($x * $cos + $y * -$sin) + ($offset ? $offset[0] : 0);
    $ty = round($y * $cos - $x * -$sin) + ($offset ? $offset[1] : 0);
    return array($tx, $ty);
  }

}
