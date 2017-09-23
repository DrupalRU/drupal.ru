<?php

/**
 * @file
 * Provides Devel generate integration for the birthday field type.
 */

/**
 * Implements hook_devel_generate().
 */
function birthdays_devel_generate($object, $field, $instance, $bundle) {
  // Leave some fields empty, if they are not required.
  if (empty($instance['required']) && rand(0, 5) == 0) {
    return;
  }

  // Randomly try finding a valid date.
  while (TRUE) {
    try {
      if (rand(0, 1)) {
        $year = rand(date('Y') - 90, date('Y') - 1);
      }
      else {
        $year = 0;
      }
      return BirthdaysBirthday::fromDate($year, rand(1, 12), rand(1, 31))->toArray() + array('triggers' => 1);
    }
    catch (InvalidArgumentException $e) { }
  }
}
