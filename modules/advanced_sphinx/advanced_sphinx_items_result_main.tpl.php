<?php
/**
 * @file
 * Default theme implementation to item search result.
 *
 * Available variables:
 * - $result: Array with all data:
 *   - $result['number']: Serial number search results.
 *   - $result['title']: Linked title to full node.
 *   - $result['excerpts']: .
 *   - $result['date']: Date and time of posting.
 *   - $result['username']: Linked login to node author.
 *   - $result['tax']: List of taxonomy term.
 */
?>
<li class="result-folded">
  <h3 class="title-result"><span
      class="number-result"><?php print $result['number']; ?>
      .</span> <?php print $result['title']; ?></h3>
  <?php if (isset($result['excerpts'])): ?>
    <div class="content-result">
      <?php print $result['excerpts']; ?>
    </div>
  <?php endif; ?>

  <div class="info-result">
    <?php if (isset($result['date'])): ?>
      <span class="date-result"><?php print $result['date']; ?>,</span>
    <?php endif; ?>

    <?php if (isset($result['username'])): ?>
      <span class="autor-result"><?php print $result['username']; ?></span>
    <?php endif; ?>

    <?php if (isset($result['tags'])): ?>
      <span
        class="tax-result"><?php print t('Tags') . ': ' . $result['tags']; ?></span>
    <?php endif; ?>
  </div>
</li>
