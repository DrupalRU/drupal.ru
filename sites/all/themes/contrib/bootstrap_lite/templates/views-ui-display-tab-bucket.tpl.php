<?php
/**
 * @file
 * Template for each "box" on the display query edit screen.
 */
?>
<div class="<?php print implode(' ', $classes); ?> panel panel-default"<?php print backdrop_attributes($attributes); ?>>
  <?php if (!empty($title) or !empty($actions)) : ?>
  <div class="panel-heading">
  <?php endif; ?>  
  <?php if (!empty($title)) : ?>
    <h3 class="panel-title"><?php print $title; ?></h3>
  <?php endif; ?>
  <?php print $item_help_icon; ?>
  <?php if(!empty($actions)) : ?>
    <?php print $actions; ?>
  <?php endif; ?>
  <?php if (!empty($title) or !empty($actions)) : ?>
  </div>
  <?php endif; ?>
  <div class="panel-body">
  <?php print $content; ?>
  </div>
</div>
