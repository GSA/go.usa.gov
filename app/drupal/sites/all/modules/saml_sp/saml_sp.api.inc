<?php
/**
 * @file
 * API hooks for SAML Service Provider module.
 */


/**
 * Alter the settings used when generating SAML requests.
 *
 * @param OneLogin_Saml_Settings $settings
 */
function hook_saml_sp_settings_alter(&$settings) {
  // Change the consuming URL to a custom endpoint.
  if ($settings->login_url == 'http://example.com/saml/foo') {
    $settings->spReturnUrl = url('saml/custom_action', array('absolute' => TRUE));
  }
}

/**
 * Provide default IDPs, exported to code.
 *
 * @return Array
 * Array of IDP objects, keyed by machine name.
 */
function hook_saml_sp_default_idps() {
  $saml_idp = new stdClass();
  $saml_idp->disabled = FALSE; /* Edit this to true to make a default saml_idp disabled initially */
  $saml_idp->api_version = 1;
  $saml_idp->machine_name = 'foo';
  $saml_idp->name = 'Foo IDP';
  $saml_idp->app_name = '';
  $saml_idp->nameid_field = 'mail';
  $saml_idp->login_url = 'http://example.com/saml/login';
  $saml_idp->logout_url = 'http://example.com/saml/logout';
  $saml_idp->x509_cert = '-----BEGIN CERTIFICATE-----
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx==
-----END CERTIFICATE-----';

  $saml_sp_idps[$saml_idp->machine_name] = $saml_idp;

  return $saml_sp_idps;
}
