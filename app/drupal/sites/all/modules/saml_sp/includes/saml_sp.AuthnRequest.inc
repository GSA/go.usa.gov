<?php

class saml_sp_AuthnRequest extends OneLogin_Saml2_AuthnRequest {

  /**
   * Constructs the AuthnRequest object.
   *
   * @param OneLogin_Saml2_Settings $settings Settings
   */
  public function __construct(OneLogin_Saml2_Settings $settings) {
    $this->_settings = $settings;

    $spData = $this->_settings->getSPData();
    $idpData = $this->_settings->getIdPData();
    $security = $this->_settings->getSecurityData();

    $id = OneLogin_Saml2_Utils::generateUniqueID();
    $issueInstant = OneLogin_Saml2_Utils::parseTime2SAML(time());

    $nameIDPolicyFormat = $spData['NameIDFormat'];
    if (isset($security['wantNameIdEncrypted']) && $security['wantNameIdEncrypted']) {
      $nameIDPolicyFormat = OneLogin_Saml2_Constants::NAMEID_ENCRYPTED;
    }

    $providerNameStr = '';
    $organizationData = $settings->getOrganization();
    if (!empty($organizationData)) {
      $langs = array_keys($organizationData);
      if (in_array('en-US', $langs)) {
        $lang = 'en-US';
      }
      else {
        $lang = $langs[0];
      }
      if (isset($organizationData[$lang]['displayname']) && !empty($organizationData[$lang]['displayname'])) {
        $providerNameStr = <<<PROVIDERNAME
ProviderName="{$organizationData[$lang]['displayname']}"
PROVIDERNAME;
      }
    }

    //check to see if we are using an old style setting (before multiple
    // contexts were supported)
    $contexts = saml_sp_get_authn_contexts();
    $authncontext = array();
    // get the list of supported contexts

    $context_settings = explode('|', $idpData['AuthnContextClassRef']);
    foreach ($contexts AS $key => $value) {
      if (array_search($value['id'], $context_settings) !== FALSE) {
        $authncontext[] = "      <saml:AuthnContextClassRef>{$key}</saml:AuthnContextClassRef>";
      }
    }
    $authncontext = "\n" . implode("\n", $authncontext);

    $request = <<<AUTHNREQUEST
<samlp:AuthnRequest
  xmlns:samlp="urn:oasis:names:tc:SAML:2.0:protocol"
  xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion"
  ID="$id"
  Version="2.0"
  {$providerNameStr}
  IssueInstant="$issueInstant"
  Destination="{$idpData['singleSignOnService']['url']}"
  ProtocolBinding="urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST"
  AssertionConsumerServiceURL="{$spData['assertionConsumerService']['url']}">
  <saml:Issuer>{$spData['entityId']}</saml:Issuer>
    <samlp:NameIDPolicy
        Format="{$nameIDPolicyFormat}"
        AllowCreate="true" />
    <samlp:RequestedAuthnContext Comparison="exact">{$authncontext}
    </samlp:RequestedAuthnContext>
</samlp:AuthnRequest>
AUTHNREQUEST;

    if (variable_get('saml_sp__debug', FALSE)) {
      watchdog('saml_sp', 'samlp:AuthnRequest =><br/><pre>@request</pre>', array('@request' => $request), WATCHDOG_DEBUG);
    }
    $this->_id = $id;
    $this->_authnRequest = $request;
    if (variable_get('saml_sp__debug', FALSE) && function_exists('dpm')) {
      dpm($request, 'samlp:AuthnRequest');
    }
  }


  /**
   * Returns deflated, base64 encoded, unsigned AuthnRequest.
   *
   */
  public function getRequest($deflate = NULL) {
    $deflatedRequest = gzdeflate($this->_authnRequest);
    $base64Request = base64_encode($deflatedRequest);
    return $base64Request;
  }

    /**
   * Returns the AuthNRequest ID.
   *
   * @return string
   */
  public function getId() {
      return $this->_id;
  }
}
