<?php

class saml_sp_Auth extends OneLogin_Saml2_Auth {
  public $auth_callback;

  /**
   * Set the auth callback for after the response is returned
   */
  public function setAuthCallback($callback) {
    $this->auth_callback = $callback;
  }

  /**
    * Initiates the SSO process.
    *
    * @param string $returnTo   The target URL the user should be returned to after login.
    * @param array  $parameters Extra parameters to be added to the GET
    */
   public function login($returnTo = null, $parameters = array(), $forceAuthn = false, $isPassive = false, $stay=false, $setNameIdPolicy = true) {
    assert('is_array($parameters)');

    $settings = $this->getSettings();

    $authnRequest = new saml_sp_AuthnRequest($settings);

    $samlRequest = $authnRequest->getRequest();
    $parameters['SAMLRequest'] = $samlRequest;

    if (!empty($returnTo)) {
      $parameters['RelayState'] = $returnTo;
    }
    else {
      $parameters['RelayState'] = OneLogin_Saml2_Utils::getSelfRoutedURLNoQuery();
    }

    $security = $settings->getSecurityData();
    if (isset($security['authnRequestsSigned']) && $security['authnRequestsSigned']) {
      $signature = $this->buildRequestSignature($samlRequest, $parameters['RelayState']);
      $parameters['SigAlg'] = XMLSecurityKey::RSA_SHA1;
      $parameters['Signature'] = $signature;
    }

    // get this necessary information for this IdP
    $idp = (object) $settings->getIdPData();
    $all_idps = saml_sp__load_all_idps();
    foreach ($all_idps AS $this_idp) {
      if ($this_idp->entity_id == $idp->entityId) {
        $idp->machine_name = $this_idp->machine_name;
      }
    }


    // record the outbound Id of the request
    $id = $authnRequest->getId();
    saml_sp__track_request($id, $idp, $this->auth_callback);
    if (variable_get('saml_sp__debug', FALSE) && module_exists('devel')) {
      return t('This is a debug page, <a href="!url">you can proceed by clicking this link.</a>', array('!url' => url($this->getSSOurl(), array('query' => $parameters))));
    }
    if (variable_get('saml_sp__debug', FALSE)) {
      watchdog('saml_sp', 'Using $parameters => <pre>@parameters</pre>', array('@parameters' => print_r($parameters, TRUE)), WATCHDOG_DEBUG);
    }

    $this->redirectTo($this->getSSOurl(), $parameters);
  }
}
