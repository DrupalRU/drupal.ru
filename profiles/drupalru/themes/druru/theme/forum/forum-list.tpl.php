<?php

/**
 * @file
 * Displays a list of forums and containers.
 *
 * Available variables:
 * - $forums: An array of forums and containers to display. It is keyed to the
 *   numeric IDs of all child forums and containers. Each $forum in $forums
 *   contains:
 *   - $forum->is_container: TRUE if the forum can contain other forums. FALSE
 *     if the forum can contain only topics.
 *   - $forum->depth: How deep the forum is in the current hierarchy.
 *   - $forum->zebra: 'even' or 'odd' string used for row class.
 *   - $forum->icon_class: 'default' or 'new' string used for forum icon class.
 *   - $forum->icon_title: Text alternative for the forum icon.
 *   - $forum->name: The name of the forum.
 *   - $forum->link: The URL to link to this forum.
 *   - $forum->description: The description of this forum.
 *   - $forum->new_topics: TRUE if the forum contains unread posts.
 *   - $forum->new_url: A URL to the forum's unread posts.
 *   - $forum->new_text: Text for the above URL, which tells how many new posts.
 *   - $forum->old_topics: A count of posts that have already been read.
 *   - $forum->num_posts: The total number of posts in the forum.
 *   - $forum->last_reply: Text representing the last time a forum was posted or
 *     commented in.
 * - $forum_id: Forum ID for the current forum. Parent to all items within the
 *   $forums array.
 *
 * @see template_preprocess_forum_list()
 * @see theme_forum_list()
 *
 * @ingroup themeable
 */
?>
<div id="forum-<?php print $forum_id; ?>" class="clearfix forum">
  <?php foreach ($forums as $child_id => $forum): ?>
    <div id="forum-list-<?php print $child_id; ?>"
         class=" <?php print $forum->zebra; ?> <?php print ($forum->is_container ? 'parent' : 'child'); ?>">

      <?php if ($forum->is_container) : ?>

        <div class="col-xs-12 row-container">
          <?php print str_repeat('<div class="indent">', $forum->depth); ?>

            <h2><?php print $forum->linkable_title ?></h2>

            <?php if (!empty($forum->description)): ?>
              <small class="text-muted">
                <?php print $forum->description; ?>
              </small>
            <?php endif; ?>

          <?php print str_repeat('</div>', $forum->depth); ?>
        </div>

      <?php else: ?>

        <?php print str_repeat('<div class="indent">', $forum->depth); ?>
          <div class="row-container col-sm-9 col-md-10">

            <h3><?php print $forum->linkable_title ?></h3>

            <?php if ($forum->description): ?>
              <small class="text-muted">
                <?php print $forum->description; ?>
              </small>
            <?php endif; ?>
          </div>

          <div class="row-container details col-sm-3 col-md-2 text-color-default text-right">
            <div class="posts detail">
              <i class="fa fa-file-o"></i>
              <?php print $forum->num_topics ?>
              <?php if ($forum->new_topics): ?>
                <span class="new_replies detail">
                  <a href="<?php print $forum->new_url; ?>">
                    <i class="fa fa-file"></i>
                    <?php print $forum->new_topics; ?>
                  </a>
                </span>
              <?php endif; ?>
            </div>

            <div class="comments detail">
              <i class="fa fa-commenting-o"></i> <?php print $forum->num_posts ?>
            </div>

            <div class="last-reply detail">
              <i class="fa fa-clock-o"></i> <?php print $forum->last_reply ?>
            </div>


          </div>
        <?php print str_repeat('</div>', $forum->depth); ?>
      <?php endif; ?>

    </div>
  <?php endforeach; ?>
</div>
