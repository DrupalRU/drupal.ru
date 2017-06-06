<div id="node-<?php print $node->nid; ?>"  class="row  <?php if(isset($classes)): print $classes; endif;?> <?php print $zebra;?> "<?php if(isset($attributes)): print $attributes; endif;?>>
  <div class="col-xs-2 col-sm-1 image">
    <?php print render($content['event_image']); ?>
  </div>
  
    <div class="col-xs-10 col-sm-7 title">
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
      <?php if (isset($content['address'])): ?>
        <div class="address">
          <?php print render($content['address']); ?>
        </div>
      <?php endif; ?>
      <div class="event-type">
        <?php print render($content['simple_event_type']); ?>
      </div>
    </div>

  <div class="col-xs-10 col-xs-offset-2 col-sm-offset-0 col-sm-4 date_time">
    <?php print render($content['datetime']); ?>
  </div>
</div>
  
  