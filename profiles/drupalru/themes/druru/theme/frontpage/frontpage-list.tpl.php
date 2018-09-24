<div class="frontpage-list">
  <h2><?php print 'Интересное'; ?></h2>

  <div class="list-group">
    <?php print render($content['nodes']); ?>
  </div>

  <div class="actions">
    <ul class="list-inline"><li class="add first"><a href="/node/add/blog"> <i class="text-accent icon fa fa-plus" aria-hidden="true"></i> Создать</a></li>
        <li class="list last"><a href="/node"> <i class="text-accent icon fa fa-list" aria-hidden="true"></i> Еще</a></li>
    </ul>
  </div>
</div>

