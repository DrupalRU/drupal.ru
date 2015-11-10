/**
 *
 */

(function ($) {

Drupal.wysiwyg.plugins['quote'] = {

  /**
   * Execute the button.
   */
  invoke: function(data, settings, instanceId) {
    Drupal.wysiwyg.instances[instanceId].insert('[quote]' + data.content + '[/quote]');
  }
};

})(jQuery);