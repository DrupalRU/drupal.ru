/*
 * Обработка событий формы: голосование, воздержание, отмена голосования и вспомогательная функция.
 */

(function ($) {
  /* Для совместимости JavaScript и jQuery */
})(jQuery);

function vote(v, n){
  basePath = Drupal.settings.basePath;

  if(!is_array(Drupal.settings.sPath))sPath = Drupal.settings.sPath;
  else sPath = Drupal.settings.sPath[0];

  d = "";

  for (i = 0; i < v.length; i++){
    if (v[i].checked==true){
      d += ";" + v[i].value;
    }
  }

  if(d != "") {
    jQuery('div#inner_poll_' + n).html(Drupal.t('Loading...'));

    // Drupal.detachBehaviors(jQuery);

    jQuery.ajax({
      type: "POST",
      url: "http://" + sPath + basePath + "inner_poll/vote_js",
      dataType: 'json',
      data: "v=" + d + "&n=" + n,
      success: function(data){ jQuery('div#inner_poll_' + n).replaceWith(data.result);},
      error: function(data){jQuery('div#inner_poll_' + n).replaceWith(data.result);}
    });
  }
}

function abst(n){
  basePath = Drupal.settings.basePath;

  if(!is_array(Drupal.settings.sPath))sPath = Drupal.settings.sPath;
  else sPath = Drupal.settings.sPath[0];

  jQuery('div#inner_poll_' + n).html(Drupal.t('Loading...'));

  // Drupal.detachBehaviors(jQuery);

  jQuery.ajax({
    type: "POST",
    url: "http://" + sPath + basePath + "inner_poll/vote_js",
    dataType: 'json',
    data: "abstain=1&n=" + n,
    success: function(data){jQuery('div#inner_poll_' + n).replaceWith(data.result);},
    error: function(data){jQuery('div#inner_poll_' + n).replaceWith(data.result);}
  });
}

function cancel_vote(n){
  basePath = Drupal.settings.basePath;

  if(!is_array(Drupal.settings.sPath))sPath = Drupal.settings.sPath;
  else sPath = Drupal.settings.sPath[0];

  jQuery('div#inner_poll_' + n).html(Drupal.t('Loading...'));

  // Drupal.detachBehaviors(jQuery);

  jQuery.ajax({
    type: "POST",
    url: "http://" + sPath + basePath + "inner_poll/cancel_vote",
    dataType: 'json',
    data: "n=" + n,
    success: function(data){jQuery('div#inner_poll_' + n).replaceWith(data.result);},
    error: function(data){jQuery('div#inner_poll_' + n).replaceWith(data.result);}
  });
}

function is_array(input) {
  return typeof(input)=='object'&&(input instanceof Array);
}
