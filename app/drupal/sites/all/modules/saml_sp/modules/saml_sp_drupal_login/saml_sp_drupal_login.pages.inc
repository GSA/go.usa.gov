<?php

/**
 * allows a user to request access to the site
 */
function saml_sp_drupal_login__request_access() {
  if (variable_get('saml_sp_drupal_login__request_account', 0) === 0) {
    // the setting is disabled
    drupal_not_found();
    return;
  }
  $form = array();

  $form['name'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Name'),
    '#description'    => t('The name a site administrator would know you by.'),
  );

  $form['email'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Email'),
    '#description'    => t('The email address you would like associated with your account'),
    '#default_value'  => isset($_GET['email']) && !empty($_GET['email']) && valid_email_address($_GET['email']) ? $_GET['email'] : '',
  );

  $form['explanation'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Explanation'),
    '#description'    => t('Why do you need an account for access to @site_name?', array('@site_name' => variable_get('site_name', t('Drupal')))),
  );

  $form['submit'] = array(
    '#type'           => 'submit',
    '#value'          => t('Submit'),
  );

  return $form;
}

function saml_sp_drupal_login__request_access_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('You have provided an invalid email address.'));
  }

  if (empty($form_state['values']['explanation'])) {
    form_set_error('explanation', t('You must provide an explanation of why you think you need access to the site.'));
  }

  if (empty($form_state['values']['name'])) {
    form_set_error('name', t('You must provide an name so site administrators jnow who you are.'));
  }
}

function saml_sp_drupal_login__request_access_submit($form, &$form_state) {
  $params = array(
    'mail'        => $form_state['values']['email'],
    'explanation' => $form_state['values']['explanation'],
    'name'        => $form_state['values']['name'],
  );

  $emails = array();
  if (variable_get('saml_sp_drupal_login__account_request_site_mail', FALSE)) {
    // send the email to the site_mail
    $site_mail = variable_get('site_mail', '');
    if (!empty($site_mail) && valid_email_address($site_mail)) {
      $emails[] = variable_get('site_mail', '');
    }
  }

  $admin_uids = array();
  foreach (variable_get('saml_sp_drupal_login__account_request_site_administrators') AS $uid) {
    if ($uid) {
      $admin_uids[] = $uid;
    }
  }

  $admin_role = variable_get('user_admin_role', 3);

  $query = db_select('users', 'u');
  $query->fields('u', array('uid', 'mail'));
  $query->join('users_roles', 'ur', 'ur.uid=u.uid');
  $query->condition('u.uid', $admin_uids, 'IN');
  $query->condition('ur.rid', $admin_role);
  $query->condition('u.status', 1);
  $result = $query->execute();

  foreach ($result->fetchAllAssoc('uid') AS $u) {
    if ($u->mail && valid_email_address($u->mail)) {
      $emails[] = $u->mail;
    }
  }

  // we now have a list of the email addresses to send the message to
  foreach ($emails AS $mail) {
    drupal_mail('saml_sp_drupal_login', 'account_request', $mail, language_default(), $params);
  }

  drupal_set_message(t('Your account creation request has been submitted to site administrators. They will inform you of the results.'));
  drupal_goto('user');
}
