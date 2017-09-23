/**
 * @file
 * Adds two new methods to the Infobubble.prototype class.
 */
/*jshint -W069 */

if (typeof InfoBubble === 'function') {
    /* First new method: bubbleBackgroundClassName allows theming of the whole
     popup bubble via css. */
    InfoBubble.prototype.setBubbleBackgroundClassName = function (className) {
        this.contentContainer_.classList.add(className);
    };
    InfoBubble.prototype['setBubbleBackgroundClassName'] =
        InfoBubble.prototype.setBubbleBackgroundClassName;

    /* Second new method: closeImage allows reference to a custom image to
     close the popup window. */
    InfoBubble.prototype.setCloseImage = function (image) {
        this.close_.src = image;
    };
    InfoBubble.prototype['setCloseImage'] =
        InfoBubble.prototype.setCloseImage;

    /* Third new method: closePosition allows you to set the position to something
     other than absolute. */
    InfoBubble.prototype.setClosePosition = function (position) {
        this.close_.style['position'] = position;
    };
    InfoBubble.prototype['setClosePosition'] =
        InfoBubble.prototype.setClosePosition;

    /* Fourth new method: closeWidth allows you to specify a custom close image width */
    InfoBubble.prototype.setCloseWidth = function (width) {
        this.close_.style['width'] = width;
    };
    InfoBubble.prototype['setCloseWidth'] =
        InfoBubble.prototype.setCloseWidth;

    /* Fifth new method: closeHeight allows you to specify a custom close image height */
    InfoBubble.prototype.setCloseHeight = function (height) {
        this.close_.style['height'] = height;
    };
    InfoBubble.prototype['setCloseHeight'] =
        InfoBubble.prototype.setCloseHeight;

    /* Sixth new method: closeBorder allows you to add a border to the close image. */
    InfoBubble.prototype.setCloseBorder = function (border) {
        this.close_.style['border'] = border;
    };
    InfoBubble.prototype['setCloseBorder'] =
        InfoBubble.prototype.setCloseBorder;

    /* Seventh new method: closeZIndex allows you to set a custom zindex for your
     close image. */
    InfoBubble.prototype.setCloseZIndex = function (zIndex) {
        this.close_.style['zIndex'] = zIndex;
    };
    InfoBubble.prototype['setCloseZIndex'] =
        InfoBubble.prototype.setCloseZIndex;

    /* Eighth new method: closeCursor allows you change what your cursor turns
     into on hovering on the close image. */
    InfoBubble.prototype.setCloseCursor = function (cursor) {
        this.close_.style['cursor'] = cursor;
    };
    InfoBubble.prototype['setCloseCursor'] =
        InfoBubble.prototype.setCloseCursor;
}
