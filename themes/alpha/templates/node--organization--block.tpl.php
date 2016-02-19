<div id="node-<?php
/**
 * @file
 */
print $node->nid; ?>" class="<?php print $classes; ?>  col-xs-12 col-sm-4"<?php print $attributes; ?>>
  <div class="name">
    <?php print render($title_prefix); ?>
    <h3<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h3>
    <?php print render($title_suffix); ?>
  </div>

  <div class="logo">
  <?php print render($content['logo']); ?>
  </div>
  
  <?php if ($content['website']): ?>
    <div class="website">
      <?php print render($content['website']); ?>
    </div>
  <?php endif; ?>

  <?php if ($content['address']): ?>
    <div class="address">
      <?php print render($content['address']); ?>
    </div>
  <?php endif; ?>

  <div class="organization-type">
    <?php print render($content['organization_type']); ?>
  </div>
  
</div>
