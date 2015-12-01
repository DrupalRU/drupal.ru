<?php

/**
 * @file
 * Displays an appropriate icon for a forum post.
 *
 * Available variables:
 * - $new_posts: Indicates whether or not the topic contains new posts.
 * - $icon_class: The icon to display. May be one of 'hot', 'hot-new', 'new',
 *   'default', 'closed', or 'sticky'.
 * - $first_new: Indicates whether this is the first topic with new posts.
 *
 * @see template_preprocess_forum_icon()
 * @see theme_forum_icon()
 *
 * @ingroup themeable
 */
?>
<?php if ($first_new): ?>
  <a id="new"></a>
<?php endif; ?>
<?php
  switch($icon_class){
    case 'new':
        print '<i class="fa fa-envelope"></i>';
      break;

    case 'hot-new':
    case 'hot':
        print '<i class="fa fa-fire"></i>';
      break;

    case 'closed':
        print '<i class="fa fa-lock"></i>';
      break;

    case 'sticky':
        print '<i class="fa fa-flag"></i>';
      break;

    default:
      print '<i class="fa fa-envelope-o"></i>';

  }
?>
