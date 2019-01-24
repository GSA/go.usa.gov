<?php
/**
 * @file
 * Exported views
 */

/**
 * Implements hook_views_default_views()
 */
function audit_log_db_views_default_views() {
  $view = new view();
  $view->name = 'audit_log';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'audit_log';
  $view->human_name = 'Audit Log';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Audit Log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'audit_log_db_view_admin';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'audit_action' => 'audit_action',
    'bundle' => 'bundle',
    'entity_id' => 'entity_id',
    'entity_label' => 'entity_label',
    'entity_type' => 'entity_type',
    'timestamp' => 'timestamp',
    'message' => 'message',
  );
  $handler->display->display_options['style_options']['default'] = 'timestamp';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'audit_action' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'bundle' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_label' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'message' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Audit Log: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'audit_log';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  /* Field: Audit Log: Audit Action */
  $handler->display->display_options['fields']['audit_action']['id'] = 'audit_action';
  $handler->display->display_options['fields']['audit_action']['table'] = 'audit_log';
  $handler->display->display_options['fields']['audit_action']['field'] = 'audit_action';
  /* Field: Audit Log: Bundle */
  $handler->display->display_options['fields']['bundle']['id'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['table'] = 'audit_log';
  $handler->display->display_options['fields']['bundle']['field'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['label'] = '';
  $handler->display->display_options['fields']['bundle']['exclude'] = TRUE;
  $handler->display->display_options['fields']['bundle']['element_label_colon'] = FALSE;
  /* Field: Audit Log: Entity Id */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'audit_log';
  $handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_id']['separator'] = '';
  /* Field: Audit Log: Entity Label */
  $handler->display->display_options['fields']['entity_label']['id'] = 'entity_label';
  $handler->display->display_options['fields']['entity_label']['table'] = 'audit_log';
  $handler->display->display_options['fields']['entity_label']['field'] = 'entity_label';
  $handler->display->display_options['fields']['entity_label']['label'] = 'Entity';
  $handler->display->display_options['fields']['entity_label']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['entity_label']['alter']['path'] = '[bundle]/[entity_id]';
  /* Field: Audit Log: Entity Type */
  $handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['table'] = 'audit_log';
  $handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
  /* Field: Audit Log: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'audit_log';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'raw time ago';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
  /* Field: Audit Log: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'audit_log';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  /* Filter criterion: Audit Log: Audit Action */
  $handler->display->display_options['filters']['audit_action']['id'] = 'audit_action';
  $handler->display->display_options['filters']['audit_action']['table'] = 'audit_log';
  $handler->display->display_options['filters']['audit_action']['field'] = 'audit_action';
  $handler->display->display_options['filters']['audit_action']['exposed'] = TRUE;
  $handler->display->display_options['filters']['audit_action']['expose']['operator_id'] = 'audit_action_op';
  $handler->display->display_options['filters']['audit_action']['expose']['label'] = 'Audit Action';
  $handler->display->display_options['filters']['audit_action']['expose']['operator'] = 'audit_action_op';
  $handler->display->display_options['filters']['audit_action']['expose']['identifier'] = 'audit_action';
  $handler->display->display_options['filters']['audit_action']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['audit_action']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    11 => 0,
    16 => 0,
  );
  /* Filter criterion: Audit Log: Entity Type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'audit_log';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_type']['expose']['operator_id'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['label'] = 'Entity Type';
  $handler->display->display_options['filters']['entity_type']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['entity_type']['expose']['operator'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['identifier'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Audit Log: Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'audit_log';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'User';
  $handler->display->display_options['filters']['name']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/reports/audit';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Audit Log';
  $handler->display->display_options['menu']['description'] = 'Show items in Audit log.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  return $views;
}
