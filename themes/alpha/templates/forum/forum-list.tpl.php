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
<div id="forum-<?php print $forum_id; ?>">
  <?php foreach ($forums as $child_id => $forum): ?>
    <div id="forum-list-<?php print $child_id; ?>" class="row <?php print $forum->zebra; ?>">
      <?php if ($forum->is_container) : ?>
        <div class="container col-xs-12">
          <?php print str_repeat('<div class="indent">', $forum->depth); ?>
            <?php if($forum->awesome_icon): ?> 
              <div class="awesome_icon">
                <?php print $forum->awesome_icon; ?>
              </div>
            <?php endif; ?> 
            <div class="icon forum-status-<?php print $forum->icon_class; ?>" title="<?php print $forum->icon_title; ?>">
              <span class="element-invisible"><?php print $forum->icon_title; ?></span>
            </div>
            <div class="name"><a href="<?php print $forum->link; ?>"><?php print $forum->name; ?></a></div>
            <?php if ($forum->description): ?>
              <div class="description"><?php print $forum->description; ?></div>
            <?php endif; ?>
          <?php print str_repeat('</div>', $forum->depth); ?>
        </div>
      <?php else: ?>        
        <div class="forum col-xs-12 col-sm-9 col-md-10">
          <?php print str_repeat('<div class="indent">', $forum->depth); ?>
            <?php if($forum->awesome_icon): ?> 
              <div class="awesome_icon">
                <?php print $forum->awesome_icon; ?>
              </div>
            <?php endif; ?> 
            <div class="icon forum-status-<?php print $forum->icon_class; ?>" title="<?php print $forum->icon_title; ?>">
              <span class="element-invisible"><?php print $forum->icon_title; ?></span>
            </div>
            <div class="name"><a href="<?php print $forum->link; ?>"><?php print $forum->name; ?></a></div>
            <?php if ($forum->description): ?>
              <div class="description"><?php print $forum->description; ?></div>
            <?php endif; ?>
          <?php print str_repeat('</div>', $forum->depth); ?>
        </div>
        <div class="details col-xs-11 col-sm-3 col-md-2 col-xs-offset-1 col-sm-offset-0">
          <div class="topics">
            <i class="fa fa-envelope-o"></i>
            <?php print $forum->num_topics ?>
            <?php if ($forum->new_topics): ?>
              <span class="new_replies">
              <i class="fa fa-envelope"></i>&nbsp;<a href="<?php print $forum->new_url; ?>"><?php print $forum->new_topics; ?></a>
              </span>
            <?php endif; ?>
          </div>
          <div class="posts"><i class="fa fa-comments-o"></i>&nbsp;<?php print $forum->num_posts ?></div>
          <div class="last-reply"><i class="fa fa-history"></i> <?php print $forum->time ?></div>
        </div>
      <?php endif; ?>
    </div>
  <?php endforeach; ?>
</div>
