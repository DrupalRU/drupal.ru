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

<div class="node node-event node-teaser well-lg">
    <h2 class="title"><?php print $fields['title']->content; ?>
      <small class="event-date"><?php print $fields['field_event_date']->content; ?></small>
    </h2>
    <div class="row">
      <div class="col-sm-4 col-sd-3">
          <div class="image">
              <?php print $fields['field_event_image']->content; ?>
          </div>
      </div>
      <div class="col-sm-8 col-sd-9">
          <dl class="alert alert-info">
              <dd class="website"><?php print $fields['field_event_link']->content; ?></dd>
              <dd class="address"><?php print $fields['field_event_address']->content; ?></dd>
              <dd class="event-type"><?php print $fields['field_event_type']->content; ?></dd>
          </dl>
      </div>
    </div>
    <div class="row">
        <div class="col-xs-12">
            <?php print $fields['body']->content; ?>
        </div>
    </div>
</div>
