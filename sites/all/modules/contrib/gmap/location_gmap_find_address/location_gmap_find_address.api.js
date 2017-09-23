/**
 * Documents the JS hooks.
 */

/**
 * Allows to alter the address parts of a location address before being geo-coded.
 *
 * @param object event
 * @param object params
 *   Data to handle:
 *   - addressParts: An object containing all the address parts
 *     that are present on the location widget form, which includes:
 *     name, street, additional, city, province, postal-code, country.
 *   - separator: The string used to concatenate the address parts.
 */
$(document).on('location_gmap_find_address.address_parts_alter', function(event, params) {
  // Remove the name from the address parts object, so it is not
  // used in geo-coding process.
  if (typeof params.addressParts !== 'undefined' &&
    typeof params.addressParts.name !== 'undefined'
  ) {
    delete params.addressParts.name;
  }
  params.separator = ', ';
});

