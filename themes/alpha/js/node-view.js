/**
 * @file
 * Adjust node details block width for screen size.
 */

(function ($, Drupal) {
  "use strict";

  /**
   * Provide node details affix feature.
   */
  Drupal.behaviors.AlphaNodeAffix = {
    attach: function (context) {

    $("#node-details").width($("#node-details").parent().width());

    $("#node-details").affix({
        offset: {top: function () {
            // возвращает координату Y родительского .node
            return (this.top = $("#node-details").closest('.node').offset().top)
            },
            bottom: function () {
                // возвращает расстояние от нижней точки родительского .node до низа окна
                var node_parent = $("#node-details").closest('.node');
                var all_height = $(document).height();
                return (this.bottom = all_height - node_parent.offset().top - node_parent.height())
            }
        }
    });

      $(window).on('resize', function(){
        $("#node-details").width($("#node-details").parent().width());
      });
    }
  };

})(jQuery, Drupal);
