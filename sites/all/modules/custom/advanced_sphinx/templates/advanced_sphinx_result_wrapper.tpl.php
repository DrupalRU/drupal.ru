<?php

/**
 * @file
 * Default theme implementation to wrap search result.
 *
 * Available variables:
 * - $result: Array with all data:
 *   - $result['stat']: General statistics search.
 *   - $result['word_stat']: Statistics search every word form.
 *   - $result['sorted']: Links to the sorting criteria.
 *   - $result['result']: List of result.
 */
?>
<?php print render($result['search_form']); ?>

<div class="search-stat">
  <?php print $result['stat']; ?>
  <?php if (isset($result['word_stat'])): ?>
    <?php print $result['word_stat']; ?>
  <?php endif; ?>
</div>

<?php if (isset($result['sorted'])): ?>
  <div
    class="search-sorted"><?php print t('Sort') . ': ' . $result['sorted']; ?></div>
<?php endif; ?>

<div id="advanced-search clear-block">
  <ul class="search-results">
    <?php print $result['result']; ?>
  </ul>
</div>

<?php if (isset($result['sorted'])): ?>
  <div
    class="search-sorted"><?php print t('Sort') . ': ' . $result['sorted']; ?></div>
<?php endif; ?>
