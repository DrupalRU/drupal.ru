This module provides a filter for referencing image nodes from other
nodes. It will not work unless image.module is installed and
configured properly.

Syntax:

  [image:node_id align=alignment hspace=n vspace=n border=n
     size=label width=n height=n nolink=(0|1)
     class=name style=style-data]

In its simplest form, "[image:node_id]", each image code will be
replaced by a thumbnail-sized image linked to the full-size image
node.  The other parameters, which are optional, give the user greater
control over the presentation of the thumbnail image.

For more information see http://drupal.org/project/image_filter
