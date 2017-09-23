(function (E, $) {
  // BUE.popups = BUE.popups || {};
  BUE.popHtml = '<div class="bue-popup" style="display: none;" tabindex="-1" role="dialog">' +
    '<div class="modal-dialog" role="document">' +
    '<div class="modal-content">' +
    '<div class="modal-header bue-popup-head">' +
    '<button type="button" class="close bue-popup-close" data-dismiss="modal" aria-label="Close">' +
    '<span aria-hidden="true">&times;</span>' +
    '</button>' +
    '<h4 class="modal-title bue-popup-title"></h4>' +
    '</div>' +
    '<div class="modal-body bue-popup-body">' +
    '<p class="bue-popup-content"></p>' +
    '</div>' +
    '</div>' +
    '</div>' +
    '</div>';
  
  //form input html.
  BUE.input = function (t, n, v, a) {
    var output, attributes = $.extend({
      'type' : t,
      'name' : n,
      'value': v || null,
      'class': 'form-submit'
    }, a);
    switch (t) {
      case 'submit':
        if (typeof attributes.class == 'string') {
          attributes.class += ' btn btn-primary';
        }
        output = BUE.html('div', BUE.html('input', '', attributes), {'class': 'form-actions'});
        break;
      case 'text':
        if (typeof attributes.class == 'string') {
          attributes.class += ' form-control';
        }
        output = BUE.html('div', BUE.html('input', '', attributes), {'class': 'form-group'});
        break;
    }
    return output;
  };
  
  //selectbox html. opt has property:value pairs.
  BUE.selectbox = function (n, v, opt, attr) {
    var H = '';
    opt = opt || {};
    for (var i in opt) {
      H += BUE.html('option', opt[i], {'value': i, 'selected': i == v ? 'selected' : null});
    }
    attr = attr || {};
    if (typeof attr.class === 'undefined') {
      attr.class = '';
    }
    attr.class += ' form-control form-select';
    return BUE.html('div', BUE.html('select', H, $.extend({}, attr, {'name': n})), {'class': 'form-group'});
  };
  
  BUE.table = function (rows, attr) {
    for (var R, H = '', i = 0; R = rows[i]; i++) {
      H += R['data'] === undefined ? BUE.trow(R) : BUE.trow(R['data'], R['attr']);
    }
    attr = attr || {};
    attr.class += ' table';
    return BUE.html('table', H, attr);
  };
  
})(BUE.instance.prototype, jQuery);
