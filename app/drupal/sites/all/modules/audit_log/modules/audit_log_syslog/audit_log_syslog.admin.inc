<?php
/**
 * @file
 * Admin settings for the Audit syslog logging module.
 */

/**
 * Render admin form.
 */
function audit_log_syslog_settings() {
  $form = array();

  $form['audit_log_syslog_log_severity'] = array(
    '#type'          => 'select',
    '#title'         => t('Syslog severity'),
    '#options'       => array(
      WATCHDOG_EMERGENCY => t('emergency'),
      WATCHDOG_ALERT => t('alert'),
      WATCHDOG_CRITICAL => t('critical'),
      WATCHDOG_ERROR => t('error'),
      WATCHDOG_WARNING => t('warning'),
      WATCHDOG_NOTICE => t('notice'),
      WATCHDOG_INFO => t('info'),
      WATCHDOG_DEBUG => t('debug'),
    ),
    '#default_value' => variable_get('audit_log_syslog_log_severity', LOG_NOTICE),
    '#description'   => t('Select severity level for the logging.'),
  );

  $form['audit_log_syslog_identity'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Syslog identity'),
    '#default_value' => variable_get('audit_log_syslog_identity', variable_get('syslog_identity', 'drupal')),
    '#description'   => t('A string that will be prepended to every message logged to Syslog. If you have multiple sites logging to the same Syslog log file, a unique identity per site makes it easy to tell the log entries apart.'),
  );

  if (defined('LOG_LOCAL0')) {
    $form['audit_log_syslog_facility'] = array(
      '#type'          => 'select',
      '#title'         => t('Syslog facility'),
      '#default_value' => variable_get('audit_log_syslog_facility', variable_get('syslog_facility', LOG_LOCAL0)),
      '#options'       => syslog_facility_list(),
      '#description'   => t('Depending on the system configuration, Syslog and other logging tools use this code to identify or filter messages from within the entire system log.'),
     );
  }

  $form['audit_log_syslog_format'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Syslog format'),
    '#default_value' => variable_get('audit_log_syslog_format', variable_get('syslog_format', '!base_url|!timestamp|!type|!ip|!request_uri|!referer|!uid|!link|!message')),
    '#description'   => t('Specify the format of the syslog entry. Available variables are: <dl><dt><code>!base_url</code></dt><dd>Base URL of the site.</dd><dt><code>!timestamp</code></dt><dd>Unix timestamp of the log entry.</dd><dt><code>!type</code></dt><dd>The category to which this message belongs.</dd><dt><code>!ip</code></dt><dd>IP address of the user triggering the message.</dd><dt><code>!request_uri</code></dt><dd>The requested URI.</dd><dt><code>!referer</code></dt><dd>HTTP Referer if available.</dd><dt><code>!uid</code></dt><dd>User ID.</dd><dt><code>!entity_id</code></dt><dd>The entity Id.</dd><dt><code>!entity_type</code></dt><dd>The entity type.</dd><dt><code>!entity_label</code></dt><dd>The entity label.</dd><dt><code>!bundle</code></dt><dd>The entity bundle.</dd><dt><code>!audit_action</code></dt><dd>The audit action.</dd></dl>'),
  );

  return system_settings_form($form);
}
