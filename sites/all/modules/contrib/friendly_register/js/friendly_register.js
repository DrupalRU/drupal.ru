(function ($) {

  Drupal.friendly_register = Drupal.friendly_register || {};

  Drupal.friendly_register.flood = false;
  Drupal.friendly_register.timeout = null;

  Drupal.friendly_register.checkUserName = function (userName) {
    $.getJSON(userName.ajaxPath + encodeURIComponent(userName.oldValue), function(data) {
      if (!data.flood) {
        var message;
        var cssclass;
        if (data.available) {
          message = userName.avail;
          cssclass = 'ok';
        } else {
          message = userName.notAvail;
          cssclass = 'error';
        }
        $('#edit-name-check').remove();
        userName.field.after('<div id="edit-name-check" class="' + cssclass + '"><span class="text">' + message + '</span></div>');
        Drupal.attachBehaviors();
      } else {
        Drupal.friendly_register.flood = true;
        $('#edit-name-check').remove();
        clearTimeout(Drupal.friendly_register.timeout);
      }
    });
  };

  Drupal.friendly_register.checkEmail = function (email) {
    $.getJSON(email.ajaxPath + encodeURIComponent(email.oldValue), function(data) {
      if (!data.flood) {
        if (data.available == 'incomplete') {
          $('#edit-mail-check').remove();
          return;
        }
        var message;
        if (data.available) {
          message = email.avail;
          cssclass = 'ok';
        } else {
          message = email.notAvail;
          cssclass = 'error';
        }
        $('#edit-mail-check').remove();
        email.field.after('<div id="edit-mail-check" class="' + cssclass + '"><span class="text">' + message + '</span></div>');
        Drupal.attachBehaviors();
      } else {
        Drupal.friendly_register.flood = true;
        $('#edit-mail-check').remove();
        clearTimeout(Drupal.friendly_register.timeout);
      }
    });
  };

  Drupal.behaviors.friendly_register = {
    attach: function (context, settings) {
      var loginURL = Drupal.settings.basePath + 'user';
      var resetURL = Drupal.settings.basePath + 'user/password';

      var userName = new Object();
      userName.oldValue = '';
      userName.ajaxPath = Drupal.settings.basePath + 'ajax/check-user/';
      userName.field = $('.friendly-register-name', context);
      userName.avail = Drupal.t('This username is available.');
      userName.notAvail = Drupal.t('This username is not available.');

      var email = new Object();
      email.oldValue = '';
      email.ajaxPath = Drupal.settings.basePath + 'ajax/check-email/';
      email.field = $('.friendly-register-mail', context);
      email.avail = Drupal.t('This email address has not been used.');
      email.notAvail = Drupal.t('This email address is already in use, please <a href="@login">try logging in</a> with that email address or <a href="@reset">resetting your password</a>.', {'@login': loginURL, '@reset': resetURL});

      userName.field.once('friendly-register').focus(function () {
        if (Drupal.friendly_register.flood) {
          return;
        }
        Drupal.friendly_register.timeout = setInterval(function (){
          var newValue = userName.field.val();
          if (newValue != userName.oldValue) {
            userName.oldValue = newValue;
            Drupal.friendly_register.checkUserName(userName);
          }
        }, 1000);
      })
      .blur(function () {
        clearTimeout(Drupal.friendly_register.timeout);
        var newValue = userName.field.val();
        if (!Drupal.friendly_register.flood && newValue != userName.oldValue) {
          userName.oldValue = newValue;
          Drupal.friendly_register.checkUserName(userName);
        }
      });

      email.field.once('friendly-register').focus(function () {
        if (Drupal.friendly_register.flood) {
          return;
        }
        Drupal.friendly_register.timeout = setInterval(function (){
          var newValue = email.field.val();
          if (newValue != email.oldValue) {
            email.oldValue = newValue;
            Drupal.friendly_register.checkEmail(email);
          }
        }, 1000);
      })
      .blur(function () {
        clearTimeout(Drupal.friendly_register.timeout);
        var newValue = email.field.val();
        if (!Drupal.friendly_register.flood && newValue != email.oldValue) {
          email.oldValue = newValue;
          Drupal.friendly_register.checkEmail(email);
        }
      });
    }
  };

})(jQuery);
