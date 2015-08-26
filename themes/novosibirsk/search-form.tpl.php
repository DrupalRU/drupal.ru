<?php
	//foreach($form as $k => $v)drupal_set_message($k . " = " . $v);
	//drupal_set_message("===  ===");
	//foreach($form["form_id"] as $k => $v)drupal_set_message($k . " = " . $v);
	//drupal_set_message("===  ===");
	//foreach($form["form_token"] as $k => $v)drupal_set_message($k . " = " . $v);
	//return;

	$form['#action'] = url('search/node');
	$form["search_block_form"]["#theme"] = "search_form_block_field";
	$form["submit"]["#theme"] = "search_form_block_submit";
	
	$form["form_build_id"] = array(
		"#name" => "form_build_id",
		"#type" => "hidden",
		"#value" => $form["#build_id"],
		"#id" => $form["#build_id"],
	);
	
	$form["form_id"] = array(
		"#name" => "form_id",
		"#type" => "hidden",
		"#value" => "search_block_form",
		"#id" => "edit-search-block-form",
	);
	
	$form["form_token"] = array(
		"#name" => "form_token",
		"#type" => "hidden",
		"#value" => $form["form_token"]["#value"],
		"#id" => "edit-search-block-form",
	);
	
	print	"<fieldset>";
	print drupal_render($form["search_block_form"]);
	print drupal_render($form["hidden"]);
	print drupal_render($form);	
	print	"</fieldset>";
	//drupal_set_message(" ========= ");
	//foreach($form as $k => $v)drupal_set_message($k . " = " . $v);
	