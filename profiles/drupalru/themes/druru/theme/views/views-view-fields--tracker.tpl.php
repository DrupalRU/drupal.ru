<?php

/**
 * @file
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>

<?php

switch ($fields['taxonomy_vocabulary_8']->content) {
  case "Есть вопрос": $node_type_icon = "fa-question-circle-o"; break;
  case "Предлагаю решение": $node_type_icon = "fa-file-text-o"; break;
  default: $node_type_icon = "";
}

$link = "/node/" . $fields['nid']->content . "/#new";

empty($fields['new_comments_1']->content) ? $comment_icon = "label-default" : $comment_icon = "label-success";

?>

<a href='<?php print $link; ?>' class="node-item list-group-item clearfix">
  <span class="node-item--comments-stat label <?php print $comment_icon ?>"> <i class="icon fa fa-comment-o" aria-hidden="true"></i> <?php print $fields['comment_count']->content; ?> <?php print $fields['new_comments_1']->content; ?></span>
  <span class="node-item--title"> <i class='icon fa <?php print $node_type_icon ?>' aria-hidden="true"></i> <?php print $fields['title']->content; ?></span>
  <small class="node-item--author user-picture"><?php print $fields['name']->content; ?></small>
</a>

