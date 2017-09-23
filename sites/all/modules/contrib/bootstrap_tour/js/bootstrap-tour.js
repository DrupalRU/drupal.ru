(function($) {
  Drupal.behaviors.bootstrapTour = {
    attach: function(context) {
      var tourConfig = Drupal.settings.bootstrapTour.tour;
      if (!tourConfig) {
        return;
      }

      // Take the path and remove the tour GET arguments from it.
      function cleanPath(path) {
        // Replace the '?' mark with '&' temporarily.
        path = path.replace('?', '&');
        // Remove any instance of '&tour=' or '&step'.
        path = path.replace(/&tour=[^&]*/, '');
        path = path.replace(/&step=[^&]*/, '');
        // Now, change the first '&' back to a '?' mark.
        path = path.replace('&', '?');

        return path;
      }

      Tour.prototype._isRedirect = function(path, currentPath) {
        if (path == null || path == Drupal.settings.basePath) {
          return false;
        }

        // Override the isRedirect function so that we can support non-clean-URLs.
        currentPath = '/' + (location.pathname+location.search).substr(1);
        currentPath = cleanPath(currentPath.replace(Drupal.settings.basePath, '/'));
        path = cleanPath(path.replace(Drupal.settings.basePath, '/'));

        if (path !== '/') {
          return (path !== currentPath);
        } else {
          return (currentPath.indexOf('?q=') !== -1);
        }
      };

      var wanderedOff = Drupal.t("You have wandered off from the tour! You will be automatically redirected back to the tour. Please click 'OK' to continue, or 'Cancel' to end the tour.");

      var basePath = Drupal.settings.basePath;
      var prev = Drupal.t("« Prev");
      var next = Drupal.t("Next »");
      var endtour = Drupal.t("End Tour");
      var shown = false;
      var t = new Tour({
        storage: window.localStorage,
        basePath: basePath,
        template: "<div class='popover tour'> \
          <div class='arrow'></div> \
          <h3 class='popover-title'></h3> \
          <div class='popover-content'></div> \
          <nav class='popover-navigation'> \
              <div class='btn-group'> \
                  <button class='btn btn-default' data-role='prev'>"+prev+"</button> \
                  <button class='btn btn-default' data-role='next'>"+next+"</button> \
              </div> \
              <button class='btn btn-default' data-role='end'>"+endtour+"</button> \
          </nav> \
          </div>",
        onShown: function () {
          shown = true;
        },
        onEnd: function () {
          $.ajax(basePath + 'bootstrap_tour/ajax/end_current_tour', {async: false});
        },
        redirect: function (path) {
          var browserPath = cleanPath("" + document.location.pathname + document.location.hash),
              cleanedPath = cleanPath(path),
              // Newer versions have a this.getCurrentStep() function - this is for backcompat.
              currentIndex = this._current,
              nextStep = this.getStep(currentIndex + 1),
              nextPath = nextStep ? basePath + nextStep.path : '',
              cleanedNextPath = cleanPath(nextPath);

          // If we haven't shown a single step and bootstrap tour is trying to
          // redirect, well, it means we've wandered off from the tour. Ask the
          // user if they'd like to return.
          if (!shown && browserPath !== cleanedPath && browserPath !== cleanedNextPath && !window.confirm(wanderedOff)) {
            // The user has opted to leave the tour!
            this.end();
            return;
          }

          // If the user has shown initiative and jumped to the next step, then
          // we advance the step counter for them, before redirecting the the
          // path which has &tour= and &step= in it.
          if (!shown && browserPath !== cleanedPath && browserPath === cleanedNextPath) {
            this.setCurrentStep(currentIndex + 1);
            path = nextPath;
          }

          // We mark this as 'shown', so we don't ask them twice.
          shown = true;

          document.location.href = path;
        }
      });

      $.each(tourConfig.steps, function(index, step) {
        var options = {
          title: step.title,
          content: step.content,
          placement: step.placement,
          animation: true
        };
        if (step.path) {
          options.path = '';
          if (step.path.trim() != '<front>') {
            if (!tourConfig.cleanUrls) {
              options.path += '?q=' // Don't need the first / in this case.
            }
            options.path += step.path;
          }
          if (step.path.indexOf('?tour') === -1 && step.path.indexOf('&tour') === -1) {
            if (!tourConfig.cleanUrls) {
              options.path += '&';
            } else {
              options.path += '?';
            }
            options.path += 'tour=' + tourConfig.name;
            if (!(tourConfig.isFirstStep && index == 0)) {
              options.path += '&step=' + index;
            }
          }
        }

        if (step.selector == '') {
          options.orphan = true;
        } else {
          options.element = step.selector;
          options.onShown = function (tour) {
            $(options.element).addClass('bootstrap-tour-selected');
            shown = true;
          };
          options.onHidden = function (tour) {
            $(options.element).removeClass('bootstrap-tour-selected');
          };
        }
        t.addSteps([options])

      });

      if (tourConfig.force && tourConfig.isFirstStep) {
        // Manually restart if "force" is true and we're on the path of the first step.
        t.restart();
      } else {
        t.start();
      }

      $(window).trigger('resize');
    }
  }
})(jQuery);

