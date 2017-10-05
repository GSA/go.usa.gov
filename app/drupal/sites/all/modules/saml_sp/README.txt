SAML Service Provider
=====================

This package provides two modules:
- SAML Service Provider API
- SAML Drupal Login


The API module lets other modules leverage SAML authentication.

The SAML Drupal Login module specifically enables Drupal to become a "Service
Provider" for an IDP, so users can authenticate to Drupal (without entering a
username or password) by delegating authenticate to a SAML IDP (Identity
Provider).


Dependencies
============
Drupal module ctools http://drupal.org/project/ctools

Requires the OneLogin SAML-PHP toolkit, downloaded to your 'libraries' folder:

'cd libraries'
'git clone https://github.com/onelogin/php-saml.git .'

So that the folder structure looks like this when it comes to lib folder
- libraries
  - php-saml
    ...
    - lib
      - Saml
      - Saml2
        - Auth.php
        ...


SimpleSamlPHP Configuration
===========================

First, configure your IdP in Drupal:
Note: Multiple IdPs can be configured, but only one is chosen to be used for the
Drupal login. This is good for development purposes, because different
environments (local, development, staging, production etc.) can be configured
with different App names and exported to code with Features. Then each
environment chooses a different IdP configuration for the Drupal login.

Name = Human readable name for IdP.

App Name: will be used in the IdP configuration. For example
"demoLocalDrupal".

NameID field: this defaults to user mail and works for most configurations. In
that case the IdP is configure to use email address for NameID.
But if you need to support changing email on the IdP, then you need to add
a custom field to user profile and then choose that field here. It is
recommended to use "Hidden Field Widgets" module (https://www.drupal.org/project/hidden_field)
for that field so that users don't need to worry about it, ever.

IDP Login URL: e.g. http:///myIdp.example.com/simplesaml/saml2/idp/SSOService.php
IDP Logout URL: e.g. http:///myIdp.example.com/simplesaml/saml2/idp/SingleLogoutService.php

x.509 certificate: Should correspond to the "certificate" field in
saml20-idp-hostd.php

Here's a sample config for saml20-sp-remote.php (when email is used for NameID):

$metadata['demoLocalDrupal'] = array(
'AssertionConsumerService' => 'http://mydrupal.example.com/drupal7/?q=saml/consume',
'NameIDFormat' => 'urn:oasis:names:tc:SAML:2.0:nameid-format:email',
'simplesaml.nameidattribute' => 'uid',
'simplesaml.attributes' => FALSE,
);

Usage
=====

When everything is set and ready to go, the process begins from
http://www.yoursite.com/saml/drupal_login

A returnTo parameter can be appended to the url, if you want to redirect
the user somewhere else than the front page after login. For example the user
profile page http://www.yoursite.com/saml/drupal_login?returnTo=user

The login block and user login form will show a link with
"Log in using Single Sign-On" text on it. The user login page will return the
user to the profile page and the login block will return the user to the same page
where the login process was started from.

