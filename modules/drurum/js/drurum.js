(function ($) {
  "use strict";
  
  /**
   * Js behavior of drurum module.
   */
  Drupal.behaviors.drurum = {
    attach: function (context, settings) {
      this.bueditorImprovement();
    },
    
    /**
     * Allows to use text/html instead of images in buttons.
     */
    bueditorImprovement: function () {
      if (window.BUE) {
        
        // Return html for editor templates.
        window.BUE.theme = function (tplid) {
          var tpl = BUE.templates[tplid] || {html: ''}, html = '', sprite;
          if (typeof tpl.html == 'string') return tpl.html;
          // Load sprite
          if (sprite = tpl.sprite) {
            var surl = (new Image()).src = sprite.url, sunit = sprite.unit, sx1 = sprite.x1;
            $(document.body).append('<style type="text/css" media="all">.bue-' + tplid + ' .bue-sprite-button {background-image: url(' + surl + '); width: ' + sunit + 'px; height: ' + sunit + 'px;}</style>');
          }
          var title, content, icon, key, func,
            style = document.documentElement.style,
            access = ('MozAppearance' in style) && 'Shift + Alt' || (('ActiveXObject' in window) || ('WebkitAppearance' in style)) && 'Alt';
          // Create html for buttons. B(0-title, 1-content, 2-icon or caption, 3-accesskey) and
          // 4-function for js buttons
          for (var B, isimg, src, type, btype, attr, alt, i = 0, s = 0; B = tpl.buttons[i]; i++) {
            // Empty button.
            if (B.length == 0) {
              s++;
              continue;
            }
            title = B[0], content = B[1], icon = B[2], key = B[3], func = null;
            // Set button function
            if (content.substr(0, 3) == 'js:') {
              func = B[4] = new Function('E', '$', content.substr(3));
            }
            isimg = (/\.(png|gif|jpg)$/i).test(icon);
            // Theme button.
            if (title.substr(0, 4) == 'tpl:') {
              html += func ? (func(null, $) || '') : content;
              html += icon ? ('<span class="separator">' + (isimg ? '<img src="' + tpl.iconpath + '/' + icon + '" />' : icon) + '</span>') : '';
              continue;
            }
            // Text button
            if (!isimg) {
              // (1) Just set 'attr' = 'icon' as we either have text or html.
              // and want to output it directly.
              //type = 'button', btype = 'text', attr = 'value="'+ icon +'"';
              type = 'button', btype = 'text', attr = icon;
            }
            else {
              type = 'image';
              // Sprite button
              if (sprite) {
                // (2) To make this work with image buttons we need to add the actual <img> tag.
                //btype = 'sprite', attr = 'src="'+ sx1 +'" style="background-position: -'+ (s *
                // sunit) +'px 0;"';
                btype = 'sprite', attr = '<img src="' + sx1 + '" style="background-position: -' + (s * sunit) + 'px 0;" />';
                s++;
              }
              // Image button
              else {
                // (3) To make this work with image buttons we need to add the actual <img> tag.
                //btype = 'image', attr = 'src="'+ tpl.iconpath +'/'+ icon +'"';
                btype = 'image', attr = '<img src="' + tpl.iconpath + '/' + icon + '" />';
              }
            }
            alt = title + (key ? '(' + key + ')' : '');
            title += access && key ? ' (' + access + ' + ' + key + ')' : '';
            // (4) This can probably be any tag but an anchor seemed appropriate.
            // We wrap 'attr' which can be text, html or an image with the anchor tag so it doesn't
            // really matter what 'attr' is, it'll work either way. html += '<input type="'+ type
            // +'" alt="'+ alt +'" title="'+ title +'" accesskey="'+ key +'" id="bue-%n-button-'+ i
            // +'" class="bue-button bue-'+ btype +'-button editor-'+ btype +'-button" '+ attr +'
            // tabindex="'+ (i ? -1 : 0) +'" />';
            html += '<button alt="' + alt + '" title="' + title + '" accesskey="' + key + '" id="bue-%n-button-' + i + '" class="bue-button bue-' + btype + '-button editor-' + btype + '-button" tabindex="' + (i ? -1 : 0) + '" >' + attr + '</button>';
          }
          return tpl.html = '<div class="bue-ui bue-' + tplid + ' editor-container clearfix" id="bue-ui-%n" role="toolbar">' + html + '</div>';
        };
      }
    }
  };
})(jQuery);
