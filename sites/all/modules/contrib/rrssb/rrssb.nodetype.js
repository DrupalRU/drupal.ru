(function ($) {

Drupal.behaviors.rrssbSettingsSummary = {
  attach: function (context) {
    $('fieldset#edit-rrssb', context).drupalSetSummary(function (context) {
      var vals = [];

      // Inclusion select field.
      if ($('#edit-show', context).is(':checked')) {
        vals.push(Drupal.t('Enabled'));
      }
      else {
        vals.push(Drupal.t('Disabled'));
      }
        
      return vals.join(', ');
    });
  }
};

})(jQuery);
