<?php

/**
 * @file
 * Definition of views_handler_field_audit_log_roles.
 */

/**
 * Field handler to provide a list of roles.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_audit_log_roles extends views_handler_field_prerender_list {
  function construct() {
    parent::construct();
    $this->additional_fields['audit_log_id'] = array('table' => 'audit_log', 'field' => 'id');
  }

  function pre_render(&$values) {
    $this->field_alias = $this->aliases['audit_log_id'];
    $log_ids = array();
    $this->items = array();

    foreach ($values as $result) {
      if (!empty($result->{$this->aliases['audit_log_id']})) {
        $log_ids[] = $result->{$this->aliases['audit_log_id']};
      }
    }

    if ($log_ids) {
      $result = db_query("SELECT r.rid, r.name, ar.audit_log_id FROM {role} r INNER JOIN {audit_log_roles} ar ON r.rid = ar.role_id WHERE ar.audit_log_id IN (:log_ids) ORDER BY r.name",
        array(':log_ids' => $log_ids));
      foreach ($result as $role) {
        $this->items[$role->audit_log_id][$role->rid]['role'] = check_plain($role->name);
        $this->items[$role->audit_log_id][$role->rid]['rid'] = $role->rid;
      }
    }
  }

  function render_item($count, $item) {
    return $item['role'];
  }

  function document_self_tokens(&$tokens) {
    $tokens['[' . $this->options['id'] . '-role' . ']'] = t('The name of the role.');
    $tokens['[' . $this->options['id'] . '-rid' . ']'] = t('The role ID of the role.');
  }

  function add_self_tokens(&$tokens, $item) {
    if (!empty($item['role'])) {
      $tokens['[' . $this->options['id'] . '-role' . ']'] = $item['role'];
      $tokens['[' . $this->options['id'] . '-rid' . ']'] = $item['rid'];
    }
  }
}
