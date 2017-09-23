Drupal.behaviors.cache_router_switch = function() {
  $('form#cr-cache-switch select').change(function(){
    window.location = $(this).val();
  });
}