<?php
/**
 * @file comment.tpl.php
 */
?>
<div class="comment">
  <div class="autor">
    <div><?php print $submitted; ?></div>
    <?php
      if(module_exists('vote_up_down')) {
    ?>
      <p><a href="." title=""><img src="<?php print base_path() . path_to_theme(); ?>/img/ar_gn_up.gif" alt="полезно" /></a><a href="." title=""><img src="<?php print base_path() . path_to_theme(); ?>/img/ar_gr_dn.gif" alt="глупость" /></a><strong class="green"> 12 </strong><strong> балов</strong></p>
    <?php
      }
    ?>
  </div>
<?php 
  if ($comment->new) {
?>
    <span class="new"><?php print drupal_ucfirst($new) ?></span>
<?php 
  }
?>
<?php if($picture) { ?><div class="avatar"><a href="<?php print base_path() . drupal_get_path_alias("user/" . $comment->uid); ?>" title="Профиль автора сообщения" title=""><?php print $picture; ?></a></div> <?php } ?>
 
    <p><?php print $content; ?></p>
<?php 
  if ($signature) {
?>
      <div class="clear-block">
        <div>—</div>
        <?php print $signature; ?>
      </div>
<?php 
  }
?>
<br class="clear" />
  <div class="abuse"><?php print $links ?>
<?php 
  if(user_access('post comments without approval')) {
?>
  <img src="<?php print base_path() . path_to_theme(); ?>/img/icons/blog_pencil.gif" id="img_quote" title="Выделите текст и нажмите эту кнопку, чтобы вставить цитату" onmouseover="getText('<?php print $comment->name; ?>');" onclick="insertQuote();">
<?php 
  }
?>
  </div>
</div>