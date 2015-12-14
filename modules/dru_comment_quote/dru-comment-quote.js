/**
 * @file
 */
(function ($, Drupal) {
  /**
   * Provide node details affix feature.
   */

  Drupal.behaviors.CommentQuote = {
    attach: function (context) {
      var clickTimeoutId;

      $('.comment-quote-link').once('comment-quote-link', function () {
        $(this).click(function (event) {
          clickTimeoutId = setTimeout(ClickEventHandler(this), 500);
          return false;
        }).dblclick(function (event) {
          clearTimeout(clickTimeoutId);
          ClickEventHandler(this);
          return false;
        });
      });

// Обработка клика по ссылке "Цитировать"
      function ClickEventHandler(link) {
        var quote = getQoute(link) || '';
        insertAtCursor(document.getElementsByName("comment_body[und][0][value]")[0], quote);
        return false;
      }
      
// Формирует цитируемый текст для вставки в поле комментария
      function formatQuote(element, user_name) {
        var selected_text = element.innerHTML || element.textContent || '';
        if (selected_text !== '') {
          selected_text = "\n" + selected_text + "\n";
        }
        selected_text = quoteFilter(selected_text);
        selected_text = quoteReplace(selected_text);
        return "[quote=" + user_name + "]" + selected_text + "[/quote]\n";
      }
      ;

// Определяет человекопонятное значение, возвращаемое методом объекта Range
// Range.compareBoundaryPoints (параметр = key)
      function PointPosition(key) {
        var positions = ['before', 'equal', 'after'];
        return positions[key + 1];
      }


/* 
 * Корректирует выделение текста в поле сущности(node, comment)
 * 
 * Если selection внутри container:
 *  Возвращается selection
 * 
 * Если selection за пределами container:
 *  Возвращается container
 *  
 * Если selection и container пересекаются:
 *  Возвращается их пересечение 
 * 
 * @param {DODOMObject} container - комментируемое поле
 * @param {Rannge} selection - выделеный текст
 * @returns {Range}
 */
      function checkSelection(container, selection) {
        var states = {AFTER: 'after', BEFORE: 'before'}, status = {}, container_range = document.createRange();
        container_range.selectNode(container);


        status.start_to_start = PointPosition(selection.compareBoundaryPoints(selection.START_TO_START, container_range));
        status.end_to_end = PointPosition(selection.compareBoundaryPoints(selection.END_TO_END, container_range));
        status.start_to_end = PointPosition(selection.compareBoundaryPoints(selection.START_TO_END, container_range));
        status.end_to_start = PointPosition(selection.compareBoundaryPoints(selection.END_TO_START, container_range));

//Выделение В Контейнере 
        if (status.start_to_start === states.AFTER && status.end_to_end === states.BEFORE) {
          return selection;
        }


// ЧАСТЬ Выделения НЕ в Контейнере
// Сдвигаем нужную границу Выделения к границе Контейнера

        if (status.end_to_start === states.BEFORE &&
                status.end_to_end === states.AFTER) {
          selection.setEndAfter(container);
          return selection;
        }

        if (status.start_to_end === states.AFTER &&
                status.start_to_start === states.BEFORE) {
          selection.setStartBefore(container);
          return selection;
        }

// Выделение НЕ в КОНТЕЙНЕРЕ
        return container_range;

      }
/*
 * Возвращает текст цитату, обернутый в тэг цитаты.
 * 
 * @param {type} link - ссылка - инициатор цитирования (Цитировать) 
 *    соответствующего комментария
 * @returns {undefined|String} Цитату
 */
      function getQoute(link) {
        var source_container, user_name, selection, content, wrapper;

        source_container = $('[data-source="' + $(link).data('id') + '"]');
        if (source_container.length === 0) {
          return;
        }
        user_name = $(source_container).data('user');
        source_container = source_container.find('.field-item')[0];

        try {
          if (window.getSelection) {
            selection = window.getSelection().getRangeAt(0);
          }
          else {
            selection = document.getSelection().getRangeAt(0);
          }
        }
        catch (error) {
          return formatQuote(source_container, user_name);
        }

        content = checkSelection(source_container, selection).cloneContents();

        wrapper = document.createElement('span');
        wrapper.appendChild(content);

        return formatQuote(wrapper, user_name);

      }
      
/**
 * Фильтрует цитируемый текст
 * @param {type} text
 * @returns {unresolved} Отфильтрованный текст
 */
      function quoteFilter(text) {
        var filter = ["\s\[\?\]"];
        for (i = 0; i < filter.lenght; i++) {
          text = text.replace(new RegExp(filter[i]), '');
        }
        return text;
      }
 
 
/**
 * Заменяет текст в цитате
 * @param {type} text
 * @returns {unresolved} Текст цитаты
 */
      function quoteReplace(text) {
        var data = [];
        for (var i in data) {
          text.replace(i, data[i]);
        }
        return text;
      }
/**
 * Вставляет цитату в место курсора в текстовое поле вормы комментирования
 * @param {type} textArea
 * @param {type} text
 * @returns {undefined}
 */
      function insertAtCursor(textArea, text) {
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
      }
    }
  };

})(jQuery, Drupal);
