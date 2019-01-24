<?php
/**
 * @file
 * Hook implemenations for the Audit database logging module.
 */

/**
 * Render admin form to select roles.
 */
function audit_log_filter_by_role_form($form, &$form_state) {
  $form['audit_log_filter_exclude_roles'] = array(
    '#title' => t('Exclude roles'),
    '#description' => t('Select the roles which should be excluded from logging.'),
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#default_value' => variable_get('audit_log_filter_exclude_roles', array()),
  );
  return system_settings_form($form);
}

/**
 * Render admin form to select entity types and bundles.
 *
 * audit_log_exclude_entity_types
 *
 * - Entity1
 * -- Bundle 1
 * --- View
 * --- Insert
 * --- Update
 * --- Delete
 * -- Bundle 2
 * --- View
 * --- Insert
 * --- Update
 * --- Delete
 * - Entity2
 * -- Bundle 1
 * --- View
 * --- Insert
 * --- Update
 * --- Delete
 */
function audit_log_filter_by_entity_form($form, &$form_state) {
  $entity_info = entity_get_info();
  $actions = audit_log_action_options();
  $form['description'] = array(
    '#markup' =>  t('Select the Entity Type, Bundle, and/or action you would like to exclude from being logged below.'),
  );
  $form['audit_log_exclude_entity_types'] = array(
    '#type' => 'container',
    '#attached' =>  array('js' => array(drupal_get_path('module', 'audit_log_filter') . '/js/entity_filter.js')),
  );
  $alf_exclude = variable_get('audit_log_exclude_entity_types', array());
  foreach ($entity_info as $entity_type => $info) {
    // Selecting this will select all bundles and actions.
    $form['audit_log_exclude_entity_types'][$entity_type . '-all'] = array(
      '#type' => 'checkbox',
      '#title' => t($info['label']),
      '#default_value' => isset($alf_exclude[$entity_type . '-all']) ? $alf_exclude[$entity_type . '-all'] : FALSE,
    );
    // This makes the form appear as a "tree" and gives an array structure.
    $form['audit_log_exclude_entity_types'][$entity_type] = array(
      '#type' => 'container',
      '#attributes' => array('style' => 'padding-left:2em;'),
    );
    foreach ($info['bundles'] as $bundle_name => $bundle_info) {
      // Selecting this will select all actions for the bundle.
      $form['audit_log_exclude_entity_types'][$entity_type][$bundle_name . '-all'] = array(
        '#type' => 'checkbox',
        '#title' => t($bundle_info['label']),
        '#default_value' => isset($alf_exclude[$entity_type][$bundle_name . '-all']) ? $alf_exclude[$entity_type][$bundle_name . '-all'] : FALSE ,
      );
      // This makes the actions appear to branch out further than the bundles.
      // also like above... makes a pretty array.
      $form['audit_log_exclude_entity_types'][$entity_type][$bundle_name] = array(
        '#type' => 'container',
        '#attributes' => array('style' => 'padding-left:2em;'),
      );
      foreach ($actions as $action_name => $action_label) {
      $form['audit_log_exclude_entity_types'][$entity_type][$bundle_name][$action_name] = array(
          '#type' => 'checkbox',
          '#title' => t($action_label),
          '#default_value' => isset($alf_exclude[$entity_type][$bundle_name][$action_name]) ? $alf_exclude[$entity_type][$bundle_name][$action_name] : FALSE,
        );
      }
    }
  }
  $form['#tree'] = TRUE;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Form submit callback.
 */
function audit_log_filter_by_entity_form_submit($form, &$form_state) {
  $actions = audit_log_action_options();
  $alf_exclude = &$form_state['values']['audit_log_exclude_entity_types'];
  $entity_info = entity_get_info();
  foreach ($entity_info as $entity_type => $info) {
    foreach ($info['bundles'] as $bundle_name => $bundle_info) {
      // If the Entity is selected, all bundles will be selected.
      if (isset($alf_exclude[$entity_type . '-all']) && TRUE == $alf_exclude[$entity_type . '-all']) {
        $alf_exclude[$entity_type][$bundle_name . '-all'] = TRUE;
      }
      foreach ($actions as $action_name => $action_label) {
        if (isset($alf_exclude[$entity_type][$bundle_name . '-all']) && TRUE == $alf_exclude[$entity_type][$bundle_name . '-all']) {
          $alf_exclude[$entity_type][$bundle_name][$action_name] = TRUE;
        }
      }
    }
  }
  variable_set('audit_log_exclude_entity_types', $alf_exclude);

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Render admin form to select environments.
 */
function audit_log_filter_by_cli_form($form, &$form_state) {
  $form['audit_log_exclude_cli'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude cli'),
    '#description' => t('If checked, there will be no logging if Drupal is called form the command line interface.'),
    '#default_value' => variable_get('audit_log_exclude_cli', FALSE),
  );

  $form['audit_log_exclude_drush'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude drush'),
    '#description' => t('If checked, there will be no logging if Drupal is called form the command line interface through drush.'),
    '#default_value' => variable_get('audit_log_exclude_drush', FALSE),
  );

  return system_settings_form($form);
}
