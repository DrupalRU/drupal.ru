<div class="frontpage-list clearfix">
  <h2><?php print 'Интересное'; ?></h2>

  <div class="list-group">
    <?php print render($content['nodes']); ?>
  </div>
  <div class="form-actions">
    <?php print render($content['links']); ?>
  </div>
</div>

