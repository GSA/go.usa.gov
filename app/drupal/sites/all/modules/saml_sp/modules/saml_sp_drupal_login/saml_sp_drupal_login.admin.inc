<?php

/**
 * @file
 * Admin pages for the SAML Drupal Login module
 */

/**
 * Configure which IDP to use when authenticating with Drupal.
 */
function saml_sp_drupal_login__admin_config_form($form, &$form_state) {
  $idps = array();
  // List all the IDPs in the system.
  foreach (saml_sp__load_all_idps() as $machine_name => $idp) {
    $idps[$machine_name] = $idp->name;
  }

  $form['saml_sp_drupal_login__idp'] = array(
    '#type' => 'select',
    '#options' => $idps,
    '#title' => t('IdP'),
    '#description' => t('Choose the IdP to use when authenticating Drupal logins'),
    '#default_value' => variable_get('saml_sp_drupal_login__idp', ''),
  );

  $form['saml_sp_drupal_login__logout'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Single Log Out'),
    '#description'    => t('When logging out of %site_name also log out of the IdP', array('%site_name' => variable_get('site_name', 'Drupal'))),
    '#default_value'  => variable_get('saml_sp_drupal_login__logout', TRUE),
  );

  $form['saml_sp_drupal_login__update_email'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Update Email address'),
    '#description'    => t('If an account can be found on %site_name but the e-mail address differs from the IdP update the email on record with the new address from the IdP.', array('%site_name' => variable_get('site_name', 'Drupal'))),
    '#default_value'  => variable_get('saml_sp_drupal_login__update_email', TRUE),
  );

  $form['saml_sp_drupal_login__update_language'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Update Language'),
    '#description'    => t('If the account language of %site_name differs from that of the IdP update to match.', array('%site_name' => variable_get('site_name', 'Drupal'))),
    '#default_value'  => variable_get('saml_sp_drupal_login__update_language', TRUE),
  );

  $form['saml_sp_drupal_login__no_account_authenticated_user_role'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Login users without a user account as an authenticated user.'),
    '#description'    => t('If a user is authenticated by the SAML Service Provider but no matching account can be found the user will be logged in as an authenticated user. This will allow users to be authenticated to receive more permissions than an anonymous user but less than a user with any other role.'),
    '#default_value'  => variable_get('saml_sp_drupal_login__no_account_authenticated_user_role', FALSE),
  );

  $form['saml_sp_drupal_login__no_account_authenticated_user_account'] = array(
    '#type'               => 'textfield',
    '#title'              => t('Authenticated user account'),
    '#description'        => t('This is the account with only the authenticated user role which a user is logged in as if no matching account exists. As this account will be used for all users make sure that this account has only the "Authenticated User" role.'),
    '#default_value'      => variable_get('saml_sp_drupal_login__no_account_authenticated_user_account', ''),
    '#autocomplete_path'  => 'user/autocomplete',
    '#states'             => array(
      'visible'             => array(
        ':input[name="saml_sp_drupal_login__no_account_authenticated_user_role"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['saml_sp_drupal_login__force_saml_only'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Force SAML Login'),
    '#description'    => t('The User Login form will not be used, when an anonymous user goes to /user they will be automatically redirected to the SAML authentication page.'),
    '#default_value'  => variable_get('saml_sp_drupal_login__force_saml_only', FALSE),
  );

  $form['account_request'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Accounts'),
    '#description'    => t('Allow Users to request account creation.'),
    '#access'         => (variable_get('user_register', USER_REGISTER_ADMINISTRATORS_ONLY) == USER_REGISTER_ADMINISTRATORS_ONLY ? 1 : 0),
  );

  $form['account_request']['saml_sp_drupal_login__request_account'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Allow users without an account to request an account?'),
    '#description'    => t('Since you only allow Administrators to create an account: <a href="!account_settings_url" target="_blank">see Account Settings page</a> should the user who authenticates against the SAML IdP be able to request an account from administrators?', array('!account_settings_url' => url('admin/config/people/accounts'))),
    '#default_value'  => variable_get('saml_sp_drupal_login__request_account', 0),
  );

  $form['account_request']['saml_sp_drupal_login__account_request_site_mail'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Send request to site mail account ( @site_mail )', array('@site_mail' => variable_get('site_mail', ''))),
    '#default_value'  => variable_get('saml_sp_drupal_login__account_request_site_mail', FALSE),
    '#states'         => array(
      'visible'         => array(
        ':input[name="saml_sp_drupal_login__request_account"]' => array('checked' => TRUE),
      ),
    ),
  );

  $admin_role = variable_get('user_admin_role', 3);

  $query = db_select('users', 'u');
  $query->fields('u', array('uid', 'name', 'mail'));
  $query->join('users_roles', 'ur', 'ur.uid=u.uid');
  $query->condition('ur.rid', $admin_role); // the user has the admin role
  $query->condition('u.status', 1); // the user is active (not blocked)

  $result = $query->execute();
  $admin_options = array();
  foreach ($result->fetchAllAssoc('uid') AS $u) {
    $admin_options[$u->uid] = $u->name . ' - ' . $u->mail;
  }

  $form['account_request']['saml_sp_drupal_login__account_request_site_administrators'] = array(
    '#type'           => 'checkboxes',
    '#title'          => t('Send request to Site Administrators'),
    '#description'    => t('The request email will be sent to these site Administrators'),
    '#options'        => $admin_options,
    '#default_value'  => variable_get('saml_sp_drupal_login__account_request_site_administrators', FALSE),
    '#states'         => array(
      'visible'         => array(
        ':input[name="saml_sp_drupal_login__request_account"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}
