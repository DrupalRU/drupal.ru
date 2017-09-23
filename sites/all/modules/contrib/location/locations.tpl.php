<?php

/**
 * @file
 * Template for displaying locations.
 */
?>
<?php if (!empty($locations)): ?>
  <div class="location-locations-display">
    <h3 class="location-locations-header"><?php print format_plural(count($locations), 'Location', 'Locations'); ?></h3>

    <div class="location-locations-wrapper">
      <?php foreach ($locations as $location): ?>
        <?php print $location; ?>
      <?php endforeach; ?>
    </div>
  </div>
<?php endif; ?>
