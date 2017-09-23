<?php
/**
 * @file
 * Migrate integration.
 */

/**
 * Implements hook_migrate_api().
 */
function location_migrate_api() {
  $api = array(
    'api' => 2,
    'field handlers' => array(
      'MigrateLocationFieldHandler',
    ),
  );

  return $api;
}

/*
*
* Primary value passed to this field is the is_primary value (boolean)
* but can be overwritten with subfields.
*
* Subfields are used to specify all the other values:
*  'street'
*  'additional',
*  'city',
*  'province',
*  'postal_code',
*  'country',
*  'is_primary',
*  'latitude',
*  'longitude'
*
* @code
*   $this->addFieldMapping('field_location_dest')->defaultValue('TRUE');
*   $this->addFieldMapping('field_location_dest:street', 'field_source_address_1');
*   $this->addFieldMapping('field_location_dest:city', 'field_source_city');
* @endcode
*/

class MigrateLocationFieldHandler extends MigrateFieldHandler {

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    $this->registerTypes(array('location'));
  }

  /**
   * Implementation of MigrateFieldHandler::fields().
   *
   * @param string $type
   *   The file field type - 'file' or 'image'
   *
   * @param string $parent_field
   *   Name of the parent field.
   *
   * @param Migration $migration
   *   The migration context for the parent field. We can look at the mappings
   *   and determine which subfields are relevant.
   *
   * @return array
   *   Array of fields.
   */
  public function fields($type, $parent_field, $migration = NULL) {
    $dummy = array();
    $all_fields = location_invoke_locationapi($dummy, 'fields');
    unset($all_fields['locpick']);
    foreach ($all_fields as $field => $label) {
      $fields[$field] = t('Subfield: @label', array('@label' => $label));
    }
    $fields['latitude'] = t('Subfield: Latitude');
    $fields['longitude'] = t('Subfield: Longitude');
    $fields['source'] = t('Subfield: Source value');
    $fields['is_primary'] = t('Subfield: Is primary');

    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {
    $migration = Migration::currentMigration();
    $destination = $migration->getDestination();

    $arguments = array();
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }

    $language = $this->getFieldLanguage($entity, $field_info, $arguments);
    $return = array($language => array());

    foreach ($values as $delta => $value) {
      // Handle potentially multiple arguments.
      $instance_arguments = array();

      foreach ($arguments as $key => $argument) {
        // For a scalar argument, pass it directly.
        if (!is_array($argument)) {
          if ($delta == 0) {
            $instance_arguments[$key] = $argument;
          }
        }
        elseif (isset($argument[$delta])) {
          $instance_arguments[$key] = $argument[$delta];
        }
      }

      if (isset($instance_arguments['country'])) {
        $instance_arguments['country'] = strtolower($instance_arguments['country']);
      }
      // Ensure we have at least one location field besides 'is_primary' before saving.
      if (!empty($instance_arguments)) {
        $instance_arguments['is_primary'] = ($delta == 0);
        $instance_arguments['inhibit_geocode'] = TRUE;

        // Only save the location if saved successfully.
        if ($lid = location_save($instance_arguments)) {
          $return[$language][$delta]['lid'] = $lid;
        }
      }
    }

    return $return;
  }
}
