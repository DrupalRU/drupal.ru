<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
  <?php print render($title_suffix); ?>
  
  <div class="image">
  <?php print render($content['event_image']); ?>
  </div>
  
  <div class="date_time">
    <?php print render($content['datetime']); ?>
  </div>
  
  <?php if (isset($content['url'])): ?>
    <div class="website">
      <?php print render($content['url']); ?>
    </div>
  <?php endif; ?>

  <?php if (isset($content['address'])): ?>
    <div class="address">
      <?php print render($content['address']); ?>
    </div>
  <?php endif; ?>

  <div class="event-type">
    <?php print render($content['simple_event_type']); ?>
  </div>

  
</div>
