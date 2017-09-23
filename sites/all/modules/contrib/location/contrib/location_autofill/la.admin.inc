<?php

/**
 * @file
 * Location autofill routines.
 */

define('NUM_EMPTY_ENTITIES_TO_DISPLAY', 25);

/**
 * Module's admin form.
 */
function la_admin($form, &$form_state) {
  $form = array();

  $count = la_info();

  if ($count !== NULL) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Autofill locations')
    );
    $form['submit_list'] = array(
      '#type' => 'submit',
      '#value' => t('Get empty locations')
    );
  }

  return $form;
}

/**
 * Admin form submit handler.
 */
function la_admin_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  // Run batch.
  if ($values['op'] == $values['submit']) {
    la_batch();
    return;
  }
  // Display entities with non filled locations.
  if ($values['op'] == $values['submit_list']) {
    $fields = field_info_fields();
    $entity_list = array();
    foreach ($fields as $fname => $fdata) {
      if ($fdata['type'] != 'location') {
        unset($fields[$fname]);
        continue;
      }
    }
    if (count($fields) == 0) {
      drupal_set_message(t('Nothing to process.'));
      return;
    }
    // Subquery for a list of all empty location's ids.
    $l_subquery = db_select('location', 'l')
      ->fields('l', array('lid'))
      ->condition('latitude', '0.000000')
      ->condition('longitude', '0.000000');
    foreach ($fields as $fname => $fdata) {
      $tbl_name = key(
        $fdata['storage']['details']['sql']['FIELD_LOAD_CURRENT']
      );
      $column_name = $fdata['storage']['details']['sql']['FIELD_LOAD_CURRENT'][$tbl_name]['lid'];
      $entities = db_select($tbl_name, 't')
        ->fields('t', array('entity_type', 'entity_id', $column_name))
        ->condition($column_name, $l_subquery, 'IN')
        ->range(0, NUM_EMPTY_ENTITIES_TO_DISPLAY)
        ->execute()
        ->fetchAllAssoc('entity_id');

      if (count($entities) == 0) {
        continue;
      }

      foreach ($entities as $entity_id => $edata) {
        $entity = entity_load($edata->entity_type, array($entity_id));
        $entity = array_shift($entity);
        $entity_uri = entity_uri($edata->entity_type, $entity);
        $entity_url = url(
          $entity_uri['path'],
          array(
            'absolute' => TRUE,
            'entity_type' => $edata->entity_type,
            'entity' => $entity
          )
        );
        $entity_link = l(
          t('View entity'),
          $entity_url,
          array('attributes' => array('target' => '_blank'))
        );
        $entity_edit_link = '';
        if ($edata->entity_type == 'node') {
          $node_url = url(
            'node/' . $entity_id . '/edit',
            array(
              'absolute' => TRUE,
              'entity_type' => $edata->entity_type,
              'entity' => $entity
            )
          );
          $node_edit_url = l(
            t('Edit node'),
            $node_url,
            array('attributes' => array('target' => '_blank'))
          );
          $entity_edit_link = $node_edit_url;
        }
        $node_edit_url = '';
        $entity_list[] = array(
          $edata->entity_type,
          $entity_id,
          $edata->{$column_name},
          $entity_link,
          $entity_edit_link
        );
      }

      $ecount = count($entity_list);
      if ($ecount >= NUM_EMPTY_ENTITIES_TO_DISPLAY) {
        break;
      }
    }

    drupal_set_message(
      t(
        'Entities with empty locations (latitude and longitude): @count of @total. Use entity urls for manual access and edit.',
        array(
          '@count' => count($entity_list),
          '@total' => $l_subquery->execute()->rowCount()
        )
      ) . theme(
        'table',
        array(
          'header' => array(
            t('Entity type'),
            t('Entity ID'),
            t('Location ID'),
            t('Entity URL'),
            t('Entity edit URL')
          ),
          'rows' => $entity_list
        )
      )
    );
  }
}
