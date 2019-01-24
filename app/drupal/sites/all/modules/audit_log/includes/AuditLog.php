<?php
/**
 * Contains AuditLog.
 */

/**
 * Represents an AuditLog object.
 */
class Auditlog {
  /**
   * The user ID of the user performing the action.
   *
   * @var int
   */
  public $uid;

  /**
   * The role IDs of the user at the time (s)he performed the action.
   *
   * @var int[]
   */
  public $role_ids;

  /**
   * The user name of the user at the time (s)he performed the action.
   *
   * @var string
   */
  public $name;

  /**
   * The (relative) url at which the action was performed.
   *
   * @var string
   */
  public $url;

  /**
   * The entity id of the entity on which the action was performed.
   *
   * @var int
   */
  public $entity_id;

  /**
   * The entity type of the entity on which the action was performed.
   *
   * @var string
   */
  public $entity_type;

  /**
   * The bundle of the entity on which the action was performed.
   *
   * @var string
   */
  public $bundle;

  /**
   * The label of the entity on which the action was performed, at the time it
   * occured.
   *
   * @var string
   */
  public $entity_label;

  /**
   * The action that was performed.
   *
   * One of 'view', 'insert', 'update' or 'delete'.
   *
   * @var string
   */
  public $audit_action;

  /**
   * The timestamp when the action was performed.
   *
   * @var int
   */
  public $timestamp;

  /**
   * Message.
   *
   * A message describing the log item.
   *
   * @var string
   */
  public $message;

  /**
   * Create an audit log.
   *
   * @param array $values
   *   An array where the keys are the audit log's property names and the values
   *   are the corresponding values.
   *   Valid keys are:
   *     - entity_id: The entity id of the entity on which the action was
   *       performed (required).
   *     - entity_type: The entity type of the entity on which the action was
   *       performed (required).
   *     - entity_label: The label of the entity on which the action was
   *       performed, at the time it occured (required).
   *     - bundle: The bundle of the entity on which the action was performed
   *       (required).
   *     - audit_action: The action that was performed (required, one of 'view',
   *       'insert', 'update' or 'delete').
   *     - uid: The user ID of the user performing the action (optional,
   *       defaults to the current user).
   *     - role_ids:  The role IDs of the user at the time (s)he performed the
   *       action (optional, defaults to the role ids of the provided uid).
   *     - url: The (relative) url at which the action was performed (optional,
   *       defaults to request_path()).
   *     - timestamp: The timestamp when the action was performed (optional,
   *       defaults to REQUEST_TIME).
   *     - message: Message describing the log item (optional).
   */
  public function __construct($values = array()) {
    if (!isset($values['uid'])) {
      global $user;
      $values = array(
        'uid' => $user->uid,
        'role_ids' => array_keys($user->roles),
        'name' => user_is_anonymous() ? variable_get('anonymous', t('Anonymous')) : $user->name,
      ) + $values;
    }
    if (!isset($values['name'])) {
      $account = user_load($values['uid']);
      $values['name'] = $account->name;
    }
    if (!isset($values['role_ids'])) {
      $account = user_load($values['uid']);
      $values['role_ids'] = array_keys($account->roles);
    }
    $values += array(
      'url' => request_path(),
      'timestamp' => REQUEST_TIME,
    );
    foreach ($values as $property => $value) {
      $this->{$property} = $value;
    }
  }

  /**
   * Logs this audit log.
   *
   * @see hook_audit_log()
   */
  public function log() {
    module_invoke_all('audit_log', $this);
  }

}
