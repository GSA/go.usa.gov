<?php
/**
 * @file
 * User pages for the SAML Service Provider module.
 */

/**
 * Page callback to complete the SAML authentication process.
 * This is the consumer endpoint for all SAML authentication requests.
 */
function saml_sp__endpoint() {
  // Check that the request is a valid SAML response.
  if (!saml_sp__is_valid_authentication_response()) {
    // Not a valid incoming auth-message from an IDP, so abort.
    drupal_goto();
  }

  // The OneLogin_Saml_Response object uses the settings to verify the validity
  // of a request, in OneLogin_Saml_Response::isValid(), via XMLSecurityDSig.
  // Extract the incoming ID (the `inresponseto` parameter of the
  // `<samlp:response` XML node).

  if ($inbound_id = _saml_sp__extract_inbound_id($_POST['SAMLResponse'])) {
    if (variable_get('saml_sp__debug', FALSE)) {
      watchdog('saml_sp', 'Inbound ID: @inbound_id', array('@inbound_id' => $inbound_id), WATCHDOG_DEBUG);
    }
    if ($request = saml_sp__get_tracked_request($inbound_id)) {
      if (variable_get('saml_sp__debug', FALSE)) {
        watchdog('saml_sp', 'Request: <pre>@request</pre>', array('@request' => print_r($request, TRUE)), WATCHDOG_DEBUG);
      }
      $idp = saml_sp_idp_load($request['idp']);
      if (variable_get('saml_sp__debug', FALSE)) {
        watchdog('saml_sp', 'IdP cert(s): <pre>@certs</pre>', array('@certs' => print_r($idp->x509_certs, TRUE)), WATCHDOG_DEBUG);
      }
      $idp_certs = $idp->x509_certs;
      if (!empty($idp_certs)) {
        $settings = saml_sp__get_settings($idp);
        if (variable_get('saml_sp__debug', FALSE)) {
          watchdog('saml_sp', 'Attempting validation with: $settings => <pre>@settings</pre>', array('@settings' => print_r($settings, TRUE)), WATCHDOG_DEBUG);
        }
        // Creating Saml2 Settings object from array
        $saml_settings = new OneLogin_Saml2_Settings($settings);
        $saml_response = new OneLogin_Saml2_Response($saml_settings, $_POST['SAMLResponse']);
        if (empty($saml_response)) {
          watchdog('saml_sp', 'Response could not be parsed: <pre>%response</pre>', array('%response' => $_POST['SAMLResponse']), WATCHDOG_ERROR);
        }

        // Try to check the validity of the samlResponse.
        // isValid() will throw various exceptions to communicate errors.
        // Sadly, these are all caught and it returns only a boolean.
        $is_valid = $saml_response->isValid();
        if (variable_get('saml_sp__debug', FALSE) && $is_valid) {
          watchdog('saml_sp', 'Certificate worked!: %cert', array('%cert' => $cert), WATCHDOG_DEBUG);
        }
      }
      // Remove the now-expired tracked request.
      saml_sp_clear_request_store($inbound_id);


      if (!$is_valid) {
        try {
          if ($saml_response) {
            $error = $saml_response->getError();
            list($problem) = array_reverse(explode(' ', $error));

            switch ($problem) {
              case 'Responder':
                $message = t('There was a problem with the response from @idp_name. Please try again later.', array('@idp_name' => $idp->name));
                break;
              case 'Requester':
                $message = t('There was an issue with the request made to @idp_name. Please try again later.', array('@idp_name' => $idp->name));
                break;
              case 'VersionMismatch':
                $message = t('SAML VersionMismatch between @idp_name and @site_name. Please try again later.', array('@idp_name' => $idp->name, '@site_name' => variable_get('site_name', 'Drupal')));
                break;
            }
            if (!empty($message)) {
              drupal_set_message($message, 'error');
            }
            watchdog('saml_sp', 'Invalid response, @error: <pre>@response</pre>', array('@error' => $error, '@response' => print_r($saml_response, TRUE)), WATCHDOG_ERROR);
          }
        }
        catch (Exception $e) {
          drupal_set_message('An unknown error has occurred occurred, the Response was deemed invalid. Please contact a site administrator so they can look in the logs.');
          watchdog('saml_sp', 'An error occurred, the SAML response was present, but declared invalid with no apparent error. <pre>!saml_response</pre>', array('!saml_response' => print_r($saml_response, TRUE)), WATCHDOG_ERROR);
        }
      }

      // Invoke the callback function.
      $callback = $request['callback'];
      $result = $callback($is_valid, $saml_response);

      // The callback *should* redirect the user to a valid page.
      // Provide a fail-safe just in case it doesn't.
      if (empty($result)) {
        drupal_goto('user');
      }
      else {
        return $result;
      }
    }
  }
  // Failover: redirect to the homepage.
  watchdog('saml_sp', 'Failover: redirect to the homepage. No inbound ID or something.');
  drupal_goto();
}

/**
 * Check that a request is a valid SAML authentication response.
 *
 * @return Boolean
 */
function saml_sp__is_valid_authentication_response() {
  return ($_SERVER['REQUEST_METHOD'] == 'POST' && !empty($_POST['SAMLResponse']));
}

/**
 * Page callback to initiate the SAML SLO process.
 *
 */
function saml_sp__logout() {
  // Load the IDP to authenticate against.
  $idp = saml_sp_drupal_login__get_idp();

  // Settings is a OneLogin_Saml2_Settings object.
  $settings = saml_sp__get_settings($idp);

  $auth = new OneLogin_Saml2_Auth($settings);
  // Handle SAML Request / Response and process Single LogOut
  $auth->processSLO();

  $errors = $auth->getErrors();
  if (empty($errors)) {
    watchdog('saml_sp', 'Handled SLO Request/Response from SSO.');
  }
  else {
    watchdog('saml_sp', 'Errors in SLO process: %errors', array('%errors' => implode(', ', $errors)));
  }
  // Redirect user to path defined in RelayState after logout, or to front page
  // if it's empty
  drupal_set_message(t('You have successfully logged out from all SSO services.'));
  drupal_goto($_GET['RelayState']);
}
