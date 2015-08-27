function vote(v, n, block){
  block = block || 0;
  suffix = '';
  url_suffix = '';
  if(block != 0) {
    suffix = 'block_';
    url_suffix = '/block';
  }
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
		$('div#inner_poll_' + suffix + n).html('Loading');
  	$.ajax(
	  {
 	 		type: "POST",
		 		url: "http://" + sPath + basePath + "inner_poll/vote_js" + url_suffix,
		 		dataType: 'json',
		 		data: "v=" + d + "&n=" + n,
		 		success: function(data){$('div#inner_poll_' + suffix + n).replaceWith(data.result['#value']);},
		 		error: function(data){$('div#inner_poll_' + suffix  + n).replaceWith(data.result['#value']);}
	 	});
	}	
}

function abst(n, block){
  block = block || 0;
  suffix = '';
  url_suffix = '';
  if(block != 0) {
    suffix = 'block_';
    url_suffix = '/block';
  }
  basePath = Drupal.settings.basePath;
  if(!is_array(Drupal.settings.sPath))sPath = Drupal.settings.sPath;
  else sPath = Drupal.settings.sPath[0];
	$('div#inner_poll_' + suffix + n).html('Loading...');
 	$.ajax(
  {
	 		type: "POST",
	 		url: "http://" + sPath + basePath + "inner_poll/vote_js" + url_suffix,
	 		dataType: 'json',
	 		data: "abstain=1&n=" + n,
	 		success: function(data){$('div#inner_poll_' + suffix + n).replaceWith(data.result['#value']);},
		 	error: function(data){$('div#inner_poll_' + suffix + n).replaceWith(data.result['#value']);}
 	});
}

function cancel_vote(n, block){
  block = block || 0;
  suffix = '';
  url_suffix = '';
  if(block != 0) {
    suffix = 'block_';
    url_suffix = '/block';
  }
  basePath = Drupal.settings.basePath;
  if(!is_array(Drupal.settings.sPath))sPath = Drupal.settings.sPath;
  else sPath = Drupal.settings.sPath[0];
	$('div#inner_poll_' + suffix + n).html('Loading...');
 	$.ajax(
  {
	 		type: "POST",
	 		url: "http://" + sPath + basePath + "inner_poll/cancel_vote" + url_suffix,
	 		dataType: 'json',
	 		data: "n=" + n,
	 		success: function(data){$('div#inner_poll_' + suffix + n).replaceWith(data.result['#value']);},
		 	error: function(data){$('div#inner_poll_' + suffix + n).replaceWith(data.result['#value']);}
 	});
}

function is_array(input) {
    return typeof(input)=='object'&&(input instanceof Array);
}