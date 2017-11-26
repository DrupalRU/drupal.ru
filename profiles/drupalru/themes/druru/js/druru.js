/**
 * @file
 * druru.js
 *
 * Provides general enhancements and fixes to Bootstrap's JS files.
 */

var Drupal = Drupal || {};

(function ($, Drupal) {
  "use strict";

  Drupal.behaviors.druru = {
    attach: function (context, settings) {

      // Improvement for bootstrap-filestyle plugin.
      $.fn.filestyle.defaults.buttonText = 'Выбрать файл';
      $.fn.filestyle.defaults.iconName = 'fa fa-folder-open';
      this.improveBootstrapTabIntegration(context, settings);
      this.improveBootstrapDropdowns(context, settings);
      this.initContextMenu(context, settings);

      // disallow click by link with path to this page
      $('a.active').click(function () {
        return window.location.pathname !== $(this).attr('href');
      });
      // stylizing the select
      $('.selectpicker').selectpicker({
        noneSelectedText: 'Ничего не выбрано'
      });
      // stylizing the file input
      $(":file").filestyle();


    },

    improveBootstrapTabIntegration: function (context, settings) {
      // We need to wait when drupal perform wrapping for the vertical tabs.
      // After it, we can execute the handler for the tabs (wrapper).
      setTimeout(function () {
        // Provide some Bootstrap tab/Drupal integration.
        $(context).find('.tabbable').once('bootstrap-tabs', function () {
          var $wrapper = $(this);
          var $tabs = $wrapper.find('.nav-tabs');
          var $content = $wrapper.find('.tab-content');
          var borderRadius = parseInt($content.css('borderBottomRightRadius'), 10);
          var druruTabResize = function () {
            if ($wrapper.hasClass('tabs-left') || $wrapper.hasClass('tabs-right')) {
              $content.css('min-height', $tabs.outerHeight());
            }
          };
          // Add min-height on content for left and right tabs.
          druruTabResize();
          // Detect tab switch.
          if ($wrapper.hasClass('tabs-left') || $wrapper.hasClass('tabs-right')) {
            $tabs.on('shown.bs.tab', 'a[data-toggle="tab"]', function (e) {
              druruTabResize();
              if ($wrapper.hasClass('tabs-left')) {
                if ($(e.target).parent().is(':first-child')) {
                  $content.css('borderTopLeftRadius', '0');
                }
                else {
                  $content.css('borderTopLeftRadius', borderRadius + 'px');
                }
              }
              else {
                if ($(e.target).parent().is(':first-child')) {
                  $content.css('borderTopRightRadius', '0');
                }
                else {
                  $content.css('borderTopRightRadius', borderRadius + 'px');
                }
              }
            });
          }
        });
      }, 200);
    },

    improveBootstrapDropdowns: function (context, settings) {
      var checkCommentSelection = function () {
        var $dropdown = $(this).closest('.dropdown'),
          $comment = $dropdown.closest('.comment');
        $dropdown.hasClass('open') ? $comment.addClass('hovered') : $comment.removeClass('hovered');
      };
      $('.comment .dropdown').once('dropdown-events').each(function (idx, obj) {
        $(this)
          .on('show.bs.dropdown', Drupal.behaviors.druru.hideContextMenus)
          .on('shown.bs.dropdown', checkCommentSelection)
          .on('hidden.bs.dropdown', checkCommentSelection);
      });
    },

    excludeTags    : ['a', 'button'],

    contextMenuId: null,

    initContextMenuClicks: 0,

    initContextMenu: function (context, settings) {
      // Build links expanded by showing of context menu.
      var comments = $('.comment'), $comment, hasComments = false;
      if (comments.length) {
        for (var x = 0; x < comments.length; x++) {
          $comment = $(comments[x]);
          if ($comment.find('.dropdown').length) {
            $comment.once('contextual').on('contextmenu', Drupal.behaviors.druru.showContextMenu);
            hasComments = true;
          }
        }
        if (hasComments) {
          $(window).on('blur', Drupal.behaviors.druru.hideContextMenus);
          $(document).on('click', Drupal.behaviors.druru.hideContextMenus);
        }
      }
    },

    clearContextMenuData: function() {
      this.initContextMenuClicks = 0;
      this.contextMenuId = null;
    },

    showContextMenu: function (e) {

      var $target = $(e.target),
        $targetId = $target.closest('.comment').data('comment-id');

      if (Drupal.behaviors.druru.initContextMenuClicks > 0) {
        // Hide all previously showed menus.
        Drupal.behaviors.druru.hideContextMenus(e);
      }

      Drupal.behaviors.druru.initContextMenuClicks++;

      if (Drupal.behaviors.druru.initContextMenuClicks === 1) {
        // Don't triggering the event at excluded tags.
        var excludeTags = Drupal.behaviors.druru.excludeTags,
          childOfExcludedTags = false,
          targetTagName = $target.prop("tagName").toString().toLowerCase(),
          tagIsExcluded = excludeTags.indexOf(targetTagName) !== -1;
        for (var tag in excludeTags) {
          if (excludeTags.hasOwnProperty(tag)) {
            childOfExcludedTags = childOfExcludedTags || !!$target.closest(excludeTags[tag]).length;
          }
        }
        if (tagIsExcluded || childOfExcludedTags) {
          return true;
        }

        e.defaultPrevented = true;

        // Hide all bootstrap dropdowns, showed by bootstrap event.
        $(document).trigger('click.bs.dropdown.data-api');

        var $comment = $(this),
          $dropdown = $comment.find('.dropdown'),
          $menu = $dropdown.find('.dropdown-menu'),
          menuWidth = $menu.outerWidth();

        // Fix for IE. He incorrectly detected offset left inside code.
        if ($target.closest('.geshifilter').length) {
          $target = $target.closest('.geshifilter > div');
        }

        $comment.addClass('hovered').addClass('context-menu-showed');
        $dropdown.css('position', 'static');
        $menu.css({
          display : 'block',
          position: 'absolute',
          left    : e.offsetX + $target.position().left,
          top     : e.offsetY + $target.position().top,
          width   : menuWidth
        });

        Drupal.behaviors.druru.contextMenuId = $targetId;

        // Disallow to show default context menu.
        return false;
      }
      else {
        Drupal.behaviors.druru.hideContextMenu($target.closest('.comment'));

        Drupal.behaviors.druru.clearContextMenuData();
      }
    },

    hideContextMenu: function (comment) {
      comment
        .removeClass('context-menu-showed')
        .removeClass('hovered')
        .find('.dropdown')
        .css('position', '')
        .find('.dropdown-menu')
        .css({
          display: '',
          position: '',
          left: '',
          top: '',
          width: ''
        });
    },

    hideContextMenus: function (e) {
      if (typeof e === 'undefined') {e = {which: 3};}

      if (
        Drupal.behaviors.druru.contextMenuId !== $(e.target).closest('.comment').data('comment-id') ||
        e.which !== 3
      ) {
        $('.context-menu-showed').each(function (idx, obj) {
          var $comment = $(this);
          Drupal.behaviors.druru.hideContextMenu($comment);
        });

        Drupal.behaviors.druru.clearContextMenuData();
      }
    }
  };

  /**
   * Bootstrap Popovers.
   */
  Drupal.behaviors.druruPopovers = {
    attach: function (context, settings) {
      if (settings.druru && settings.druru.popoverEnabled) {
        var elements = $(context).find('[data-toggle="popover"]').toArray();
        for (var i = 0; i < elements.length; i++) {
          var $element = $(elements[i]);
          var options = $.extend(true, {}, settings.druru.popoverOptions, $element.data());
          $element.popover(options);
        }
      }

      // Tickets should be shown always.
      var tickets = $(context).find('[data-toggle="ticket-popover"]').toArray();
      for (var i = 0; i < tickets.length; i++) {
        $(tickets[i]).popover({
          placement: 'bottom',
          html: true,
          content: function () {
            return $('.ticket-content[data-ticket="' + $(this).data('ticket') + '"]').html();
          }
        });
      }

      // Open or close ticket-popover on click
      $('[data-toggle="ticket-popover"]').on('click', function() {
        $(this).popover('toggle');
      });
      // Close ticket-popover on click outside OR on click close button
      $('body').on('click', function(e) {
        var target = $(e.target);
        if (target.data('toggle') !== 'popover'
          && target.parents('[data-toggle="ticket-popover"]').length === 0
          && target.parents('.popover.in').length === 0
          || target.is('.btn-ticket-popover-close') ) {
          $('[data-toggle="ticket-popover"]').popover('hide');
        }
      });
    }
  };

  /**
   * Bootstrap Tooltips.
   */
  Drupal.behaviors.druruTooltips = {
    attach: function (context, settings) {
      if (settings.druru && settings.druru.tooltipEnabled) {
        var elements = $(context).find('[data-toggle="tooltip"]').toArray();
        for (var i = 0; i < elements.length; i++) {
          var $element = $(elements[i]);
          var options = $.extend(true, {}, settings.druru.tooltipOptions, $element.data());
          $element.tooltip(options);
        }
      }
    }
  };

  /**
   * Anchor fixes.
   */
  var $scrollableElement = $();
  Drupal.behaviors.druruAnchors = {
    attach     : function (context, settings) {
      var i, elements = ['html', 'body'];
      if (!$scrollableElement.length) {
        for (i = 0; i < elements.length; i++) {
          var $element = $(elements[i]);
          if ($element.scrollTop() > 0) {
            $scrollableElement = $element;
            break;
          }
          else {
            $element.scrollTop(1);
            if ($element.scrollTop() > 0) {
              $element.scrollTop(0);
              $scrollableElement = $element;
              break;
            }
          }
        }
      }
      if (!settings.druru || !settings.druru.anchorsFix) {
        return;
      }
      var anchors = $(context).find('a').toArray();
      for (i = 0; i < anchors.length; i++) {
        if (!anchors[i].scrollTo) {
          this.druruAnchor(anchors[i]);
        }
      }
      $scrollableElement.once('bootstrap-anchors', function () {
        $scrollableElement.on('click.bootstrap-anchors', 'a[href*="#"]:not([data-toggle],[data-target])', function (e) {
          this.scrollTo(e);
        });
      });
    },
    druruAnchor: function (element) {
      element.validAnchor = element.nodeName === 'A' && (location.hostname === element.hostname || !element.hostname) && element.hash.replace(/#/, '').length;
      element.scrollTo = function (event) {
        var attr = 'id';
        var $target = $(element.hash);
        if (!$target.length) {
          attr = 'name';
          $target = $('[name="' + element.hash.replace('#', '') + '"');
        }
        var offset = $target.offset().top - parseInt($scrollableElement.css('paddingTop'), 10) - parseInt($scrollableElement.css('marginTop'), 10);
        if (this.validAnchor && $target.length && offset > 0) {
          if (event) {
            event.preventDefault();
          }
          var $fakeAnchor = $('<div/>')
            .addClass('element-invisible')
            .attr(attr, $target.attr(attr))
            .css({
              position: 'absolute',
              top     : offset + 'px',
              zIndex  : -1000
            })
            .appendTo(document);
          $target.removeAttr(attr);
          var complete = function () {
            location.hash = element.hash;
            $fakeAnchor.remove();
            $target.attr(attr, element.hash.replace('#', ''));
          };
          if (Drupal.settings.druru.anchorsSmoothScrolling) {
            $scrollableElement.animate({scrollTop: offset, avoidTransforms: true}, 400, complete);
          }
          else {
            $scrollableElement.scrollTop(offset);
            complete();
          }
        }
      };
    }
  };

  Drupal.behaviors.druruHelpBlocks = {
    showed: false,
    $switcher: null,
    position: {
      top: null,
      left: null,
      offsetTop: 30
    },
    attach: function (context, settings) {
      var $helpBlocks = $('.help-block'),
        self = Drupal.behaviors.druruHelpBlocks,
        position = self.position;
      if ($helpBlocks.length) {
        $('.main-content').once(function () {
          var icon = Drupal.theme('icon', 'question-circle fa-2x'),
            $switcher = $('<a name="help-switcher" href="#" class="help-switcher text-default" title="' +
              Drupal.t('Toggle descriptions on the page') +
              '">' + icon + '</a>');
          $switcher.on('click', function () {
            var showed = Drupal.behaviors.druruHelpBlocks.showed;
            showed ? $helpBlocks.hide() : $helpBlocks.show();
            $('[name="help-switcher"]').toggleClass('text-default', 'text-primary');
            Drupal.behaviors.druruHelpBlocks.showed = !showed;
            return false;
          });
          $helpBlocks.hide();
          $('.main-content').prepend($switcher);
          $switcher = $('.help-switcher');
          self.$switcher = $switcher;

          // Fixation logic.
          position.top = $switcher.offset().top;
          position.left = $switcher.offset().left;

          self.setPosition();
          $(window).on('scroll', function (event) {
            self.setPosition();
          });
        });
      }
    },

    /**
     * Set position of helper switcher.
     */
    setPosition: function () {
      var self = Drupal.behaviors.druruHelpBlocks,
        position = self.position,
        $switcher = self.$switcher;
      if (($(window).scrollTop() + position.offsetTop) > position.top) {
        $switcher
          .css({
            top  : position.offsetTop,
            left : position.left,
            right: 'auto'
          })
          .addClass('fixed')
        ;
      }
      else {
        $switcher.css('left', '');
        $switcher.css('right', '');
        $switcher.css('top', '');
        $switcher.removeClass('fixed');
      }
    }
  };

  Drupal.behaviors.druruBlogTeaserViewSwitcher = {
    attach: function (context, settings) {
      $('.view-switcher').on('click', function () {
        var $this = $(this);
        $this.closest('div').find('button').removeClass('active');
        if ($this.data('view') === 'short') {
          $this.closest('#blog').addClass('short-view');
        }
        else{
          $this.closest('#blog').removeClass('short-view');
        }
        $this.addClass('active');
      });
    }
  };

  // Make parallax effect on front page.
  // Drupal.behaviors.druruParallax = {
  //   attach: function (context, settings) {
  //     $('body').once(function () {
  //       $(window).on('mousemove', function (e) {
  //         $('body.front .jumbotron').css('background-position-x',(
  //           (100 / $(window).outerWidth() * e.pageX) * (-1) // inverse moving
  //         ));
  //       })
  //     });
  //   }
  // };

  Drupal.theme.prototype.attributes = function (attributes) {
    var parsedAttrs = '', attr = null;

    //set attributes
    for (var attribute in attributes) {
      if (attributes.hasOwnProperty(attribute)) {
        attr = attributes[attribute];
        if (typeof attributes[attribute] === 'object' && attributes[attribute] !== null) {
          attr = attributes[attribute].join(' ');
        }
        parsedAttrs += attribute + '="' + attr + '" ';
      }
    }

    return ' ' + parsedAttrs + ' ';

  };

  /**
   * Theme for icon.
   *
   * @param icon
   * @param attributes
   * @returns {string}
   */
  Drupal.theme.prototype.icon = function (icon, attributes) {
    if (typeof attributes === 'undefined') {
      attributes = {
        'class': []
      };
    }
    if (typeof attributes.class === 'undefined') {
      attributes.class = [];
    }

    if (typeof attributes.class === 'object' && attributes.class !== null) {
      attributes.class.push('fa');
      attributes.class.push('fa-' + icon);
    }
    else if (typeof attributes.class === 'string') {
      attributes.class += ' fa fa-' + icon + ' ';
    }
    return '<i ' + Drupal.theme('attributes', attributes) + '"></i>';
  };

  // Inner poll.
  window.addFormField = function () {
    if (!window.id_chose) {
      window.id_chose = document.getElementById("edit-inner-poll-new-id").value;
    }
    if (window.id_chose < 999) {
      window.id_chose++;
      var id = window.id_chose,
        choseHtml = '';
      choseHtml += '<div id="edit-choice-' + id + '-wrapper" class="form-group input-group">';
      choseHtml += '<input type="text" class="form-text form-control" value="" size="" id="edit-choice-' + id + '" name="choice_' + id + '" maxlength="128"/>';

      choseHtml += '<span class="input-group-btn">';
      choseHtml += '<button class="btn btn-default" onclick="removeFormField(\'#edit-choice-' + id + '-wrapper\'); return false;" type="button">';
      choseHtml += '<i class="fa fa-times"></i>';
      choseHtml += '</button>';
      choseHtml += '</span>';

      choseHtml += '</div>';

      $("#inner_poll_new_fields").append(choseHtml);
      document.getElementById("edit-inner-poll-new-id").value = id;
    }
  };

  // issue-639: Добавить скролинг тела комментариев
  $('.comment').each(function() {
    var $comment = $(this),
      $content = $comment.find('.content'),
      contentWidth = $content.width(),
      allowedRightSide = $comment.width() - $comment.find('.media-left').outerWidth(true) - ($content.outerWidth(true) - contentWidth);
    contentWidth > allowedRightSide ? $content.css('max-width', allowedRightSide) : null;
  });

})(jQuery, Drupal);
