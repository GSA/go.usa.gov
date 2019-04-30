<?php
/**
 * @file
 * Admin pages for the SAML Service Provider module.
 */


/**
 * Overview page.
 * Display a list of IDPs in a table.
 */
function saml_sp__admin_overview() {
  return array(
    'idps' => saml_sp__load_all_idps(),
    '#theme' => 'saml_sp__idp_list',
  );
}



/**
 * Configure or add a SAML IDP.
 *
 * @ingroup forms
 */
function saml_sp__configure_idp_form($form, &$form_state, $saml_idp = NULL) {
  $library = _saml_sp__prepare();
  $show_metadata = TRUE;
  if (is_null($saml_idp)) {
    $show_metadata = FALSE;
    // Populate a default IDP object, with empty fields.
    $saml_idp = _saml_sp__default_idp();
  }
  $form['#destination'] = 'admin/config/people/saml_sp/IDP';

  $form['export_type'] = array(
   '#type' => 'value',
   '#value' => isset($saml_idp->export_type) ? $saml_idp->export_type : NULL,
  );

   // If this is an update to an existing IDP, track the original machine name,
   // in case it is changed.
   if (!empty($saml_idp->machine_name)) {
     $form['orig_machine_name'] = array(
       '#type' => 'value',
       '#value' => $saml_idp->machine_name,
     );
   }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $saml_idp->name,
    '#description' => t('The human-readable name of this IDP. This text will be displayed to administrators who can configure SAML.'),
    '#required' => TRUE,
    '#size' => 30,
    '#maxlength' => 30,
  );

  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $saml_idp->machine_name,
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'saml_sp_idp_load',
      // 'source' => array('machine_name'),
    ),
    '#description' => t('A unique machine-readable name for this IDP. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['entity_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Entity ID'),
    '#description' => t('The entityID identifier which the Identity Provider will use to identiy itself by, this may sometimes be a URL.'),
    '#default_value' => $saml_idp->entity_id,
    '#maxlength' => 255,
  );

  $form['app_name'] = array(
    '#type' => 'textfield',
    '#title' => t('App name'),
    '#description' => t('The app name is provided to the Identiy Provider, to identify the origin of the request.'),
    '#default_value' => $saml_idp->app_name,
    '#maxlength' => 255,
  );

  // Adding mail and extra fields to select list
  $fields = array('mail' => t('Email'));
  $extra_fields = field_info_instances($entity_type = 'user', $bundle_name = NULL);
  $extra_fields = array_keys($extra_fields['user']);
  foreach ($extra_fields as $value) {
    $fields[$value] = $value;
  }
  $form['nameid_field'] = array(
    '#type' => 'select',
    '#title' => t('NameID field'),
    '#description' => t('Mail is usually used between IdP and SP, but if you want to let users change the email address in IdP, you need to use a custom field to store the ID.'),
    '#options' => $fields,
    '#default_value' => $saml_idp->nameid_field,
  );

  // The SAML Login URL and x.509 certificate must match the details provided
  // by the IDP.
  $form['idp'] = array(
    '#type' => 'fieldset',
    '#title' => t('IDP configuration'),
    '#description' => t('Enter the details provided by the IDP.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['idp']['idp_login_url'] = array(
    '#type' => 'textfield',
    '#title' => t('IDP Login URL'),
    '#description' => t('Login URL of the Identity Provider server.'),
    '#default_value' => $saml_idp->login_url,
    '#required' => TRUE,
    '#max_length' => 255,
  );

  $form['idp']['idp_logout_url'] = array(
    '#type' => 'textfield',
    '#title' => t('IDP Logout URL'),
    '#description' => t('Logout URL of the Identity Provider server.'),
    '#default_value' => $saml_idp->logout_url,
    '#required' => TRUE,
    '#max_length' => 255,
  );
  $form['idp']['idp_x509_certs'] = array(
    '#type' => 'fieldset',
    '#title' => t('x.509 Certificates'),
    '#tree' => TRUE,
    '#description' => t('Enter the application certificate(s) provided by the IdP. When an IdP is switching to a new certificate they will occasionally provide the certificate before hand to those with a Relying Party Trust (RTP). The Certificates listed will be tried in the order they are entered.'),
  );
  $certs = $saml_idp->x509_certs ?: (
    isset($saml_idp->x509_cert) ?
      explode("\n", $saml_idp->x509_cert) :
      array()
  );
  $count = 0;
  if (!empty($certs)) {
    foreach ($certs AS $encoded_cert) {
      $encoded_cert = trim($encoded_cert);
      $form['idp']['idp_x509_certs'][$count] = array(
        '#type' => 'fieldset',
        '#title' => t('x.509 Certificate @count', array('@count' => $count)),
      );
      if (function_exists('openssl_x509_parse')) {
        $cert = openssl_x509_parse(OneLogin_Saml2_Utils::formatCert($encoded_cert));
        // flatten the issuer array
        if (!empty($cert['issuer'])) {
          foreach ($cert['issuer'] AS $key => &$value) {
            if (is_array($value)) {
              $value = implode("/", $value);
            }
          }
        }

        if ($cert) {
          $title = t('Name: %cert-name<br/>Issued by: %issuer<br/>Valid: %valid-from - %valid-to', array(
            '%cert-name' => isset($cert['name']) ? $cert['name'] : '',
            '%issuer' => isset($cert['issuer']) && is_array($cert['issuer']) ? implode('/', $cert['issuer']) : '',
            '%valid-from' => isset($cert['validFrom_time_t']) ? date('c', $cert['validFrom_time_t']) : '',
            '%valid-to' => isset($cert['validTo_time_t']) ? date('c', $cert['validTo_time_t']) : '',
          ));
        }
      }
      if (!isset($title) || empty($title)) {
        $title = t('Certificate');
      }
      $form['idp']['idp_x509_certs'][$count]['cert'] = array(
        '#type' => 'textarea',
        '#title' => $title,
        '#description' => t('Enter an application certificate provided by the IdP.'),
        '#default_value' => $encoded_cert,
        '#max_length' => 1024,
      );

      $form['idp']['idp_x509_certs'][$count]['details'] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#title' => t('Details', array('@count' => $count)),
      );
      $form['idp']['idp_x509_certs'][$count]['details']['markup'] = array(
        '#markup' => '<pre>' . print_r($cert, TRUE) . '</pre>'
      );

      $count++;
    }
  }

  $form['idp']['idp_x509_certs'][$count]['cert'] = array(
    '#type' => 'textarea',
    '#title' => t('x.509 Certificate @count', array('@count' => $count)),
    '#description' => t('Enter an application certificate provided by the IdP'),
    '#default_value' => '',
    '#max_length' => 1024,
  );

  // get the list of supported contexts
  $contexts = saml_sp_get_authn_contexts();
  $authn_context_class_ref_options = array();
  // check to see if this is the old style setting () before multiple contexts
  // were supported
  if (strpos($saml_idp->authn_context_class_ref, '|') === FALSE && strpos($saml_idp->authn_context_class_ref, ':')) {
    $selected_contexts = array($contexts[$saml_idp->authn_context_class_ref]['id']);
  }
  else {
    $selected_contexts = explode('|', $saml_idp->authn_context_class_ref);
  }

  $authn_context_default = array();
  // create options array as well as default array
  foreach ($contexts AS $key => $value) {
    $authn_context_class_ref_options[$value['id']] = $value['label'];
    $authn_context_default[$value['id']] = array_search($value['id'], $selected_contexts) !== FALSE ? $value['id'] : 0;
  }

  $form['idp']['authn_context_class_ref'] = array(
    '#type'           => 'checkboxes',
    '#title'          => t('Authentication Method (Context)'),
    '#description'    => t('What authentication method(s) would you like to use with this IdP?'),
    '#default_value'  => $authn_context_default,
    '#options'        => $authn_context_class_ref_options,
    '#required'       => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}



/**
 * Submit handler for the SAML IDP configuration form.
 */
function saml_sp__configure_idp_form_submit($form, &$form_state) {
  // handle multiple certs
  $certs = array();
  foreach ($form_state['values']['idp_x509_certs'] AS $cert) {
    if (!empty($cert['cert'])) {
      $certs[] = $cert['cert'];
    }
  }
  // handle multiple Authn Contexts
  $contexts = array();
  foreach ($form_state['values']['authn_context_class_ref'] AS $value) {
    if ($value) {
      $contexts[] = $value;
    }
  }

  // Redirect to the admin overview page.
  if (!empty($form['#destination'])) {
    $form_state['redirect'] = $form['#destination'];
  }

  $idp = (object) array(
    'name'                    => $form_state['values']['name'],
    'machine_name'            => $form_state['values']['machine_name'],
    'app_name'                => $form_state['values']['app_name'],
    'entity_id'               => $form_state['values']['entity_id'],
    'nameid_field'            => $form_state['values']['nameid_field'],
    'login_url'               => $form_state['values']['idp_login_url'],
    'logout_url'              => $form_state['values']['idp_logout_url'],
    'x509_certs'              => $certs,
    'authn_context_class_ref' => implode('|', $contexts),
    'export_type'             => $form_state['values']['export_type'],
  );

  // Machine names can be changed.
  if (isset($form_state['values']['orig_machine_name']) && $form_state['values']['orig_machine_name'] != $form_state['values']['machine_name']) {
    $idp->orig_machine_name = $form_state['values']['orig_machine_name'];
  }

  $result = saml_sp_idp_save($idp);
  switch ($result) {
    case SAVED_NEW:
      drupal_set_message(t('IdP %idp_name has been created.', array('%idp_name' => $form_state['values']['name'])));
      break;

    case SAVED_UPDATED:
      drupal_set_message(t('IdP %idp_name has been updated.', array('%idp_name' => $form_state['values']['name'])));
      break;

    default:
      drupal_set_message(t('An error occurred, IdP %idp_name has not been saved.', array('%idp_name' => $form_state['values']['name'])), 'error');
      break;
  }
}

/**
 * Confirmation form to delete an IDP.
 */
function saml_sp__delete_idp_form($form, &$form_state, $saml_idp) {
  $form['#destination'] = 'admin/config/people/saml_sp/IDP';

  // Pass the name to the submit handler, to use in the message.
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $saml_idp->name,
  );

  // Pass the machine name to the handler, to use as the key for invoking
  // the delete API.
  $form['machine_name'] = array(
    '#type' => 'value',
    '#value' => $saml_idp->machine_name,
  );

  // Usage: confirm_form($form, $question, $path, $description = NULL, $yes = NULL, $no = NULL, $name = 'confirm')
  $question = ($saml_idp->export_type & EXPORT_IN_CODE)
    ? t('Are you sure you wish revert the IDP %idp_name?', array('%idp_name' => $saml_idp->name))
    : t('Are you sure you wish delete the IDP %idp_name?', array('%idp_name' => $saml_idp->name));

  return confirm_form(
    $form,
    $question,
    'admin/config/people/saml_sp/IDP',
    t('This action cannot be undone.'),
    ($saml_idp->export_type & EXPORT_IN_CODE) ? t('Revert') : t('Delete'));
}

/**
 * Submit handler.
 */
function saml_sp__delete_idp_form_submit($form, &$form_state) {
  // Redirect to the admin overview page.
  if (!empty($form['#destination'])) {
    $form_state['redirect'] = $form['#destination'];
  }

  $result = saml_sp_idp_delete($form_state['values']['machine_name']);
  drupal_set_message(t('IDP %idp_name has been deleted.', array('%idp_name' => $form_state['values']['name'])));
}


/**
 * Export handler.
 */
function saml_sp__export_idp($saml_idp) {
  $output = array();

  $code = ctools_export_crud_export('saml_sp_idps', $saml_idp);
  $code .= "\n";
  $code .= '$saml_sp_idps[$saml_idp->machine_name] = $saml_idp;';
  $code .= "\n";
  $file = 'modulename.saml_sp_idps.inc';
  $export_form = drupal_get_form('ctools_export_form', $code, t('Add this to hook_saml_sp_default_idps().'));
  $output['saml_sp_idps__inc'] = array(
    '#markup' => drupal_render($export_form),
  );

  return $output;
}

/**
 * configure this SAML Service Provider
 */
function saml_sp__admin_config($form, &$form_state) {
  $library = _saml_sp__prepare();
  $contact = variable_get('saml_sp__contact', FALSE);

  $form['saml_sp__contact'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Contact Information'),
    '#description'  => t('Information to be included in the federation metadata.'),
    '#tree'         => TRUE,
  );
  $form['saml_sp__contact']['technical'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Technical'),
  );
  $form['saml_sp__contact']['technical']['name'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Name'),
    '#default_value'  => $contact ? $contact['technical']['name'] : ''
  );
  $form['saml_sp__contact']['technical']['email'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Email'),
    '#default_value'  => $contact ? $contact['technical']['email'] : '',
  );
  $form['saml_sp__contact']['support'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Support'),
  );
  $form['saml_sp__contact']['support']['name'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Name'),
    '#default_value'  => $contact ? $contact['support']['name'] : '',
  );
  $form['saml_sp__contact']['support']['email'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Email'),
    '#default_value'  => $contact ? $contact['support']['email'] : '',
  );

  $organization = variable_get('saml_sp__organization', FALSE);
  $form['saml_sp__organization'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Organization'),
    '#description'    => t('Organization information for the federation metadata'),
    '#tree'           => TRUE,
  );
  $form['saml_sp__organization']['name'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Name'),
    '#description'    => t('This is a short name for the organization'),
    '#default_value'  => $organization ? $organization['name'] : '',
  );
  $form['saml_sp__organization']['display_name'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Display Name'),
    '#description'    => t('This is a long name for the organization'),
    '#default_value'  => $organization ? $organization['display_name'] : '',
  );
  $form['saml_sp__organization']['url'] = array(
    '#type'           => 'textfield',
    '#title'          => t('URL'),
    '#description'    => t('This is a URL for the organization'),
    '#default_value'  => $organization ? $organization['url'] : '',
  );

  $form['saml_sp__strict'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Strict Protocol'),
    '#description'    => t('SAML 2 Strict protocol will be used.'),
    '#default_value'  => variable_get('saml_sp__strict', FALSE),
  );

  $security = variable_get('saml_sp__security', FALSE);
  $form['saml_sp__security'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Security'),
    '#tree'           => TRUE,
  );
  $form['saml_sp__security']['offered'] = array(
    //'#type'           => 'markup',
    '#markup'          => t('Signatures and Encryptions Offered:'),
  );
  $form['saml_sp__security']['nameIdEncrypted'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('NameID Encrypted'),
    '#default_value'  => $security ? $security['nameIdEncrypted'] : '',
  );
  $form['saml_sp__security']['authnRequestsSigned'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Authn Requests Signed'),
    '#default_value'  => $security ? $security['authnRequestsSigned'] : '',
  );
  $form['saml_sp__security']['logoutRequestSigned'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Logout Requests Signed'),
    '#default_value'  => $security ? $security['logoutRequestSigned'] : '',
  );
  $form['saml_sp__security']['logoutResponseSigned'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Logout Response Signed'),
    '#default_value'  => $security ? $security['logoutResponseSigned'] : '',
  );

  $form['saml_sp__security']['required'] = array(
    //'#type'           => 'markup',
    '#markup'          => t('Signatures and Encryptions Required:'),
  );
  $form['saml_sp__security']['wantMessagesSigned'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Want Messages Signed'),
    '#default_value'  => $security ? $security['wantMessagesSigned'] : '',
  );
  $form['saml_sp__security']['wantAssertionsSigned'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Want Assertions Signed'),
    '#default_value'  => $security ? $security['wantAssertionsSigned'] : '',
  );
  $form['saml_sp__security']['wantNameIdEncrypted'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Want NameID Encrypted'),
    '#default_value'  => $security ? $security['wantNameIdEncrypted'] : '',
  );
  $form['saml_sp__security']['metadata'] = array(
    //'#type'           => 'markup',
    '#markup'          => t('Metadata:'),
  );


  $form['saml_sp__security']['signMetaData'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Sign Meta Data'),
    '#default_value'  => $security ? $security['signMetaData'] : '',
  );

  $form['saml_sp__cert_location'] = array(
    '#type'   => 'textfield',
    '#title'  => t('Certificate Location'),
    '#description'  => t('The location of the x.509 certificate file on the server. This must be a location that PHP can read.'),
    '#default_value' => variable_get('saml_sp__cert_location', ''),
    '#suffix' => saml_sp_get_cert_info(variable_get('saml_sp__cert_location', '')),
  );

  $form['saml_sp__key_location'] = array(
    '#type'   => 'textfield',
    '#title'  => t('Key Location'),
    '#description'  => t('The location of the x.509 key file on the server. This must be a location that PHP can read.'),
    '#default_value' => variable_get('saml_sp__key_location', ''),
  );

  $form['saml_sp__new_cert_location'] = array(
    '#type'   => 'textfield',
    '#title'  => t('New Cert Location'),
    '#description'  => t('The location of the x.509 certificate file on the server. If the certificate above is about to expire add your new certificate here after you have obtained it. This will add the new certificate to the metadata to let the IdP know of the new certificate. This must be a location that PHP can read.'),
    '#default_value' => variable_get('saml_sp__new_cert_location', ''),
    '#suffix' => saml_sp_get_cert_info(variable_get('saml_sp__new_cert_location', '')),
  );
  $error = FALSE;
  try {
    $metadata = saml_sp__get_metadata(NULL, FALSE);
    if (is_array($metadata)) {
      if (isset($metadata[1])) {
        $errors = $metadata[1];
      }
      $metadata = $metadata[0];
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('Attempt to create metadata failed: %message.', array('%message' => $e->getMessage())), 'error');
    $metadata = '';
    $error = $e;
  }
  if (empty($metadata) && $error) {
    $no_metadata = t('There is currently no metadata because of the following error: %error. Please resolve the error and  return here for your metadata.', array('!url' => url('admin/config/people/saml_sp/drupal_login'), '%error' => $error->getMessage()));
  }
  $form['metadata'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => TRUE,
    '#collapsible'  => TRUE,
    '#title'        => t('Metadata'),
    '#description'  => t('This is the Federation Metadata for this IdP'),
  );

  if ($metadata) {
    $form['metadata']['data'] = array(
      '#type'           => 'textarea',
      '#title'          => t('XML'),
      '#description'    => t(
        'This metadata for @idp can also be accessed !link',
        array(
          '@idp' => variable_get('saml_sp_drupal_login__idp', ''),
          '!link' => l(t('here'), saml_sp__metadata_url()),
        )),
      '#disabled'       => TRUE,
      '#rows'           => 20,
      '#default_value'  => $metadata,
    );
  }
  else {
    $form['metadata']['none'] = array(
      '#markup'         => $no_metadata,
    );
  }
  $form['saml_sp__debug'] = array(
    '#type'             => 'checkbox',
    '#title'            => t('Debug'),
    '#description'      => t('Works with Devel module to display SAML requests and Responses for review.'),
    '#default_value'    => variable_get('saml_sp__debug', FALSE),
  );



  return system_settings_form($form);
}

/**
 * with the certificate location retrieve pertinant certificate data and output
 * in a string for display
 */
function saml_sp_get_cert_info($cert_location) {
  if (!empty($cert_location) && file_exists($cert_location) && function_exists('openssl_x509_parse')) {
    $encoded_cert = trim(file_get_contents($cert_location));
    $cert = openssl_x509_parse(OneLogin_Saml2_Utils::formatCert($encoded_cert));
    // flatten the issuer array
    if (!empty($cert['issuer'])) {
      foreach ($cert['issuer'] AS $key => &$value) {
        if (is_array($value)) {
          $value = implode("/", $value);
        }
      }
    }

    if ($cert) {
      $info = t('Name: %cert-name<br/>Issued by: %issuer<br/>Valid: %valid-from - %valid-to', array(
        '%cert-name' => isset($cert['name']) ? $cert['name'] : '',
        '%issuer' => isset($cert['issuer']) && is_array($cert['issuer']) ? implode('/', $cert['issuer']) : '',
        '%valid-from' => isset($cert['validFrom_time_t']) ? date('c', $cert['validFrom_time_t']) : '',
        '%valid-to' => isset($cert['validTo_time_t']) ? date('c', $cert['validTo_time_t']) : '',
      ));
      return $info;
    }
  }

  return FALSE;
}
function saml_sp__admin_config_validate(&$form, &$form_state) {
  // check the saml_sp__sign_metadata status
  if (isset($form_state['values']['saml_sp__sign_metadata']) && !empty($form_state['values']['saml_sp__sign_metadata'])) {
    // certificate file
    if (empty($form_state['values']['saml_sp__cert_location'])) {
      form_set_error('saml_sp__cert_location', 'The Certificate Location must be provided.');
    }
    if (!is_file($form_state['values']['saml_sp__cert_location'])) {
      form_set_error('saml_sp__cert_location', 'The Certificate file does not exist.');
    }
    // key file
    if (empty($form_state['values']['saml_sp__key_location'])) {
      form_set_error('saml_sp__cert_location', 'The Certificate Location must be provided.');
    }
    if (!is_file($form_state['values']['saml_sp__key_location'])) {
      form_set_error('saml_sp__cert_location', 'The Certificate file does not exist.');
    }
  }

}
