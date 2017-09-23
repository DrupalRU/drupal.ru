BlogAPI
================================================================================================================
The Blog API module enables a post to be published to a site via external GUI applications.
Many users prefer to use external tools to improve legibility and posting responses in a
customized way. The Blog API provides users the freedom to use the blogging tools they want
but still have the blogging server of choice.

When this module is enabled and configured, you can use a variety of programs to create and
publish posts from your desktop. Blog API module supports several XML-RPC based blogging APIs
such as the Blogger API (outdated) (new Blogger Data API, MetaWeblog API, and most of the Movable Type API.

Features
================================================================================================================

  * Now supports such APIs: Blogger Data API, MetaWeblog API, Movable Type API.

  * Use Content types as different Blog IDs

  * Supports such operations: new post (new node), retrieve post, edit post, get recent posts, get categories,
    set categories, add media object, etc

  * Covered by simple tests

  * Provide list of available hooks to manipulate of work

  * Built on Services module and support all its submodules


Configuration
================================================================================================================
  1. Enable BlogAPI module and at least on submodule for particular endpoint

  2. On the People Permissions administration page (admin/people/permissions) you need to assign:
     - "Manage content with BlogAPI" permission to be possible use any BlogAPI endpoint method

     - "Administer BlogAPI settings" to have access to admin/config/services/blogapi page and
      change global settings, as content types or default Blog API

  3. On the BlogAPI Configuration page (admin/config/services/blogapi) set:
     - Content types to be available through external blogging tools

     - Default BlogAPI endpoint

     - Configure File Settings such as maximum, total and other sizes of uploaded files

  4. Use privileged user's credentials to use on external Blogging applications

  5. Use machine name of content type as Blog Id




