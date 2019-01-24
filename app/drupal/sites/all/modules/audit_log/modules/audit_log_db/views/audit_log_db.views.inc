<?php
/**
 * @file
 * Views hook implementations.
 */

/**
 * Implements hook_views_data().
 */
function audit_log_db_views_data() {
  $data = array();
  $data['audit_log'] = array(
    'table' => array(
      'base' => array(
        'field' => 'id',
        'title' => t('Audit Log'),
        'help' => t('Table that contains audit logs.'),
      ),
      'group' => t('Audit Log'),
    ),
    'id' => array(
      'title' => 'Id',
      'help' => 'Primary Key: Unique audit log ID.',
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'uid' => array(
      'title' => 'Uid',
      'help' => 'The users.uid that performed the action.',
      'relationship' => array(
        'title' => t('Author'),
        'help' => t('Relate audit log messages to the user who triggered it.'),
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'field' => 'uid',
        'label' => t('author'),
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_user_name',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'field' => array(
        'handler' => 'views_handler_field_user',
      ),
    ),
    'name' => array(
      'title' => 'Name',
      'help' => 'The user name at the time the action was performed.',
      'field' => array(
        'handler' => 'views_handler_field_user',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'url' => array(
      'title' => 'Url',
      'help' => 'The url on which the action was performed.',
      'field' => array(
        'handler' => 'views_handler_field_url',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'entity_id' => array(
      'title' => 'Entity Id',
      'help' => 'The entity id of the entity on which the action was performed.',
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'entity_type' => array(
      'title' => 'Entity Type',
      'help' => 'The entity type of the entity on which the action was performed.',
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'bundle' => array(
      'title' => 'Bundle',
      'help' => 'The bundle of the entity on which the action was performed.',
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'entity_label' => array(
      'title' => 'Entity Label',
      'help' => 'The label of the entity on which the action was performed.',
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'audit_action' => array(
      'title' => 'Audit Action',
      'help' => 'The action that was performed.',
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_in_operator',
        'options callback' => 'audit_log_action_options',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'timestamp' => array(
      'title' => 'Timestamp',
      'help' => 'The time the action was performed.',
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_date',
      ),
    ),
    'message' => array(
      'title' => 'Message',
      'help' => 'The message of the log item.',
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
  );

  $data['audit_log_roles'] = array(
    'table' => array(
      'group' => 'Audit Log',
      'join' => array(
        'audit_log' => array(
          'left_field' => 'id',
          'field' => 'audit_log_id',
        ),
      ),
    ),
    'audit_log_id' => array(
      'title' => 'Audit Log Id',
      'help' => 'Primary Key: audit_log.id for audit logs.',
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
    ),
    'role_id' => array(
      'title' => 'Roles',
      'help' => 'The roles of the user that triggered the audit log.',
      'field' => array(
        'handler' => 'views_handler_field_audit_log_roles',
        'no group by' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_user_roles',
        'numeric' => TRUE,
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_users_roles_rid',
        'name table' => 'role',
        'name field' => 'name',
        'empty field name' => t('No role'),
        'zero is null' => TRUE,
        'numeric' => TRUE,
      ),
    ),
  );
  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function audit_log_db_views_data_alter(&$data) {
  $base_table = 'audit_log';
  $data['users']['table']['join'][$base_table] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );

  if (module_exists('date_views')) {
    $data[$base_table]['date_argument'] = $data['node']['date_argument'];
    $data[$base_table]['date_argument']['title'] = t('Date (!base_table)', array('!base_table' => $base_table));
    $data[$base_table]['date_argument']['help'] = t('Filter any Views !base_table date field.', array('!base_table' => $base_table));

    $data[$base_table]['date_filter'] = $data['node']['date_filter'];
    $data[$base_table]['date_filter']['title'] = t('Date (!base_table)', array('!base_table' => $base_table));
    $data[$base_table]['date_filter']['help'] = t('Filter any Views !base_table date field.', array('!base_table' => $base_table));
  }
}
