<?php

/**
 * @file
 * Base class with auxiliary functions for forum access module tests.
 */

/**
 * Base test class for the Forum Access module.
 */
class ForumAccessBaseTestCase extends ForumTestCase {

  protected $admin_rid;
  protected $webmaster_rid;
  protected $forum_admin_rid;
  protected $edndel_any_content_rid;
  protected $edndel_own_content_rid;
  protected $edit_any_content_rid;
  protected $edit_own_content_rid;
  protected $delete_any_content_rid;
  protected $delete_own_content_rid;
  protected $create_content_rid;
  protected $anon_rid;
  protected $auth_rid;
  protected $user1;
  protected $webmaster_user;
  protected $forum_admin_user;
  protected $edndel_any_content_user;
  protected $edndel_own_content_user;
  protected $edit_any_content_user;
  protected $edit_own_content_user;
  protected $delete_any_content_user;
  protected $delete_own_content_user;
  protected $create_content_user;
  protected $moderator;
  protected $time;
  protected $accounts;
  protected $rids;
  protected $accesses;

  /**
   * Implements setUp().
   */
  function setUp($modules = array()) {
    if (!isset($this->time)) {
      $this->time = time();
    }
    $this->timeLimit = 2345;
    $this->pass("timeLimit set to $this->timeLimit.");
    parent::setUp();
    if (!module_exists('forum_access')) {
      module_enable(array('acl', 'chain_menu_access', 'forum_access'), FALSE);
    }
    $this->assertTrue(module_exists('acl'), t('Module %module enabled!', array('%module' => 'acl')), 'Setup');
    $this->assertTrue(module_exists('chain_menu_access'), t('Module %module enabled!', array('%module' => 'chain_menu_access')), 'Setup');
    $this->assertTrue(module_exists('forum_access'), t('Module %module enabled!', array('%module' => 'forum_access')), 'Setup');
    $modules = array('devel', 'devel_node_access') + $modules;
    $files = system_rebuild_module_data();
    $available_modules = array();
    foreach ($modules as $module) {
      if (!empty($files[$module]) && !module_exists($module)) {
        $available_modules[] = $module;
      }
    }
    if (!empty($available_modules)) {
      module_enable($available_modules);
    }
    parent::resetAll();
    $this->accesses = array('view', 'create', 'update', 'delete');
  }

  /*
   * Implements additional set-up tasks.
   *
   * We cannot keep the test driver from calling setUp() for the inherited
   * tests, so keep setUp() as short as possible and manually call setUp2()
   * at the start of each test.
   */
  function setUp2() {
    $this->user1 = user_load(1);
    // Update uid 1's name and password so we know it.
    $password = user_password();
    require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
    $account = array(
      'name' => 'user1',
      'pass' => user_hash_password(trim($password)),
    );
    // We cannot use user_save() here or the password would be hashed again.
    db_update('users')
      ->fields($account)
      ->condition('uid', 1)
      ->execute();
    // Reload and log in uid 1.
    $this->user1 = user_load(1, TRUE);
    $this->user1->pass_raw = $password;

    // Rebuild content access permissions
    $this->drupalLogin($this->user1);
    $this->drupalPost('admin/reports/status/rebuild', array(), t('Rebuild permissions'));

    if (module_exists('devel_node_access')) {
      // Enable Devel Node Access.
      $this->drupalGet('admin/config/development/devel');
      $this->assertResponse(200);
      $this->drupalPost('admin/config/development/devel', array(
        'devel_node_access_debug_mode' => '1',
      ), t('Save configuration'));
      $this->assertResponse(200, 'Devel Node Access configuration saved.');

      // Enable the second DNA block, too.
      $this->drupalPost('admin/structure/block/list', array(
        'blocks[devel_node_access_dna_user][region]' => 'footer',
      ), t('Save blocks'));
    }
    if (module_exists('devel')) {
      $this->drupalPost('admin/config/development/devel', array(
        'devel_error_handlers[]' => array(1, 2, 4),
      ), t('Save configuration'));
      $this->assertResponse(200, 'Devel configuration saved.');
      $this->drupalPost('admin/people/permissions/list', array(
        '1[access devel information]' => 'access devel information',
        '2[access devel information]' => 'access devel information',
      ), t('Save permissions'));
      $this->assertResponse(200, 'Devel permissions saved.');
    }
    /*
      The base class creates the following users:
      $this->user1                = user 1
      $this->admin_user           = array('administer blocks', 'administer forums', 'administer menu', 'administer taxonomy', 'create forum content')); // 'access administration pages')
      $this->edit_any_topics_user = array('create forum content', 'edit any forum content', 'delete any forum content', 'access administration pages')
      $this->edit_own_topics_user = array('create forum content', 'edit own forum content', 'delete own forum content')
      $this->web_user             = array()

      Remove these users and roles and create the ones we need.
    */
    user_role_delete((int) reset($this->admin_user->roles));
    user_role_delete((int) reset($this->edit_any_topics_user->roles));
    user_role_delete((int) reset($this->edit_own_topics_user->roles));
    user_delete($this->admin_user->uid);
    user_delete($this->edit_any_topics_user->uid);
    user_delete($this->edit_own_topics_user->uid);
    user_delete($this->web_user->uid);
    unset($this->web_user);

    // Get rids and uids up to 10/9.
    for ($i = 0; $i < 3; ++$i) {
      $dummy_rid = (int) $this->drupalCreateRole(array(), 'dummy');
      $dummy_user = $this->drupalCreateNamedUser('Dummy', array($dummy_rid));
      user_role_delete($dummy_rid);
      user_delete($dummy_user->uid);
    }

    // Create our roles.
    $this->admin_rid = 3;
    $this->webmaster_rid = (int) $this->drupalCreateRole(array('administer blocks', 'administer forums', 'administer nodes', 'administer comments', 'administer menu', 'administer taxonomy', 'create forum content', 'access content overview', 'access administration pages', 'view revisions', 'revert revisions', 'delete revisions'), '11 webmaster');
    $this->forum_admin_rid = (int) $this->drupalCreateRole(array('administer forums', 'create forum content', 'edit any forum content', 'delete any forum content', /* 'access content overview', 'access administration pages', */), '12 forum admin');
    $this->edndel_any_content_rid = (int) $this->drupalCreateRole(array('create forum content', 'edit any forum content', 'delete any forum content', 'view own unpublished content'), '13 edndel any content');
    $this->edndel_own_content_rid = (int) $this->drupalCreateRole(array('create forum content', 'edit own forum content', 'delete own forum content'), '14 edndel own content');
    $this->edit_any_content_rid = (int) $this->drupalCreateRole(array('create forum content', 'edit any forum content', 'view own unpublished content'), '15 edit any content');
    $this->edit_own_content_rid = (int) $this->drupalCreateRole(array('create forum content', 'edit own forum content', 'edit own comments'), '16 edit own content');
    $this->delete_any_content_rid = (int) $this->drupalCreateRole(array('create forum content', 'delete any forum content', 'view own unpublished content'), '17 delete any content');
    $this->delete_own_content_rid = (int) $this->drupalCreateRole(array('create forum content', 'delete own forum content', 'edit own comments'), '18 delete own content');  // EOC should not make any difference!
    $this->create_content_rid = (int) $this->drupalCreateRole(array('create forum content'), '19 create content');
    $this->anon_rid = DRUPAL_ANONYMOUS_RID;
    $this->auth_rid = DRUPAL_AUTHENTICATED_RID;

    // Create our users.
    $this->admin_user = $this->drupalCreateNamedUser('10_Administrator', array($this->admin_rid));
    $this->webmaster_user = $this->drupalCreateNamedUser('11_Webmaster', array($this->webmaster_rid));
    $this->forum_admin_user = $this->drupalCreateNamedUser('12_Forum_admin', array($this->forum_admin_rid));
    $this->edndel_any_content_user = $this->drupalCreateNamedUser('13_EdNDel_any_content', array($this->edndel_any_content_rid));
    $this->edndel_own_content_user = $this->drupalCreateNamedUser('14_EdNDel_own_content', array($this->edndel_own_content_rid));
    $this->edit_any_content_user = $this->drupalCreateNamedUser('15_Edit_any_content', array($this->edit_any_content_rid));
    $this->edit_own_content_user = $this->drupalCreateNamedUser('16_Edit_own_content', array($this->edit_own_content_rid));
    $this->delete_any_content_user = $this->drupalCreateNamedUser('17_Delete_any_content', array($this->delete_any_content_rid));
    $this->delete_own_content_user = $this->drupalCreateNamedUser('18_Delete_own_content', array($this->delete_own_content_rid));
    $this->create_content_user = $this->drupalCreateNamedUser('19_Create_content', array($this->create_content_rid));
    $this->auth_user = $this->drupalCreateNamedUser('20_Auth_only', array());
    $this->moderator = $this->drupalCreateNamedUser('21_Moderator', array($this->create_content_rid));

    $anon = drupal_anonymous_user();
    $anon->name = check_plain(format_username($anon));
    $this->accounts = array(
      $this->user1, $this->admin_user, $this->webmaster_user, $this->forum_admin_user,
      $this->edndel_any_content_user, $this->edndel_own_content_user,
      $this->edit_any_content_user, $this->edit_own_content_user,
      $this->delete_any_content_user, $this->delete_own_content_user,
      $this->create_content_user, $this->auth_user, $this->moderator,
    );
    $this->rids = array(
      $this->anon_rid, $this->auth_rid, $this->admin_rid,
      $this->webmaster_rid, $this->forum_admin_rid, $this->edndel_any_content_rid, $this->edndel_own_content_rid,
      $this->edit_any_content_rid, $this->edit_own_content_rid,
      $this->delete_any_content_rid, $this->delete_own_content_rid, $this->create_content_rid,
    );
    // Show settings for reference.
    $this->drupalGet('admin/people/permissions/list');
    $this->assertResponse(200, '^^^ Permissions');
    $this->drupalGet('admin/people', array('query' => array('sort' => 'asc', 'order' => drupal_encode_path(t('Username')))));
    $this->assertResponse(200, '^^^ Users');
  }

  function testForum() {
    // Skip test in the base class; too bad we can't skip the unnecessary setUp() call...
    $this->pass('==========================<br />Empty testForum() @' . (time() - $this->time));
  }

  function testAddOrphanTopic() {
    // Skip test in the base class; too bad we can't skip the unnecessary setUp() call...
    $this->pass('==========================<br />Empty testAddOrphanTopic() @' . (time() - $this->time));
  }

  function testEnableForumField() {
    // Skip test in the base class; too bad we can't skip the unnecessary setUp() call...
    $this->pass('==========================<br />Empty testEnableForumField() @' . (time() - $this->time));
  }


  /**
   * Asserts that a field in the current page is enabled.
   *
   * @param $id
   *   Id of field to assert.
   * @param $message
   *   Message to display.
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertFieldEnabled($id, $message = '') {
    $elements = $this->xpath('//input[@id=:id]', array(':id' => $id));
    return $this->assertTrue(isset($elements[0]) && empty($elements[0]['disabled']), $message ? $message : t('Field @id is enabled.', array('@id' => $id)), t('Browser'));
  }

  /**
   * Asserts that a field in the current page is disabled.
   *
   * @param $id
   *   Id of field to assert.
   * @param $message
   *   Message to display.
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertFieldDisabled($id, $message = '') {
    $elements = $this->xpath('//input[@id=:id]', array(':id' => $id));
    return $this->assertTrue(isset($elements[0]) && !empty($elements[0]['disabled']), $message ? $message : t('Field @id is disabled.', array('@id' => $id)), t('Browser'));
  }

  /**
   * Pass if a button with the specified label is found, and optional with the
   * specified index.
   *
   * @param $label
   *   Text in the value attribute.
   * @param $index
   *   Link position counting from zero.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to, defaults to 'Other'.
   * @return
   *   TRUE if the assertion succeeded, FALSE otherwise.
   */
  protected function assertButton($label, $index = 0, $message = '', $group = 'Other') {
    $links = $this->xpath('//input[contains(@type, submit)][contains(@value, :label)]', array(':label' => $label));
    $message = ($message ?  $message : t('Button with label %label found.', array('%label' => $label)));
    return $this->assert(isset($links[$index]), $message, $group);
  }

  /**
   * Pass if a button with the specified label is not found.
   *
   * @param $label
   *   Text in the value attribute.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to, defaults to 'Other'.
   * @return
   *   TRUE if the assertion succeeded, FALSE otherwise.
   */
  protected function assertNoButton($label, $message = '', $group = 'Other') {
    $links = $this->xpath('//input[contains(@type, submit)][contains(@value, :label)]', array(':label' => $label));
    $message = ($message ?  $message : t('Button with label %label found.', array('%label' => $label)));
    return $this->assert(empty($links), $message, $group);
  }

  /**
   * Extend drupalCreateUser() base method to accept a name as well as
   * multiple roles (rather than permissions).
   *
   * @param $name
   *   Name to assign to the user
   * @param $rids
   *   Array of Role IDs to assign to user.
   * @return
   *   A fully loaded user object with pass_raw property, or FALSE if account
   *   creation fails.
   */
  protected function drupalCreateNamedUser($name, $rids = array()) {
    // Create a user.
    $rids2 = array();
    foreach ($rids as $rid) {
      $rids2[$rid] = $rid;
    }
    $edit = array();
    $edit['name']   = $name;
    $edit['mail']   = $edit['name'] . '@example.com';
    $edit['roles']  = $rids2;
    $edit['pass']   = user_password();
    $edit['status'] = 1;

    $account = user_save(drupal_anonymous_user(), $edit);

    $this->assertTrue(!empty($account->uid), t('User %name created, uid=%uid.', array('%name' => $edit['name'], '%uid' => $account->uid)), t('User login'));
    if (empty($account->uid)) {
      return FALSE;
    }

    // Add the raw password so that we can log in as this user.
    $account->pass_raw = $edit['pass'];
    return $account;
  }

  protected function createForumTopicWithTitle($forum, $title) {
    $node = $this->createForumTopic((array) $forum);
    $node = node_load($node->nid);
    $node->title = $title;
    $node->shadow = FALSE;
    node_save($node);
    return $node;
  }

  protected function createForumCommentWithText($node, $text) {
    static $cid = 0;
    $this->drupalPost("node/$node->nid", array(
      'comment_body[' . LANGUAGE_NONE . '][0][value]' => $text,
    ), t('Save'));
    $this->assertResponse(200);
    $this->assertText($text, "Comment '$text' found, too.");
    return comment_load(++$cid);
  }

  protected function isFieldChecked($id) {
    $elements = $this->xpath('//input[@id=:id]', array(':id' => $id));
    return isset($elements[0]) && !empty($elements[0]['checked']);
  }


  /**
   * Check the set of permissions in one forum.
   *
   * @param $forum
   *  An associative array describing the forum.
   * @param $is_default
   *  Set to TRUE if this is the default forum (without any moderator).
   */
  function checkForum($forum, $is_default = FALSE) {
    $this->drupalLogin($this->user1);

    $this->drupalGet("admin/structure/forum/edit/forum/$forum->tid");
    $this->assertResponse(200, "^^^ '$forum->name' exists.");

    foreach ($this->accounts as $key => $account) {
      // Retrieve the access settings for this account.
      $account->access = array();
      foreach ($account->roles as $rid => $role_name) {
        foreach ($this->accesses as $access) {
          if ($this->isFieldChecked("edit-forum-access-grants-checkboxes-$access-$rid")) {
            $account->access[$access] = $access;
          }
        }
      }
    }

    foreach ($this->accounts as $key => $account) {
      // Create a topic and a comment for this account to experiment with.
      $account->node = $this->createForumTopicWithTitle($forum, "Topic for $account->name");
      $account->comment = $this->createForumCommentWithText($account->node, "Comment for $account->name");
    }
    // Show the topic list.
    $this->drupalGet("forum/$forum->tid");
    $this->assertResponse(200, "^^^ '$forum->name' initial topics.");

    foreach ($this->accounts as $key => $account) {
      $is_super_user = user_access('bypass node access', $account) || ($account->uid == $this->moderator->uid && !$is_default);

      if (!empty($account->uid)) {
        $this->drupalLogin($account);
      }
      else {
        $this->drupalLogout();
      }

      // Check whether we have an 'Add new Forum topic' link.
      $this->drupalGet('forum');

      if (empty($account->access['view']) && !$is_super_user) {
        $this->assertResponse(403, "^^^ $account->name cannot see the Forum Overview");
      }
      elseif ((empty($account->access['create']) || !user_access('create forum content', $account)) && !$is_super_user) {
        $this->assertResponse(200, 'Forum Overview');
        $this->assertNoLink(t('Add new Forum topic'), "^^^ $account->name cannot post in the '$forum->name'.");
      }
      else {
        $this->assertResponse(200, 'Forum Overview');
        $this->assertLink($forum->name, 0, "^^^ $account->name can see the '$forum->name'.");
        $this->assertLink(t('Add new Forum topic'), 0, "^^^ $account->name can post in the '$forum->name'.");
      }

      foreach (array('any', 'own') as $test_type) {

        // Check whether we can View our topic.
        $comment = &$account->comment;
        $node = &$account->node;
        if ((empty($account->access['view']) || !user_access('access content', $account)) && !$is_super_user) {
          $this->drupalGet("forum/$forum->tid");
          $this->assertResponse(404, "^^^ $account->name cannot access '$forum->name'.");
          $this->drupalGet("node/$node->nid");
          $this->assertResponse(403, "^^^ $account->name cannot access $test_type topic.");
          $this->drupalGet("node/$node->nid/edit");
          $this->assertResponse(403, "$account->name cannot edit $test_type topic (not accessible).");
          $this->drupalGet("comment/$comment->cid");
          $this->assertResponse(403, "^^^ $account->name cannot access comment '$comment->subject'.");
        }
        else {
          $this->drupalGet("forum/$forum->tid");
          $this->assertResponse(200, "^^^ '$forum->name' as $account->name.");

          $this->assertLink($node->title);
          $this->clickLink($node->title);
          $this->assertResponse(200, "^^^ $account->name can access $test_type topic.");
          $this->assertText($comment->subject, "Comment '$comment->subject' found, too.");


          // Check comment visibility.
          if (!$is_super_user && (!user_access('access comments', $account) || empty($account->access['view'])) && !user_access('administer comments', $account)) {
            $this->assertNoLinkByHref("/comment/$comment->cid#comment-$comment->cid");
            $this->drupalGet("comment/$comment->cid");
            $this->assertResponse(403, "^^^ $account->name cannot see comment '$comment->subject'.");
          }
          else {
            $this->assertLinkByHref(url("comment/$comment->cid", array('fragment' => "comment-$comment->cid")));
            // Check post comment / reply link.
            if (((!user_access('post comments', $account) && !user_access('post comments without approval', $account)) || empty($account->access['create'])) && !$is_super_user) {
              if (!$account->uid) {
                $this->assertLinkByHref("/user/login?destination=node/$node->nid#comment-form");
              }
              $this->assertNoLink(t('Add new comment'));
              $this->assertNoText(t('Add new comment'));
              $this->assertNoLink(t('reply'));
              $this->drupalGet("comment/$comment->cid");
              $this->assertResponse(200, '^^^ ' . "Comment '$comment->subject' is visible to $account->name'.");
              $this->drupalGet("comment/reply/$node->nid");
              $this->assertResponse(403);
              $this->drupalGet("comment/reply/$node->nid/$comment->cid");
              $this->assertResponse(403);
            }
            else {
              $this->assertText(t('Add new comment'));
              $this->assertLink(t('reply'));
              $this->assertLinkByHref("comment/reply/$node->nid/$comment->cid");
              $this->drupalGet("comment/reply/$node->nid/$comment->cid");
              $this->assertResponse(200);
            }

            // Check comment edit links.
            global $user;
            drupal_save_session(FALSE);
            $user_save = $user;
            $user = $account;
            // We ignore the 'edit own comments' permission!
            $comment_access_edit = FALSE;  // comment_access('edit', $comment);
            $user = $user_save;
            drupal_save_session(TRUE);
            $this->drupalGet("comment/$comment->cid");
            $this->assertResponse(200);
            if (empty($account->access['update']) && !$is_super_user && !$comment_access_edit && !user_access('administer comments', $account) && !user_access('edit any forum content', $account) && !($account->uid == $comment->uid && user_access('edit own forum content', $account))) {
              $this->assertNoLink(t('edit'));
              $this->drupalGet("comment/$comment->cid/edit");
              $this->assertResponse(403);
            }
            else {
              $this->assertLink(t('edit'));
              $this->clickLink(t('edit'));
              $this->assertResponse(200);
              $this->drupalGet("comment/$comment->cid/edit");
              $this->assertResponse(200);
              $this->assertText($comment->subject);
              $comment->title .= ' (updated)';
              $this->drupalPost("comment/$comment->cid/edit", array(
                'subject' => $comment->subject,
              ), t('Save'));
              $this->assertText(t("Your comment has been posted."));  // It ought to say 'updated'!
            }

            // Check comment delete links.
            $this->drupalGet("comment/$comment->cid");
            if ((empty($account->access['delete'])) && !$is_super_user && !user_access('administer comments', $account) && !user_access('delete any forum content', $account) && !($account->uid == $comment->uid && user_access('delete own forum content', $account))) {
              $this->assertNoLink(t('delete'));
              $this->drupalGet("comment/$comment->cid/delete");
              $this->assertResponse(403);
            }
            else {
              $this->assertText($comment->subject);
              $this->assertLink(t('delete'));
              $this->clickLink(t('delete'));
              $this->assertResponse(200);
              $this->drupalGet("comment/$comment->cid/delete");
              $this->assertResponse(200);
              $this->drupalPost("comment/$comment->cid/delete", array(), t('Delete'));
              $this->assertText(t('The comment and all its replies have been deleted.'));
              $this->assertNoText($comment->subject);
              unset($account->comment);
            }
          }

          // Check whether we can Edit our topic.
          $this->drupalGet("node/$node->nid");
          $this->assertResponse(200);
          if (empty($account->access['update']) && !user_access('edit any forum content', $account) &&
              !(user_access('edit own forum content', $account) && $node->uid == $account->uid) &&
              !$is_super_user) {
            $this->assertNoLink(t('Edit'));
            $this->drupalGet("node/$node->nid/edit");
            $this->assertResponse(403, "$account->name cannot edit $test_type topic.");
          }
          else {
            $this->assertLink(t('Edit'));
            $this->clickLink(t('Edit'));
            $this->assertResponse(200, "^^^ $account->name can edit $test_type topic.");

            // Check that moderator gets administrator properties.
            if ($is_super_user || user_access('administer nodes', $account)) {
              $this->assertText(t('Revision information'), "$account->name sees Revision information.");
              $this->assertText(t('Comment settings'), "$account->name sees Comment settings.");
              $this->assertText(t('Publishing options'), "$account->name sees Publishing options.");
              if (user_access('administer nodes', $account)) {
                $this->assertText(t('Menu settings'), "$account->name sees Menu settings.");
                $this->assertText(t('Authoring information'), "$account->name sees Authoring information.");
              }
              else {
                $this->assertNoText(t('Menu settings'), "$account->name does not see Menu settings.");
                $this->assertNoText(t('Authoring information'), "$account->name does not see Authoring information.");
              }
            }
            else {
              $this->assertNoText(t('Revision information'), "$account->name does not see Revision information.");
              $this->assertNoText(t('Comment settings'), "$account->name does not see Comment settings.");
              $this->assertNoText(t('Publishing options'), "$account->name does not see Publishing options.");
              $this->assertNoText(t('Menu settings'), "$account->name does not see Menu settings.");
              $this->assertNoText(t('Authoring information'), "$account->name does not see Authoring information.");
            }

            // Check whether we can Delete our topic.
            if (empty($account->access['delete']) && !user_access('delete any forum content', $account) &&
                !(user_access('delete own forum content', $account) && $node->uid == $account->uid) &&
                !$is_super_user) {
              $this->assertNoButton(t('Delete'), 0, $account->name . ' has no Delete button.');
            }
            else {
              $this->assertButton(t('Delete'), 0, $account->name . ' has a Delete button.');
            }

            // Change the title.
            $node->title = $node->title . ' (changed)';
            $this->drupalPost("node/$node->nid/edit", array(
              'title' => $node->title,
            ), t('Save'));
            $this->assertText(t('Forum topic !title has been updated.', array('!title' => $node->title)));
          }

          // Check whether we can delete the topic.
          if (empty($account->access['delete']) && !user_access('delete any forum content', $account) &&
              !(user_access('delete own forum content', $account) && $node->uid == $account->uid) &&
              !$is_super_user) {
            $this->drupalGet("node/$node->nid/delete");
            $this->assertResponse(403, "$account->name cannot delete $test_type topic.");
          }
          else {
            $this->drupalPost("node/$node->nid/delete", array(), t('Delete'));
            $this->assertText(t('Forum topic !title has been deleted.', array('!title' => $node->title)));
          }
        }

        if ($test_type == 'any' && (!empty($account->access['view']) || $is_super_user)) {
          // Check whether we can create a topic.
          if ((empty($account->access['create']) || !user_access('create forum content', $account)) && !$is_super_user) {
            $this->drupalGet('forum');
            if (empty($account->uid)) {
              $this->assertLinkByHref('/user/login?destination=forum');
            }
            else {
              $this->assertResponse(200, "^^^ $account->name can see the Forum Overview, but...");
              $this->assertText(t('You are not allowed to post new content in the forum.'));
            }
            $this->drupalGet("node/add/forum/$forum->tid");
            $this->assertResponse(403, "^^^ $account->name cannot create a forum topic in '$forum->name'.");
            break;
          }
          else {
            $this->drupalGet('forum');
            $this->assertNoText(t('You are not allowed to post new content in the forum.'));
            $this->assertLink(t('Add new Forum topic'));
            $this->clickLink(t('Add new Forum topic'));
            $this->assertResponse(200, "^^^ $account->name can create a forum topic.");
            $this->drupalGet("node/add/forum/$forum->tid");
            $this->assertResponse(200, "^^^ $account->name can create a forum topic in '$forum->name'.");
            $this->drupalGet('forum');
            $this->assertLink(t('Add new Forum topic'));
            $this->drupalPost("node/add/forum/$forum->tid", array(
              'title' => "Topic 1 by $account->name",
            ), t('Save'));
            $node = $account->node = $this->createForumTopicWithTitle($forum, "Topic 2 by $account->name");
            $this->drupalGet('node/' . $node->nid );

            $account->comment = $this->createForumCommentWithText($node, "Comment by $account->name");

            $this->drupalGet("forum/$forum->tid");
            $this->assertResponse(200, "^^^ '$forum->name' as $account->name (own topic).");
          }
        }
      }
    }

    $this->drupalLogin($this->user1);
    $this->drupalGet("forum/$forum->tid");
    $this->assertResponse(200, "^^^ '$forum->name' remaining topics.");
  }


  function createFAForum($id, $tag, $description, array $accesses) {
    $edit = array(
      'name' => "Forum $id $tag",
      'description' => $description,
      'parent[0]' => 0,
      'weight' => '0',
    );
    $forum = (object) ($edit + array('tid' => 2));
    $edit["forum_access[grants][checkboxes][view][1]"] = FALSE;
    $edit["forum_access[grants][checkboxes][view][2]"] = FALSE;
    $edit["forum_access[grants][checkboxes][create][2]"] = FALSE;
    foreach (array($this->webmaster_rid, $this->forum_admin_rid, $this->edit_any_content_rid, $this->edit_own_content_rid, $this->create_content_rid, $this->admin_rid, $this->anon_rid, $this->auth_rid) as $rid) {
      foreach ($accesses as $access) {
        $key = "$access-$rid";
        if (array_search($key, array('update-3', 'delete-3')) === FALSE) {
          $edit["forum_access[grants][checkboxes][$access][$rid]"] = TRUE;
        }
      }
    }
    $this->drupalPost('admin/structure/forum/add/forum', $edit, t('Save'));
    $this->assertResponse(200, "'$forum->name' added.");

    // Set moderator.
    $acl_id = _forum_access_get_acl($forum->tid);
    $this->assertNotNull($acl_id);
    acl_add_user($acl_id, $this->moderator->uid);
    $this->assertTrue(acl_has_user($acl_id, $this->moderator->uid), t('User %moderator is moderator.', array('%user' => $this->moderator->uid)));

    // Show the result.
    $this->drupalGet("admin/structure/forum/edit/forum/$forum->tid");
    $this->assertResponse(200, "^^^ '$forum->name' exists, with these settings.");
    $this->drupalGet('forum');
    $this->assertResponse(200, 'Forum Overview');
    return $forum;
  }

  function createAndCheckForum($id, $tag, $description, array $accesses) {
    $this->setUp2();
    taxonomy_term_delete(1);
    $this->pass("#########################<br />#$id - $tag Configuration test @" . (time() - $this->time), '<a id="jump1" href="#jump2">/\<br />######<br />\/</a>');
    $forum = $this->createFAForum($id, $tag, $description, $accesses);
    $this->checkForum($forum);
    $this->pass("#########################<br />#$id - END $tag Configuration test @" . (time() - $this->time), '<a id="jump2" href="#jump3">/\<br />######<br />\/</a>');
  }

  /**
   * Makes Devel's dpm() work inside this test, if Devel is installed.
   */
  function dpm($input, $name = NULL) {
    if (module_exists('devel') && user_access('access devel information')) {
      $export = kprint_r($input, TRUE, $name);
      $trigger_error = 'trigger_error';
      $trigger_error($export);
    }
  }

  /**
   * Writes a krumo entry into the watchdog log, if Devel is installed.
   */
  function wpm($input, $name = NULL) {
    if (module_exists('devel') && user_access('access devel information')) {
      $export = kprint_r($input, TRUE, $name);
      watchdog('debug', $export);
    }
  }
}
