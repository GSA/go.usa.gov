<?php

class saml_sp_AuthnRequest extends OneLogin_Saml2_AuthnRequest {

  /**
   * Constructs the AuthnRequest object.
   *
   * @param OneLogin_Saml2_Settings $settings Settings
   */
  public function __construct(OneLogin_Saml2_Settings $settings) {
    parent::__construct($settings);

    if (variable_get('saml_sp__debug', FALSE)) {
      watchdog('saml_sp', 'samlp:AuthnRequest =><br/><pre>@request</pre>', array('@request' => $this->getXML()), WATCHDOG_DEBUG);
    }
    if (variable_get('saml_sp__debug', FALSE) && function_exists('dpm')) {
      dpm($this->getXML(), 'samlp:AuthnRequest');
    }
  }
}
