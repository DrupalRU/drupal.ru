<div class="container">
  <div class="col-xs-12 col-md-4">
    <h2>Избранное, Новости, Важное</h2>
      <center class="fontpageicon">
      <i class="fa fa-bullhorn"></i>
    </center>
  </div>
  <div class="col-xs-12 col-md-8">
    <div class="frontpage-list clearfix">
      <?php print render($content['nodes']); ?>
      <div class="clearfix"></div>
      <div class="actions">
        <?php print render($content['links']); ?>
      </div>
    </div>
  </div>
</div>
