(function ($) {

Drupal.behaviors.quote = {
  attach: function(context, settings) {
    var level = Drupal.settings.quote_nest - 1;
    if (level >= 0) {
      var top = $('blockquote.quote-nest-1', context);
      $('blockquote.quote-msg:eq(' + level + ')', top)
      .hide()
      .after('<div class="quote-snip">' + Drupal.t('<a href="#">[snip]</a>') + '</div>')
      .next('.quote-snip')
      .children('a')
      .click(function(e) {
        $(this).parent().siblings('.quote-msg').toggle();
        e.preventDefault();
      });
    }
  }
};

}(jQuery));
