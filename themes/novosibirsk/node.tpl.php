<?php
// $Id$
if($teaser) {
?>
  <h2><?php print "<a href=\"" . base_path() . drupal_get_path_alias("node/" . $node->nid) . "\" title='Полная версия сообщения \"" . $title . "\"'>" . $title . "</a>"; ?></h2>
  <div class="ret">
  <?php
    if(module_exists('vote_up_down')) {
  ?>
    <p><a href="." title=""><img src="<?php print base_path() . path_to_theme(); ?>/img/ar_gn_up.gif" alt="полезно" /></a><a href="." title=""><img src="<?php print base_path() . path_to_theme(); ?>/img/ar_gr_dn.gif" alt="глупость" /></a><strong class="green"> 12 </strong><strong> балов</strong></p>
  <?php
    }
  ?>
    <p>Прислано: <a href="<?php print base_path() . drupal_get_path_alias("user/" . $node->uid); ?>" title="Профиль автора сообщения"><?php print $node->name; ?></a></p><p class="formatdate"><?php print format_date($node->created); ?></p>
  </div>
  <?php
    if($terms != "") {
  ?>
    <div class="dr">Другие статьи по теме: <?php print $terms; ?></div>
  <?php
    } else {print "<br>";}
  ?>
  <p><?php print $content; ?></p>    
  <?php if ($links) {?>
    <div class="links-bottom">      
        <?php print $links ?>
      <p class="next"><a href="<?php print base_path() . drupal_get_path_alias("node/" . $node->nid); ?>" title='Полная версия сообщения " <?php print $title; ?>"'>Читать весь текст &raquo;</a></p>
    </div>
  <?php 
    } else {
  ?>
    <p class="next"><a href="<?php print base_path() . drupal_get_path_alias("node/" . $node->nid); ?>" title='Полная версия сообщения " <?php print $title; ?>"'>Читать весь текст &raquo;</a></p>    
    
  <?php
    }
} else {
  global $user;
  drupal_add_js(path_to_theme() . '/comment.js', 'theme', 'header');
?>
  <strong><h1><?php print $title; ?></h1></strong>
  <div class="ret">
  <?php
    if(module_exists('vote_up_down')) {
  ?>
    <p><a href="." title=""><img src="<?php print base_path() . path_to_theme(); ?>/img/ar_gn_up.gif" alt="полезно" /></a><a href="." title=""><img src="<?php print base_path() . path_to_theme(); ?>/img/ar_gr_dn.gif" alt="глупость" /></a><strong class="green"> 12 </strong><strong> балов</strong></p>
  <?php
    }
  ?>
    <p>Прислано: <a href="<?php print base_path() . drupal_get_path_alias("user/" . $node->uid); ?>" title="Профиль автора сообщения"><?php print $node->name; ?></a></p><p class="formatdate"><?php print format_date($node->created); ?></p>
  </div>
  <?php
    if($terms != "") {
  ?>
    <div class="dr">Другие статьи по теме: <?php print $terms; ?></div>
  <?php
    } else {print "<br>";}
  ?>
  <p><?php print $content; ?></p>  
  <?php if ($links) {?>
    <div class="links-bottom"><?php print $links ?><?php if(user_access('post comments without approval', $user)) { ?><img src="<?php print base_path() . path_to_theme(); ?>/img/icons/blog_pencil.gif" id="img_quote" title="Выделите текст и нажмите эту кнопку, чтобы вставить цитату" onmouseover="getText('<?php print $node->name; ?>');" onclick="insertQuote();"><?php } ?></div>
  <?php } ?>

<?php
}
?>