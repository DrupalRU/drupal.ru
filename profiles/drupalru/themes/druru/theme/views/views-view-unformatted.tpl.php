<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
if (isset($view->display[$view->name]->display_plugin)):
  $display_mode = $view->display[$view->name]->display_plugin;
endif;
$is_page = !isset($display_mode) || 'page' != $display_mode;
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<?php
// This syntax used because need to exclude empty
// symbols from output for correct structure of layout.
foreach ($rows as $id => $row):
  if ($is_page):
    print '<div';
    if ($classes_array[$id]) :
      print ' class="' . $classes_array[$id] . '"';
    endif;
    print '>';
  endif;
  print $row;
  if ($is_page):
    print '</div>';

  endif;
endforeach; ?>
