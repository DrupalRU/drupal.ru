<article class="media comment <?php if ($new) print "bg-info" ?> <?php if ( !empty($message_classes)) { echo implode(' ', $message_classes);} ?> clearfix"  id="privatemsg-mid-<?php print $mid; ?>">
  <?php if($author_picture): ?>
  <div class="pull-left">
    <?php print $author_picture; ?>
  </div>
  <?php endif; ?>  
  <div class="media-body">
    <span class="text-muted pull-right">
        <small class="text-muted"><?php print $message_timestamp; ?></small>
    </span>
    <strong class="text-success"><?php print $author_name_link; ?></strong>
    <?php print $anchors; ?>
    <div class="content">
      <?php print $message_body; ?>
    </div>
    <?php if (isset($message_actions)): ?>
      <div class="pull-right">
      <?php print $message_actions ?>
      </div>
    <?php endif; ?>
  </div>
</article>
