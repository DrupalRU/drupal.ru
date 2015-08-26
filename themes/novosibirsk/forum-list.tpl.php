<?php
// $Id: forum-list.tpl.php,v 1.4 2007/08/30 18:58:12 goba Exp $

/**
 * @file forum-list.tpl.php
 * Default theme implementation to display a list of forums and containers.
 *
 * Available variables:
 * - $forums: An array of forums and containers to display. It is keyed to the
 *   numeric id's of all child forums and containers.
 * - $forum_id: Forum id for the current forum. Parent to all items within
 *   the $forums array.
 *
 * Each $forum in $forums contains:
 * - $forum->is_container: Is TRUE if the forum can contain other forums. Is
 *   FALSE if the forum can contain only topics.
 * - $forum->depth: How deep the forum is in the current hierarchy.
 * - $forum->zebra: 'even' or 'odd' string used for row class.
 * - $forum->name: The name of the forum.
 * - $forum->link: The URL to link to this forum.
 * - $forum->description: The description of this forum.
 * - $forum->new_topics: True if the forum contains unread posts.
 * - $forum->new_url: A URL to the forum's unread posts.
 * - $forum->new_text: Text for the above URL which tells how many new posts.
 * - $forum->old_topics: A count of posts that have already been read.
 * - $forum->num_posts: The total number of posts in the forum.
 * - $forum->last_reply: Text representing the last time a forum was posted or
 *   commented in.
 *
 * @see template_preprocess_forum_list()
 * @see theme_forum_list()
 */
?>
<table id="forum-<?php print $forum_id; ?>">
  <thead>
    <tr>
      <th><?php print t('Forum'); ?></th>
      <th class="topics"><?php print t('Topics');?></th>
      <th class="posts"><?php print t('Posts'); ?></th>
      <th><?php print t('Last post'); ?></th>
    </tr>
  </thead>
  <tbody>
  <?php foreach ($forums as $child_id => $forum): ?>
    <tr id="forum-list-<?php print $child_id; ?>" class="<?php print $forum->zebra; ?>">
<?php
  if($forum->is_container) {
?>
  <td colspan="4" class="container">
<?php
  } else {

    if(!$forum->is_container) {
      $class = "-standart";
      if($forum->name == "Разработка и установка") {$class = "-1";}
      if($forum->name == "Разработка модулей") {$class = "-2";}
      if($forum->name == "Поиск специалистов и работы") {$class = "-3";}
      if($forum->name == "Обзоры и сравнения CMS") {$class = "-4";}
      if($forum->name == "Свободные программы") {$class = "-5";}
      if($forum->name == "Хостинг") {$class = "-6";}
      if($forum->name == "SEO") {$class = "-7";}
      if($forum->name == "Разное непонятное") {$class = "-8";}
      if($forum->name == "FAQ ") {$class = "-9";}
      if($forum->name == "Системное окружение") {$class = "-10";}
      if($forum->name == "Установка и настройка") {$class = "-11";}
      if($forum->name == "Решение проблем") {$class = "-12";}
      if($forum->name == "Безопасность") {$class = "-13";}
      if($forum->name == "Масштабируемость, нагрузка и быстродействие") {$class = "-14";}
      if($forum->name == "Выставка сайтов") {$class = "-15";}
      if($forum->name == "Программирование") {$class = "-16";}
      if($forum->name == "Дизайн и вёрстка") {$class = "-17";}
      if($forum->name == "Модераторский") {$class = "-18";}
      if($forum->name == "Сайт и проект Drupal.ru") {$class = "-19";}
      if($forum->name == "Создание документации WIKI.DRUPAL.RU") {$class = "-20";}
      if($forum->name == "Проекту нужна помощь") {$class = "-21";}
      if($forum->name == "Работа по переводам UI и документации") {$class = "-22";}
      if($forum->name == "Терминология") {$class = "-23";}
      if($forum->name == "Курилка") {$class = "-24";}
      if($forum->name == "Мусор") {$class = "-25";}
    }
?>

  <td class="forum<?php print $class; ?>">
<?php
  }
?>      
        <?php /* Enclose the contents of this cell with X divs, where X is the
               * depth this forum resides at. This will allow us to use CSS
               * left-margin for indenting.
               */ 
          $d = $forum->depth;
          if(!$forum->is_container && $d == 0) {$d = 1;}
        ?>
        <?php print str_repeat('<div class="indent">', $d); ?>
          <div class="name">
            <a href="<?php print $forum->link; ?>"><?php print $forum->name; ?></a>
          </div>
          <?php if ($forum->description): ?>
            <div class="description"><?php print $forum->description; ?></div>
          <?php endif; ?>
        <?php print str_repeat('</div>', $d); ?>
      </td>
      <?php if (!$forum->is_container): ?>
        <td class="topics">
          <?php print $forum->num_topics ?>
          <?php if ($forum->new_topics): ?>
            <br />
            <a href="<?php print $forum->new_url; ?>"><?php print $forum->new_text; ?></a>
          <?php endif; ?>
        </td>
        <td class="posts"><?php print $forum->num_posts ?></td>
        <td class="last-reply"><?php print $forum->last_reply ?></td>
      <?php endif; ?>
    </tr>
  <?php endforeach; ?>
  </tbody>
</table>
