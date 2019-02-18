<?php

/**
 * @file
 * Custom Drupal 7 RequestMehod class for Google reCAPTCHA library.
 */

namespace ReCaptcha\RequestMethod;

use ReCaptcha\ReCaptcha;
use ReCaptcha\RequestMethod;
use ReCaptcha\RequestParameters;

/**
 * Sends POST requests to the reCAPTCHA service.
 */
class Drupal7Post implements RequestMethod {

  /**
   * Submit the POST request with the specified parameters.
   *
   * @param ReCaptcha\RequestParameters $params
   *   Request parameters.
   *
   * @return string
   *   Body of the reCAPTCHA response.
   */
  public function submit(RequestParameters $params) {

    $options = array(
      'headers' => array(
        'Content-type' => 'application/x-www-form-urlencoded',
      ),
      'method' => 'POST',
      'data' => $params->toQueryString(),
    );
    $response = drupal_http_request(ReCaptcha::SITE_VERIFY_URL, $options);

    if ($response->code == 200 && isset($response->data)) {
      // The service request was successful.
      return $response->data;
    }
    elseif ($response->code < 0) {
      // Negative status codes typically point to network or socket issues.
      return '{"success": false, "error-codes": ["' . ReCaptcha::E_CONNECTION_FAILED . '"]}';
    }
    else {
      // Positive none 200 status code typically means the request has failed.
      return '{"success": false, "error-codes": ["' . ReCaptcha::E_BAD_RESPONSE . '"]}';
    }
  }

}
