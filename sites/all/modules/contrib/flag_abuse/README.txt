
This is a simple module that allows users with correct permissions to flag
content, comments, and users as abuse.  It also permits users with correct
permissions to white list any content so that it cannot be marked as abuse,
using whitelist flags.  By default six flags are created by this module:

 - abuse_comment
 - abuse_node
 - abuse_user
 - abuse_whitelist_comment
 - abuse_whitelist_node
 - abuse_whitelist_user

You can add your own abuse flags as well by categorizing them as abuse flags at
/admin/structure/flags/abuse

White listing flags must be in the form, 'abuse_whitelist_' .$flag->type
in order for them to register as abuse flags, which means that most options are
covered by the default whitelist flags.

In addition, this module creates three administrative views for reviewing abuse
reports and white listing, and they can be found here:

 - /admin/content/comment/flag-abuse/abuse_comment
 - /admin/content/flags/abuse_node
 - /admin/content/flags/abuse_user

These views do not cover every possible use case and may need to be modified to
fit your specific needs.  You must make sure to have enabled the particular
flags for the specified view or you will get an error about missing flags when
attempting to display the view.

Finally, if you are using the Flags 3 api, you will need to manually adjust the
individual flags from the administration page /admin/structure/flags to
configure the specific style and location of display you wish them to have.
This is fairly straight forward flags implementation.
