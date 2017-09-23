<?php
/**
 * @file
 * This is the administration settings for comment mover module.
 */

/**
 * Settings page for the comment mover module.
 */
function comment_mover_settings_form() {
  $form = array();

  $form['comment_mover_messages_inserting_place'] = array(
    '#title' => t('A selector for inserting a successfull message about cutting/pasting in comments'),
    '#description' => t("The script will will use the jQuery command '.after()' for inserting the messages."),
    '#type' => 'textfield',
    '#default_value' => variable_get('comment_mover_messages_inserting_place', '.comment-id-%cid'),
    '#required' => TRUE,
  );

  $form['comment_mover_messages_inserting_place_node'] = array(
    '#title' => t('A selector for inserting a successfull message about cutting/pasting in nodes'),
    '#description' => t("The script will will use the jQuery command '.after()' for inserting the messages."),
    '#type' => 'textfield',
    '#default_value' => variable_get('comment_mover_messages_inserting_place_node', '#node-%nid .link-wrapper'),
    '#required' => TRUE,
  );

  $form['comment_mover_flatcomments_change_cid'] = array(
    '#title' => t('Always add comments to the bottom of the thread when moving them to another node'),
    '#description' => t("When the style of comments is choosen as 'flat' and comments are moving from one node to another the cid of the comments would be changed and the inserted comments would be added to the bottom of the comments thread."),
    '#type' => 'checkbox',
    '#default_value' => variable_get('comment_mover_flatcomments_change_cid', TRUE),
  );

  return system_settings_form($form);
}
