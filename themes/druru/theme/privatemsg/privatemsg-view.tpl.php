<?php
/**
 * @file
 * Template for private message view page.
 */

?>
<article <?php print drupal_attributes(array('class' => $message_classes)); ?> id="privatemsg-mid-<?php print $mid; ?>">

  <?php if ('left' == $author_place): ?>
    <div class="media-left">
      <div class="placeholder"></div>
      <?php if ($show_picture): ?>
        <?php print $author_picture; ?>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <div class="media-body <?php print (!empty($new) ? 'new' : ''); ?>">
    <h4 class="media-heading">
      <?php print $author_name_link; ?>
      <small><?php print $message_timestamp; ?></small>
    </h4>

    <?php print $anchors; ?>

    <div class="content">
      <?php print $message_body; ?>

      <?php if (isset($message_actions)): ?>
        <div class="pull-right">
          <?php print $message_actions ?>
        </div>
      <?php endif; ?>
    </div>
  </div>

  <?php if ('right' == $author_place): ?>
    <div class="media-right">
      <div class="placeholder"></div>
      <?php if ($show_picture): ?>
        <?php print $author_picture; ?>
      <?php endif; ?>
    </div>
  <?php endif; ?>

</article>

