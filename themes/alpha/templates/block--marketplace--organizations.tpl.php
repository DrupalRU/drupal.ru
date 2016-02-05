<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <div class="container">
    <div class="col-xs-12 col-md-4 pull-right">
      <center class="fontpageicon">      
        <?php print render($title_prefix); ?>
        <?php if ($block->subject): ?>
          <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
        <?php endif;?>
        <?php print render($title_suffix); ?>
        <i class="fa fa-suitcase"></i>
      </center>
    </div>
    <div class="col-xs-12 col-md-8">
      <div class="content"<?php print $content_attributes; ?>>
        <?php print $content ?>
      </div>
    </div>
  </div>
</div>