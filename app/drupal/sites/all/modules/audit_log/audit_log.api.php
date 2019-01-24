<?php
/**
 * @file
 * API documentation for the Audit log module.
 */

/**
 * Decide if the item has to be logged.
 *
 * @param Auditlog $log
 *   The audit log that will be saved.
 * @param array $context
 *   An array with the following keys:
 *     - entity: The entity on which the action is performed.
 *     - entity_type: The entity type of the entity on which the action is
 *     performed.
 *     - action: The action that was performed ('view', 'insert', 'update' or
 *     'delete).
 *     - account: The user that performed the action.
 *     - url: The url on which the action was performed.
 *     - timestamp: The timestamp when the action was performed.
 * @param object $account
 *   The user object.
 *
 * @return const
 *   - AUDIT_LOG_DO_LOG: Log the item.
 *   - AUDIT_LOG_DO_NOT_LOG: Do not log the item.
 */
function hook_audit_log_it(Auditlog $log, array $context, $account) {
  if ($account->uid === 1) {
    return AUDIT_LOG_DO_NOT_LOG;
  }
  return AUDIT_LOG_DO_LOG;
}

/**
 * Act on an audit log being skipped.
 *
 * @param Auditlog $log
 *   The audit log being skipped.
 */
function hook_audit_log_skipped(Auditlog $log) {
  // Your logic.
}

/**
 * Alter an audit log before it is saved.
 *
 * @param Auditlog $log
 *   The audit log that will be saved.
 * @param array $context
 *   An array with the following keys:
 *     - entity: The entity on which the action is performed.
 *     - entity_type: The entity type of the entity on which the action is
 *     performed.
 *     - action: The action that was performed ('view', 'insert', 'update' or
 *     'delete).
 *     - account: The user that performed the action.
 *     - url: The url on which the action was performed.
 *     - timestamp: The timestamp when the action was performed.
 */
function hook_audit_log_alter(Auditlog &$log, array $context) {
  if ($context['entity_type'] == 'node' && $context['entity']->type == 'my_type') {
    $node = entity_metadata_wrapper($context['entity_type'], $context['entity']);
    $log->custom_value = $node->field_custom_value->value();
  }
}

/**
 * Save an audit log.
 *
 * @param Auditlog $log
 *   The audit log to be saved.
 */
function hook_audit_log(Auditlog $log) {
  $role_ids = $log->role_ids;

  drupal_write_record('audit_log', $log);
  foreach ($role_ids as $role_id) {
    $record = (object) array(
      'audit_log_id' => $log->id,
      'role_id' => $role_id,
    );
    drupal_write_record('audit_log_roles', $record);
  }
}

/**
 * Act on an audit log being inserted.
 *
 * @param Auditlog $log
 *   The audit log being inserted.
 */
function hook_audit_log_insert(Auditlog $log) {
  $record = (object) array(
    'log_id' => $log->id,
    'custom_value' => $log->custom_value,
  );
  drupal_write_record('log_custom_values', $record);
}

/**
 * Returns custom actions.
 */
function hook_audit_log_action_options() {
  return drupal_map_assoc(array('my-action'));
}
