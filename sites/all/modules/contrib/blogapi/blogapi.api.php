<?php

/**
 * @file
 * Describe all available hooks
 */

/**
 * Define new service endpoint for new blog API
 *
 * @return array(
 *  'api_version' => ... Version of this API
 *  'type' => ... Type of service endpoint xmlrpc|rest|...
 *  'name' => ... Name of Blog API
 * )
 */
function hook_blogapi_info() {
  return array(
    'api_version' => 2,
    'type' => 'xmlrpc',
    'name' => 'Blogger',
  );
}

/**
 * Alter Blog API definition
 */
function hook_blogapi_info_alter(&$blogapi) {

}

/**
 * Implements hook_default_services_endpoint() for BlogAPI endpoints
 */
function hook_blogapi_default_services_alter(&$export) {

}

/**
 * Allow override XML response to BlogAPI apps
 */
function hook_blogapi_xmlrpc_response_alter(&$response) {

}

/**
 * Alter node data before it's updated via blogAPI node modification
 */
function hook_blogapi_node_edit(&$node) {

}

/**
 * Alter new node data before it's created via blogAPI node modification
 */
function hook_blogapi_node_create(&$node) {

}