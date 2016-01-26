<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>  col-xs-12 col-sm-3"<?php print $attributes; ?>>

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
