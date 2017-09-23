<?php

/**
 * Theme for dl list.
 * - items:
 * Example array of items:
 * array(
 *   array(
 *     'dt' => t('Some title'),
 *     'dd' => t('Some description'),
 *   ),
 *   array(
 *     'dt' => t('Some title 2'),
 *     'dd' => t('Some description 2'),
 *     'children' => array(
 *       array(
 *         'dt' => t('Some title'),
 *         'dd' => t('Some description'),
 *       ),
 *     ),
 *   ),
 *   array(
 *     'dt' => t('Some title 3'),
 *     'dd' => t('Some description 3'),
 *     'data-bind' => 'any string',
 *     'class' => array('some-class-one', 'some-class-two'),
 *   ),
 * );
 *
 * - horizontal creates horizontal list.
 * @link http://getbootstrap.com/css/#horizontal-description
 *
 *
 * @param $variables
 *
 * @return string
 * @throws \Exception
 */
function druru_dl_list($variables) {
  $title = $variables['title'];
  $items = $variables['items'];
  $attributes = $variables['attributes'];
  $is_horizontal = $variables['horizontal'];
  $output = '';

  if (isset($title)) {
    $output .= '<h3>' . $title . '</h3>';
  }
  if ($is_horizontal) {
    $attributes['class'][] = 'dl-horizontal';
  }
  $output .= "<dl" . drupal_attributes($attributes) . '>';
  $count_items = count($items);
  $classes = array(1 => 'first', $count_items => 'last');
  $i = 0;
  foreach ($items as $item) {
    $attributes = array();
    $children = array();
    $dt = '';
    $dd = '';
    $i++;
    if (is_array($item)) {
      foreach ($item as $key => $value) {
        switch ($key) {
          case 'dt':
            $dt = $value;
            break;

          case 'dd':
            $dd = $value;
            break;

          case 'children':
            $children = $value;
            break;

          default:
            $attributes[$key] = $value;
            break;
        }
      }
      if ($children) {
        // Render nested list.
        $dd .= theme('dl_list', $children);
      }
    }

    if (isset($classes[$i])) {
      $attributes['class'][] = $classes[$i];
    }
    $attributes = drupal_attributes($attributes);

    $output .= "<dt{$attributes}>{$dt}</dt>" . PHP_EOL;
    $output .= "<dd{$attributes}>{$dd}</dd>" . PHP_EOL;
  }
  $output .= "</dl>";
  return $output;
}
