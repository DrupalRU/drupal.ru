/**
 * Добавление или удаление дополнительных ответов к вопросу.
 */

(function ($) {
  /* Для совместимости JavaScript и jQuery */
})(jQuery);

function addFormField() {
  var id = document.getElementById("edit-inner-poll-new-id").value;
  if(id < 999) {
    id_chose = id;
    id_chose++;

    jQuery("#inner_poll_new_fields").append('<div id="edit-choice-' + id + '-wrapper" class="form-item"><input type="text" class="fluid form-text" value="" size="" id="edit-choice-' + id + '" name="choice_' + id + '" maxlength="128"/><div class="description">choice ' + id_chose + ' <a href="javascript:{}" onclick="removeFormField(\'#edit-choice-' + id + '-wrapper\'); return false;">[x]</a></div></div>');

    id++;

    document.getElementById("edit-inner-poll-new-id").value = id;
  }
}

function removeFormField(id) {
  jQuery(id).remove();
}
