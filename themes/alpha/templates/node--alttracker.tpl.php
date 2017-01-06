<div id="node-<?php
/**
 * @file
 */
print $node->nid; ?>"  class="row <?php print print $classes;?>"<?php print $attributes; ?>>
  <div class="title col-xs-12 col-sm-6 col-md-6">
    <div class="icon col-xs-1">
      <?php
        $flag = '<i class="fa fa-envelope-o"></i>';
        $history = _forum_user_last_visit($node->nid);
        if($node->last_comment_timestamp > $history){
          $flag = '<i class="fa fa-envelope"></i>';
        }
        if($sticky){
          $flag = '<i class="fa fa-flag"></i>';
        }
        if($promote){
          $flag = '<i class="fa fa-star"></i>';
        }

        print $flag . ' ';
      ?>
    </div>
    <div class="col-xs-10 title">
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
      <?php print render($content);?>
    </div>
  </div>
  <div class="author col-xs-4 col-sm-2 col-md-2 col-xs-offset-1 col-sm-offset-0">
    <i class="fa fa-user"></i>
    <?php print $name; ?>
  </div>
  <div class="replies col-xs-3 col-sm-2 col-md-2">
    <i class="fa fa-comments-o"></i>
    <?php print $comment_count; ?>
    <?php if ($node->new_replies): ?>
      <span class="new_replies">
        <a href="<?php print $node->new_url; ?>"><i class="fa fa-comment"></i>&nbsp;<?php print $node->new_replies; ?></a>
      </span>
    <?php endif; ?>
  </div>
  <div class="col-xs-4 col-sm-2 col-md-2 last-reply"><i class="fa fa-history"></i> <?php print $date; ?></div>
</div>
