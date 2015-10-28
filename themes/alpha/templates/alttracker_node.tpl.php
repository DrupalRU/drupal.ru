<div id="node-<?php print $node->nid; ?>"  class="row <?php  print $classes;?> <?php print $zebra;?> "<?php print $attributes; ?>>
  <div class="title col-xs-12 col-sm-6 col-md-6">
    <div class="icon col-xs-1">
      <?php print $icon; ?>
    </div>
    <div class="col-xs-10 title">
      <a href="<?php print $node->url; ?>"><?php print $title; ?></a>
      <?php if($term): ?>
      <div class="terms">
        <?php print $term; ?>
      </div>
      <?php endif; ?>
    </div>
  </div>
  <div class="author col-xs-4 col-sm-2 col-md-2 col-xs-offset-1 col-sm-offset-0">
    <i class="fa fa-user"></i>
    <?php print $name; ?>
  </div>
  <div class="replies col-xs-3 col-sm-2 col-md-2">
    <i class="fa fa-comments-o"></i>
    <?php print $node->comment_count; ?>
    <?php if ($node->new_replies): ?>
      <span class="new_replies">
        <a href="<?php print $node->url; ?>"><i class="fa fa-comment"></i>&nbsp;<?php print $node->new_replies; ?></a>
      </span>
    <?php endif; ?>
  </div>
  <div class="col-xs-4 col-sm-2 col-md-2 last-reply"><i class="fa fa-history"></i> <?php print $timeago; ?></div>
</div>
  
  