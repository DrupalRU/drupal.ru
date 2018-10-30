<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php foreach ($rows as $id => $row): ?>
<a href="<?php print "/node/" . $result[$id]->nid; ?>" class="node-item<?php print $classes_array[$id]; ?>">
  <?php print $row; ?>
</a>
<?php endforeach; ?>
