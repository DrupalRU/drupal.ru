<?php

/**
 * @file
 * Query extender and helper functions for pager queries.
 */

/**
 * Query extender for pager queries.
 */
class AltPager extends SelectQueryExtender {

  /**
   *
   */
  public function __construct(SelectQueryInterface $query, DatabaseConnection $connection) {
    parent::__construct($query, $connection);

    $this->addTag('altpager');
  }

  /**
   * Override the execute method.
   *
   * Before we run the query, we need to add pager-based range() instructions
   * to it.
   */
  public function execute() {
    $max_count = $this
      ->query
      ->countQuery()
      ->execute()
      ->fetchField();

    altpager_count_all_items($max_count);

    $count_items = altpager_count_items_viewed();

    $this->range(0, $count_items);

    return $this->query->execute();
  }

}
