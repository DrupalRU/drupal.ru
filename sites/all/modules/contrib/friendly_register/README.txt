--------------------------------------------------------------------------------
                             Friendly Register
--------------------------------------------------------------------------------

Maintainers:
  * Andrew M Riley

Project homepage: http://drupal.org/project/friendly_register

Friendly Register module allows users to see if a username or email address has
already been used during registration before they submit the form. This module
checks the database and returns an error if the username is already in use. In
addition to checking the username the module checks if there is already an
account using that email address, if there is, a message is displayed with
links to the login or reset password pages.

Installation
------------
* Install this module into the appropriate modules directory and enable it.
  See: https://drupal.org/node/895232 for more information.
* Make sure you have cron running on a regular basis (at least once per 24 hours)

Verification
------------
* On your site, log out.
* Try to create an account using a pre-existing user name or email address.
* If the module is working, an immediate error message should appear below that
  field stating either the user name is unavailable or that the email is
  already registered.

Permissions
-----------
There is only one permission for this module and it is completely optional. Set
"Ignore Flood Checking" for a role if you wish them to not be checked by the
300 query a day limit. I would recommend against giving the "anonymous" role
this permission.

Developers
-----------
If you wish to use Friendly Register with your module:
 1) Add the friendly-register-name or friendly-register-mail class to your
    field(s).
 2) In your module, call friendly_register_load_resources() after adding the
    classes. This will usually happen inside the function that adds the classes.
    See friendly_register_form_user_register_form_alter() for an example.
