<?php
/**
 * @file
 * GD2 toolkit for image manipulation within Textimage.
 */

/**
 * Return the path of the font file, in a format usable by GD.
 */
function image_gd_textimage_get_font_path($image, $data = array()) {
  $font_wrapper = file_stream_wrapper_get_instance_by_uri($data['font_uri']);
  if ($font_wrapper instanceof DrupalLocalStreamWrapper) {
    return $font_wrapper->realpath();
  }
  else {
    return is_file($data['font_uri']) ? $data['font_uri'] : NULL;
  }
}

/**
 * Creates a new image resource and overlays the text over it.
 */
function image_gd_textimage_text_to_image($image, $data = array()) {

  // Create the image resource, fill transparent.
  $new_image = image_toolkit_invoke('textimage_create_transparent', $image, array(
      $image->info['width'],
      $image->info['height'],
      $data['gif_transparency_color'],
    )
  );
  if ($new_image) {
    $image->resource = $new_image->resource;
  }
  else {
    return FALSE;
  }

  // Draw and fill the outer text box, if required.
  if ($data['layout']['background_color']) {
    _textimage_draw_rectangle($image, $data['outer_box'], $data['layout']['background_color']);
  }

  // In debug mode, visually display the text boxes.
  if ($data['debug_visuals']) {
    _textimage_draw_box($image, $data['inner_box'], $data['layout']['background_color'], TRUE);
    _textimage_draw_box($image, $data['outer_box'], $data['layout']['background_color'], TRUE);
    // Wrapper.
    _textimage_draw_box(
      $image,
      array(
        0, 0,
        $image->info['width'] - 1, 0,
        $image->info['width'] - 1, $image->info['height'] - 1,
        0, $image->info['height'] - 1,
      ),
      '#000000'
    );
  }

  // Foreground text color.
  $foreground_color = image_toolkit_invoke('textimage_imagecolor', $image, array(array('rgba' => $data['font']['color'])));

  // Determine if outline/shadow is required.
  $outline = $shadow = FALSE;
  if ($data['font']['stroke_mode'] == 'outline' && ($data['font']['outline_top'] || $data['font']['outline_right'] || $data['font']['outline_bottom'] || $data['font']['outline_left']) && $data['font']['stroke_color']) {
    $outline = TRUE;
  }
  elseif ($data['font']['stroke_mode'] == 'shadow' && ($data['font']['shadow_x_offset'] || $data['font']['shadow_y_offset'] || $data['font']['shadow_width'] || $data['font']['shadow_height']) && $data['font']['stroke_color']) {
    $shadow = TRUE;
  }

  // Process each of the text lines.
  $current_y = 0;
  foreach ($data['text_lines'] as $text_line) {

    // This text line's box size.
    $text_line_box = _textimage_get_bounding_box($image, $text_line, 1, $data['font']['size'], $data['font']['uri']);
    $text_line_box->set('height', $data['line_height']);

    // Manage text alignment within the line.
    $x_delta = $data['inner_width'] - $text_line_box->get('width');
    $current_y += $data['line_height'];
    switch ($data['text']['align']) {
      case 'center':
        $x_offset = round($x_delta / 2);
        break;

      case 'right':
        $x_offset = $x_delta;
        break;

      case 'left':
      default:
        $x_offset = 0;
        break;

    }

    // Get details for the rotated/translated text line box.
    $text_line_box_t = $text_line_box->getTranslatedBox(
      $data['font']['angle'],
      array(
        $data['layout']['padding_left'] + $x_offset,
        $data['layout']['padding_top'] + $current_y - $data['line_height'],
      ),
      $data['topLeftCornerPosition']
    );
    list($x_pos, $y_pos) = $text_line_box_t->get('basepoint');

    // Overlays the text outline/shadow, if required.
    if ($outline || $shadow) {
      $stroke_color = image_toolkit_invoke('textimage_imagecolor', $image, array(array('rgba' => $data['font']['stroke_color'])));
      if ($outline) {
        $stroke_x_pos = $x_pos;
        $stroke_y_pos = $y_pos;
        $stroke_top = $data['font']['outline_top'];
        $stroke_right = $data['font']['outline_right'];
        $stroke_bottom = $data['font']['outline_bottom'];
        $stroke_left = $data['font']['outline_left'];
      }
      elseif ($shadow) {
        $stroke_x_pos = $x_pos + $data['font']['shadow_x_offset'];
        $stroke_y_pos = $y_pos + $data['font']['shadow_y_offset'];
        $stroke_top = 0;
        $stroke_right = $data['font']['shadow_width'];
        $stroke_bottom = $data['font']['shadow_height'];
        $stroke_left = 0;
      }
      $data_stroke = array(
        'size'        => $data['font']['size'],
        'angle'       => -$data['font']['angle'],
        'fontfile'    => $data['font']['uri'],
        'text'        => $text_line,
        'x'           => $stroke_x_pos,
        'y'           => $stroke_y_pos,
        'textcolor'   => $foreground_color,
        'strokecolor' => $stroke_color,
        'top'         => $stroke_top,
        'right'       => $stroke_right,
        'bottom'      => $stroke_bottom,
        'left'        => $stroke_left,
      );
      image_toolkit_invoke('textimage_text_stroke', $image, array($data_stroke));
    }

    // Overlays the text.
    imagettftext(
      $image->resource,
      $data['font']['size'],
      -$data['font']['angle'],
      $x_pos,
      $y_pos,
      $foreground_color,
      $data['font']['uri'],
      $text_line
    );

    // In debug mode, display a polygon enclosing the text line.
    if ($data['debug_visuals']) {
      _textimage_draw_debug_box($image, $text_line_box_t, $data['layout']['background_color'], TRUE);
    }

    // Add interline spacing (leading) before next iteration.
    $current_y += $data['text']['line_spacing'];
  }

  // Finalise image.
  imagealphablending($image->resource, TRUE);
  imagesavealpha($image->resource, TRUE);

  return TRUE;
}

/**
 * Return the bounding box of a text using TrueType fonts.
 */
function image_gd_textimage_get_bounding_box($image, $data = array()) {

  $box = new TextimageTextbox();

  // Need to calculate the height independently from primitive as
  // lack of descending/ascending characters will limit the height.
  // So to have uniformity we take a dummy string with ascending and
  // descending characters to set to max height possible.
  $box->set('points', imagettfbbox($data['size'], 0, $data['fontfile'], 'bdfhkltgjpqyBDFHKLTGJPQY§@çÀÈÉÌÒÇ'));
  $height = $data['lines'] * $box->get('height');

  // Now get the box for full text to get width.
  $box->set('points', imagettfbbox($data['size'], 0, $data['fontfile'], $data['text']));

  // Reset height.
  $box->set('height', $height);

  // Rotate if angle specified.
  if ($data['angle']) {
    $box = $box->getTranslatedBox($data['angle']);
  }

  return $box;

}

/**
 * Writes the outline/shadow of a given text into the image.
 *
 * Credit to John Ciacia.
 *
 * @param object $image
 *   Image
 * @param array $data
 *   Effect data, an array:
 *    size - font size
 *    angle - angle in degrees to rotate the text
 *    fontfile - file path of TrueType font to use
 *    text - The text string in UTF-8 encoding
 *    x - Upper left corner of the text
 *    y - Lower left corner of the text
 *    strokecolor - the rgba color of the text border
 *    top, right, bottom, left - number of pixels of the text border
 *      on each side of the text
 *
 * @see http://www.johnciacia.com/2010/01/04/using-php-and-gd-to-add-border-to-text/
 */
function image_gd_textimage_text_stroke($image, $data = array()) {
  for ($c1 = ($data['x'] - abs($data['left'])); $c1 <= ($data['x'] + abs($data['right'])); $c1++) {
    for ($c2 = ($data['y'] - abs($data['top'])); $c2 <= ($data['y'] + abs($data['bottom'])); $c2++) {
      $bg = imagettftext(
        $image->resource,
        $data['size'],
        $data['angle'],
        $c1,
        $c2,
        $data['strokecolor'],
        $data['fontfile'],
        $data['text']
      );
      if ($bg == FALSE) {
        return FALSE;
      }
    }
  }
  return TRUE;
}

/**
 * Draw a polygon.
 *
 * @param object $image
 *   Image
 * @param array $data
 *   Effect data, an array:
 *    points - the polygon point coordinates array
 *    num_points - the number of polygon points
 *    fill_color - the rgba color of the polygon fill
 *    border_color - the rgba color of the polygon line
 */
function image_gd_textimage_draw_polygon($image, $data = array()) {
  if ($data['fill_color']) {
    $color = image_toolkit_invoke('textimage_imagecolor', $image, array(array('rgba' => $data['fill_color'])));
    $success = imagefilledpolygon($image->resource, $data['points'], $data['num_points'], $color);
    if (!$success) {
      return FALSE;
    }
  }
  if ($data['border_color']) {
    $color = image_toolkit_invoke('textimage_imagecolor', $image, array(array('rgba' => $data['border_color'])));
    $success = imagepolygon($image->resource, $data['points'], $data['num_points'], $color);
    if (!$success) {
      return FALSE;
    }
  }
}

/**
 * Draw a line.
 */
function image_gd_textimage_draw_line($image, $data = array()) {
  $color = image_toolkit_invoke('textimage_imagecolor', $image, array(array('rgba' => $data['rgba'])));
  return imageline($image->resource, $data['a'][0], $data['a'][1], $data['b'][0], $data['b'][1], $color);
}

/**
 * Draw an ellipse.
 */
function image_gd_textimage_draw_ellipse($image, $data = array()) {
  $color = image_toolkit_invoke('textimage_imagecolor', $image, array(array('rgba' => $data['rgba'])));
  return imagefilledellipse($image->resource, $data['c'][0], $data['c'][1], $data['width'], $data['height'], $color);
}

/**
 * Get a GD imagecolor.
 */
function image_gd_textimage_imagecolor($image, $data = array()) {
  if ($image->info['mime_type'] == 'image/png') {
    list($r, $g, $b, $alpha) = array_values(imagecache_actions_hex2rgba($data['rgba']));
    return imagecolorallocatealpha($image->resource, $r, $g, $b, $alpha);
  }
  else {
    list($r, $g, $b) = array_values(imagecache_actions_hex2rgba($data['rgba']));
    return imagecolorallocate($image->resource, $r, $g, $b);
  }
}

/**
 * Create a truecolor transparent image.
 *
 * If file type does not allow transparency, the image will be filled in
 * white. For .gif files, takes transparency from the original image resource
 * if available and no transparent color is specified in input.
 *
 * @param object $image
 *   An image object.
 * @param int $width
 *   The width of the image, in pixels.
 * @param int $height
 *   The height of the image, in pixels.
 * @param array $transparent
 *   The transparent color array red/blue/green for gif transparency.
 *
 * @return resource
 *   A GD image handle.
 */
function image_gd_textimage_create_transparent(stdClass $image, $width, $height, $transparent = NULL) {
  $res = imagecreatetruecolor($width, $height);
  if ($image->info['mime_type'] == 'image/png') {
    imagealphablending($res, FALSE);
    $transparency = imagecolorallocatealpha($res, 0, 0, 0, 127);
    imagefill($res, 0, 0, $transparency);
    imagealphablending($res, TRUE);
    imagesavealpha($res, TRUE);
  }
  elseif ($image->info['mime_type'] == 'image/gif') {
    if (empty($transparent)) {
      // Grab transparent color index from image resource.
      if (isset($image->resource) && $transparent = imagecolortransparent($image->resource) >= 0) {
        // The original has a transparent color, allocate to the new image.
        $transparent_color = imagecolorsforindex($image->resource, $transparent);
        $transparent = imagecolorallocate($res, $transparent_color['red'], $transparent_color['green'], $transparent_color['blue']);
      }
      else {
        // No incoming image or no transparency channel, no color specified,
        // fill white.
        $transparent = imagecolorallocate($res, 255, 255, 255);
      }
    }
    else {
      // Get transparent from the input argument.
      $transparent = imagecache_actions_hex2rgba($transparent);
      $transparent = imagecolorallocate($res, $transparent['red'], $transparent['green'], $transparent['blue']);
    }
    // Flood with our transparent color.
    if ($transparent >= 0) {
      imagefill($res, 0, 0, $transparent);
      imagecolortransparent($res, $transparent);
    }
  }
  else {
    imagefill($res, 0, 0, imagecolorallocate($res, 255, 255, 255));
  }

  $new_image = clone $image;
  $new_image->info['width'] = $width;
  $new_image->info['height'] = $height;
  $new_image->resource = $res;
  return $new_image;
}

/**
 * Set a color to transparent.
 *
 * Mainly to support .gif files transparency.
 *
 * @param object $image
 *   An image object.
 * @param array $color
 *   The transparent color array red/blue/green for .gif transparency.
 */
function image_gd_textimage_set_transparency(stdClass $image, $color) {
  if ($color) {
    list($r, $g, $b) = array_values(imagecache_actions_hex2rgba($color));
    $transparent = imagecolorallocate($image->resource, $r, $g, $b);
    imagecolortransparent($image->resource, $transparent);
  }
}
