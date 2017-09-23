
****************
Module Functions
****************

* Filters *
- Filter users based on permission, status, created, accessed, email, id and
  role.
- Filter users based on profile.module admin created profile fields.
- Advanced filtering with user selected conditional phrases.

* Selected Mass Actions *
- Block/Unblock users
- Delete users
- Email users
- Add/Remove a specified role

* Administrative Background Actions *
- Use access control permission "receive email advuser" to notify selected
  roles when a new user registers for an account or a user has updated her
  profile fields.


The "Advanced User" module allows users with the correct permissions to
filter the user table so that actions can be performed.  Version 2 of this
module presents an advanced filtering form to allow the user to specify the
conditional phrase for the filter.  Access control permissions also allow
users in a specified role to be able to receive notification of new users and
user profile changes.

*************
Installation
*************

Simply drop the module into the Drupal sites/all/modules folder or the
sites/<mysite>/modules folder and activate via the admin/build/modules UI as 
normal. No database installation required.

Settings:
Navigate to admin/settings/advuser to setup the email notification and other
items related to your operation of this module.

Usage:
Navigate to admin/people/advuser to begin filtering and creating actions
for selected users.
