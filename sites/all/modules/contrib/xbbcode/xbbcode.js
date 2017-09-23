/**
 * @file
 * Add dynamic behavior to the xbbcode settings pages.
 */

(function ($) {
  Drupal.behaviors.xbbcode = {
    attach: function() {
      $('#xbbcode-handlers select.xbbcode-tag-handler').change(function() {
        var tag = this.name.match(/tags\[(.*?)\]\[module\]/)[1];
        var module = this.value;
        $('p.tag-' + tag).addClass('xbbcode-description-invisible');
        $('p.tag-' + tag + '.module-' + module).removeClass('xbbcode-description-invisible');
      });
    }
  };
})(jQuery);

