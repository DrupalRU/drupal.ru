(function ($) {
  
  
  // Hide these in a ready to ensure that Drupal.ajax is set up first.
  $(function() {
    /**
     * Override Drupal's AJAX prototype beforeSend function so it can append the
     * throbber inside the pager links.
     */
    Drupal.ajax.prototype.beforeSend = function (xmlhttprequest, options) {
      // For forms without file inputs, the jQuery Form plugin serializes the form
      // values, and then calls jQuery's $.ajax() function, which invokes this
      // handler. In this circumstance, options.extraData is never used. For forms
      // with file inputs, the jQuery Form plugin uses the browser's normal form
      // submission mechanism, but captures the response in a hidden IFRAME. In this
      // circumstance, it calls this handler first, and then appends hidden fields
      // to the form to submit the values in options.extraData. There is no simple
      // way to know which submission mechanism will be used, so we add to extraData
      // regardless, and allow it to be ignored in the former case.
      if (this.form) {
        options.extraData = options.extraData || {};
      
        // Let the server know when the IFRAME submission mechanism is used. The
        // server can use this information to wrap the JSON response in a TEXTAREA,
        // as per http://jquery.malsup.com/form/#file-upload.
        options.extraData.ajax_iframe_upload = '1';
      
        // The triggering element is about to be disabled (see below), but if it
        // contains a value (e.g., a checkbox, textfield, select, etc.), ensure that
        // value is included in the submission. As per above, submissions that use
        // $.ajax() are already serialized prior to the element being disabled, so
        // this is only needed for IFRAME submissions.
        var v = $.fieldValue(this.element);
        if (v !== null) {
          options.extraData[this.element.name] = v;
        }
      }
    
      // Disable the element that received the change to prevent user interface
      // interaction while the Ajax request is in progress. ajax.ajaxing prevents
      // the element from triggering a new request, but does not prevent the user
      // from changing its value.
      $(this.element).addClass('progress-disabled').attr('disabled', true);
    
      // Insert progressbar or throbber.
      if (this.progress.type == 'bar') {
        var progressBar = new Drupal.progressBar('ajax-progress-' + this.element.id, eval(this.progress.update_callback), this.progress.method, eval(this.progress.error_callback));
        if (this.progress.message) {
          progressBar.setProgress(-1, this.progress.message);
        }
        if (this.progress.url) {
          progressBar.startMonitoring(this.progress.url, this.progress.interval || 1500);
        }
        this.progress.element = $(progressBar.element).addClass('ajax-progress ajax-progress-bar');
        this.progress.object = progressBar;
        $(this.element).after(this.progress.element);
      }
      else if (this.progress.type == 'throbber') {
        var iconKey = Drupal.settings.druru.icons.key,
          iconThrobber = Drupal.settings.druru.icons.throbber;
        this.progress.element = $('<div class="ajax-progress ajax-progress-throbber">' +
          '<i class="icon ' + iconKey + ' ' + iconThrobber + ' ' + iconKey + '-spin" aria-hidden="true"></i>' +
          '</div>');
        // If element is an input type, append after.
        if ($(this.element).is('input')) {
          if (this.progress.message) {
            $('.throbber', this.progress.element).after('<div class="message">' + this.progress.message + '</div>');
          }
          $(this.element).parent().append(this.progress.element);
        }
        // Otherwise inject it inside the element.
        else {
          if (this.progress.message) {
            $('.throbber', this.progress.element).append('<div class="message">' + this.progress.message + '</div>');
          }
          $(this.element).parent().append(this.progress.element);
        }
      }
    };
  
    /**
     * Override handler for the form redirection error.
     */
    Drupal.ajax.prototype.error = function (xmlhttprequest, uri, customMessage) {
      // The variable response.status contain http response code.
      // Don't show message if request is not completed (status == 0)
      if (xmlhttprequest.status == 0) {
        return false;
      }
    
      switch (parseInt(Drupal.settings.druru_system_logging_settings)) {
        case 2:
          alert(Drupal.ajaxError(xmlhttprequest, uri, customMessage));
          break;
        case 1:
          console.error(Drupal.ajaxError(xmlhttprequest, uri));
          break;
      }
      // Remove the progress element.
      if (this.progress.element) {
        $(this.progress.element).remove();
      }
      if (this.progress.object) {
        this.progress.object.stopMonitoring();
      }
      // Undo hide.
      $(this.wrapper).show();
      // Re-enable the element.
      $(this.element).removeClass('progress-disabled').removeAttr('disabled');
      // Reattach behaviors, if they were detached in beforeSerialize().
      if (this.form) {
        var settings = this.settings || Drupal.settings;
        Drupal.attachBehaviors(this.form, settings);
      }
    };
  });

})(jQuery);
