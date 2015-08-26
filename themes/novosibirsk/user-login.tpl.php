<?php
	// User login form
	$form["links"] = array();
	$form["name"]["#theme"] = "user_login_form_name_field";
	$form["pass"]["#theme"] = "user_login_form_pass_field";
	$form["submit"]["#theme"] = "user_login_form_submit_field";
	
	print drupal_render($form['name']);
	print drupal_render($form['pass']);
	print drupal_render($form);
	print	"<p><a href=\"" . base_path() . "user/register\"><strong>" . t('Create new account') . "</strong></a><br><br><a href=\"" . base_path() . "user/password\">" . t('Request new password') . "</a></p>";	