function addFormField() {
	var id = document.getElementById("edit-inner-poll-new-id").value;
	if(id < 999) {
  	id1 = id;
	  id1++;
	  $("#inner_poll_new_fields").append('<div id="edit-choice-' + id + '-wrapper" class="form-item"><input type="text" class="form-text" value="" size="60" id="edit-choice-' + id + '" name="choice_' + id + '" maxlength="128"/><div class="description">choice ' + id1 + ' <a href="javascript:{}" onclick="removeFormField(\'#edit-choice-' + id + '-wrapper\'); return false;">[x]</a></div></div>');
  	id++;
	  document.getElementById("edit-inner-poll-new-id").value = id;
	} else alert("its too match");
}

function removeFormField(id) {
	$(id).remove();
}
