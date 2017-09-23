<?php

/**
 * @file
 * Template for the Whois module's output method 'HTMLized status only'.
 */


  if (!$registered) {
    echo '<span class="whois-not-registered">' . t('The domain %domain is free.', array('%domain' => $address)) . '</span>';
  }
  else {
    echo '<span class="whois-registered">' . t('The domain %domain is registered.', array('%domain' => $address)) . '</span>';
  }


