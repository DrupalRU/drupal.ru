Description
===========

This module enables you to move comments and nodes around.

Comments can be

a) moved below another comment on the same node
b) moved to another node
c) converted to a node (eg story, page)

Nodes can be converted to a comment of some another node.

In all cases child comments will be moved together with the parent comment or 
node.

The module works with Drupal 7.


Configuration
============= 
1 - After enabling the module, go to administer >> blocks (admin/block) and 
    enable the block "Comment mover clipboard".
2 - Set access permissions:
    - the 'administer comments' permission to move comments and nodes
    - the 'create [nodetype]' permission to convert a comment to [nodetype]


Usage
=============
Basic use is simple; it starts by cutting a comment via the link 
'cut'. The cutted comment now appears in the 'Clipboard' block. The clipboard
block shows the title of the cutted content and always allows you
to cancel the action. You may cut as much comments as you want. 
You can now navigate through your site and choose one of two options: pasting or
conversion.

Pasting is a simple matter: simply click the 'paste' link on the content
(comment or node) you want the cutted object to reside under. All child
comments will be moved together with the cutted node or comment.

You can use a selectbox in the 'Clipboard' block to convert the cutted comments
to a node of choosen type. When you, for example, select a 'story' content type
option and click on 'convert' button, a new node of 'story' node type will be
created. The body of the comment will be moved to the body of the node. If the
moved comment entity has some field instances that exist in the node type the
comment converted to, the data from that fields will be moved to that new node
too. All children of the original comment will be moved to the new node as child
comments.

For converting a node to a comment you should use the 'cut' link under that
node. Then you should find some other node or comment where you want to insert a
converted node as a child. Click on the 'paste' link, the node will be converted
to a comment and will be inserted as a child comment to that entity. All child
comments will be moved as child comments to that newly created comment too. If
the node type of the converted node has some field instances that exist in the
comment entity you want convert to, the data of that fields will be moved to
that new comment too.


For a new developer
===================
Do you want to improve our module? Jump the track with this easy-to-catch
explanation:
We have two classes here: CommentMoverClipboard class and
CommentMoverMovingBundle class.
When we are trying to cut an entity, we actually only write an information about
that entity to a Clipboard object (in fact, this data is storing in $_SESSTION
variable for now), and do nothing with the citted entity.
When we are trying to paste all entities from the clipboard, we are creating a
MovingBundle object and run it's function 'move'.
The 'move' function is the most hard to catch - we use here a recursion to move
not only the cutted entities, but all of their comments too, and all comments of
their comments and so on.
Try to add some debug information there (krumo or print_r) and cut/paste some
comment. You will see what happens there.
I hope you will quickly grasp the architecture of this module and help us to
maintain and develop it! Thank you for your interest and see you on drupal.org!
You can ask me any question about the developing of that module through my
contact form: https://www.drupal.org/user/1133806/contact.
Good luck!


Authors
=============
Initial version: Gerhard Killesreiter

Update to 4.7 and much improved interface: Heine Deelstra

Update to 6.x: Alan Doucette

Rewrite for 7.x: Nikita Petrov
