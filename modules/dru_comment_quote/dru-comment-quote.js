/**
 * @file
 */
(function ($, Drupal) {
  /**
   * Provide node details affix feature.
   */
  Drupal.behaviors.CommentQuote = {
    attach: function (context) {
      $('.comment-quote-link').once('comment-quote-link', function () {
        $(this).click(function (event) {
          var settings = Drupal.settings.dru_comment_quote;
          var quote = getQoute(settings);
          if (quote === false) {
            alert(settings.pleas_select);
          }
          else {
            insertAtCursor(document.getElementsByName("comment_body[und][0][value]")[0], quote);
          }
          return false;
        });
      });

      var getQoute = function () {
        var range, parent, selected_text, user_name;
        try {
          if (window.getSelection) {
            range = window.getSelection().getRangeAt(0);
          }
          else {
            range = document.getSelection().getRangeAt(0);
          }
        }
        catch (error) {
          return false;
        }

        parent = $(range.commonAncestorContainer).parents('.dru-comment-quote-content');
        if (parent.length > 0) {
          var content = range.cloneContents();
          selected_text = content.outerHTML || content.textContent;

          selected_text = quoteFilter(selected_text);
          selected_text = quoteReplace(selected_text);

          user_name = $(parent).data('user');
          return "[quote=" + user_name + "]\n" + selected_text + "\n[/quote]\n";
        }
        return false;
      };

      quoteFilter = function (text) {
        var filter = ["\s\[\?\]"];
        for (i = 0; i < filter.lenght; i++) {
          text = text.replace(new RegExp(filter[i]), '');
        }
        return text;
      };

      quoteReplace = function (text) {
        var data = [];
        for (var i in data) {
          text.replace(i, data[i]);
        }
        return text;
      };

      insertAtCursor = function (textArea, text) {
        if (document.selection) {
          textArea.focus();
          document.selection.createRange().text = text;
        }
        else if (textArea.selectionStart || textArea.selectionStart === '0') {
          var position = textArea.selectionStart;
          textArea.value = textArea.value.substring(0, textArea.selectionStart) + text + textArea.value.substring(textArea.selectionEnd, textArea.value.length);
          textArea.selectionStart = textArea.selectionEnd = position + text.length;
        }
        else {
          textArea.value += text;
        }
        textArea.focus();
      };
    }
  };

})(jQuery, Drupal);
