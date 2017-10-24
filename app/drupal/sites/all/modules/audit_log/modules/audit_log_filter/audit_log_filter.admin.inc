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
 */
function audit_log_filter_by_entity_form($form, &$form_state) {
  $entity_info = entity_get_info();
  $entity_type_options = array();
  foreach ($entity_info as $entity_type => $info) {
    $entity_type_options[$entity_type] = $info['label'];
  }
  $form['audit_log_exclude_entity_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Exclude entity types'),
    '#description' => t('Select the entity types which should be excluded from logging'),
    '#options' => $entity_type_options,
    '#default_value' => variable_get('audit_log_exclude_entity_types', array()),
  );

  $form['audit_log_exclude_bundles'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Exclude bundles'),
    '#description' => t('Select the bundles per entity type which should be excluded from logging'),
  );
  $excluded_bundles = variable_get('audit_log_exclude_bundles', array());
  foreach ($entity_info as $entity_type => $info) {
    $entity_bundle_options = array();
    $default_value = array();
    foreach ($info['bundles'] as $bundle_name => $bundle_info) {
      $entity_bundle_options[$entity_type . ':' . $bundle_name] = $bundle_info['label'];
      $default_value[$entity_type . ':' . $bundle_name] = isset($excluded_bundles) && $excluded_bundles[$entity_type . ':' . $bundle_name] ? $entity_type . ':' . $bundle_name : 0;
    }
    $form['audit_log_exclude_bundles'][$entity_type] = array(
      '#type' => 'checkboxes',
      '#title' => t('Exclude @entity_type bundles', array('@entity_type' => $info['label'])),
      '#description' => t('Select the @entity_type bundles which should be excluded from logging', array('@entity_type' => $info['label'])),
      '#options' => $entity_bundle_options,
      '#states' => array(
        'visible' => array(
          ':input[name="audit_log_exclude_entity_types[' . $entity_type . ']"]' => array('checked' => FALSE),
        ),
      ),
      '#default_value' => $default_value,
    );
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
  variable_set('audit_log_exclude_entity_types', array_filter($form_state['values']['audit_log_exclude_entity_types']));
  $audit_log_exclude_bundles = array();
  foreach ($form_state['values']['audit_log_exclude_bundles'] as $values) {
    $audit_log_exclude_bundles += array_filter($values);
  }
  variable_set('audit_log_exclude_bundles', $audit_log_exclude_bundles);
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
