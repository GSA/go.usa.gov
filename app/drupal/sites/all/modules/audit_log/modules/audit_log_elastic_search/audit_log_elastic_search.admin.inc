<?php
/**
 * @file
 * Hook implemenations for the Audit elastic search logging module.
 */

/**
 * Render admin form.
 */
function audit_log_elastic_search_settings() {
  $form = array();

  $form['#old_cluster_id'] = variable_get('audit_log_elastic_search_cluster_id', array());

  $form['audit_log_elastic_search_cluster_id'] = array(
    '#type' => 'ec_index',
    '#title' => t('Select cluster'),
    '#required' => TRUE,
    '#default_value' => variable_get('audit_log_elastic_search_cluster_id', array()),
    '#description' => t('Select the settings in order to be able to hold the logs.'),
  );

  $form['audit_log_elastic_search_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Type name'),
    '#required' => TRUE,
    '#element_validate' => array('elasticsearch_connector_validate_type_field'),
    '#default_value' => variable_get('audit_log_elastic_search_type', ELASTICSEARCH_WATCHDOG_DEFAULT_TYPE),
    '#description' => t('Enter the elasticsearch type name you want to store the logs'),
  );

  $form['#validate'] = array('audit_log_elastic_search_settings_validate');

  return system_settings_form($form);
}

/**
 * Validate the setting form submission.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function audit_log_elastic_search_settings_validate(array $form, array &$form_state) {
  $values = $form_state['values'];
  if (!empty($form_state['triggering_element']['#ajax']) || empty($values['audit_log_elastic_search_cluster_id']['index'])) {
    return;
  }

  $client = elasticsearch_connector_get_client_by_id($values['audit_log_elastic_search_cluster_id']['cluster_id']);

  $index_name = audit_log_elastic_search_get_realindex_name($values['audit_log_elastic_search_cluster_id']['index']);
  $alias_name = audit_log_elastic_search_get_index_name($values['audit_log_elastic_search_cluster_id']['index']);
  $old_type_name = variable_get('audit_log_elastic_search_type', NULL);
  $type_name = audit_log_elastic_search_get_type_name($values['audit_log_elastic_search_type']);

  if ($client) {
    if (!$client->indices()->exists(array('index' => $index_name))) {
      form_set_error('audit_log_elastic_search_cluster_id', t("The index doesn't exists. Please created it before continue."));
    }
    else {
      if (!$client->indices()->existsType(array('index' => $index_name, 'type' => $type_name))) {
        audit_log_elastic_search_create_type($client, $index_name, $type_name);
      }

      // Alias.
      if (!$client->indices()->existsAlias(array('index' => $index_name, 'name' => $alias_name))) {
        try {
          audit_log_elastic_search_create_alias($client, $index_name, $alias_name);
        }
        catch (Exception $e) {
          error_log($e->getMessage());
        }
      }

      if (isset($old_type_name) && $old_type_name != $type_name) {
        try {
          $client->indices()->deleteMapping(array(
            'index' => $index_name,
            'type'  => $old_type_name,
          ));
          drupal_set_message(t('The old type @type has been successfully deleted.', array('@type' => $old_type_name)));
        }
        catch (Exception $e) {
          error_log($e->getMessage());
        }
      }
    }
  }
  else {
    form_set_error('audit_log_elastic_search_cluster_id', t('Cannot connect to the cluster for some reason! Please contact your system administrator.'));
  }
}

/**
 * Creates alias to the provided index.
 *
 * @param object $client
 *   The ElasticSearch client object.
 * @param string $index_name
 *   Name of the index to which the alias will be created.
 * @param string $alias_name
 *   Name of the alias that will be created to the provided index.
 */
function audit_log_elastic_search_create_alias($client, $index_name, $alias_name) {
  try {
    $client->indices()->putAlias(array(
      'name' => $alias_name,
      'index' => $index_name,
    ));
  }
  catch (Exception $e) {
    throw $e;
  }
}

/**
 * Default index mapping for the elasticsearch watchdog index.
 *
 * @param int $ttl
 *   Time To Live setting.
 *
 * @return array
 *   The mapping index array.
 */
function audit_log_elastic_search_get_mapping($ttl = NULL) {
  if (!isset($ttl)) {
    $ttl = variable_get('audit_log_elastic_search_ttl', AUDIT_LOG_ELASTIC_SEARCH_DEFAULT_INTERVAL);
  }

  $mapping = array(
    '_source' => array(
      'enabled' => TRUE,
    ),
    '_all' => array(
      'enabled' => FALSE,
    ),
    '_ttl' => array(
      'enabled' => TRUE,
      'default' => $ttl,
    ),
    'properties' => array(
      'uid' => array(
        'type' => 'integer',
      ),
      'name' => array(
        'type' => 'string',
        'index' => 'not_analyzed',
      ),
      'url' => array(
        'type' => 'string',
        'index' => 'not_analyzed',
      ),
      'entity_id' => array(
        'type' => 'integer',
      ),
      'entity_type' => array(
        'type' => 'string',
        'index' => 'not_analyzed',
      ),
      'bundle' => array(
        'type' => 'string',
        'index' => 'not_analyzed',
      ),
      'bundle' => array(
        'type' => 'string',
        'index' => 'not_analyzed',
      ),
      'entity_label' => array(
        'type' => 'string',
        'index' => 'not_analyzed',
      ),
      'audit_action' => array(
        'type' => 'string',
        'index' => 'not_analyzed',
      ),
      'timestamp' => array(
        'type' => 'long',
      ),
      'microtime' => array(
        'type' => 'float',
      ),
      'date' => array(
        'type' => 'date',
      ),
    ),
  );

  return $mapping;
}

/**
 * Create Elasticsearch watchdog type.
 */
function audit_log_elastic_search_create_type(Elasticsearch\Client $client, $index, $type) {
  $mapping_params['index'] = $index;
  $mapping_params['type'] = $type;
  $mapping = audit_log_elastic_search_get_mapping();
  $mapping_params['body'][$type] = $mapping;
  $client->indices()->putMapping(
      $mapping_params
  );
}
