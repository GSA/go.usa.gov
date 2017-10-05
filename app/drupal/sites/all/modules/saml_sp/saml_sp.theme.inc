<?php
/**
 * @file
 * Theme implementations for the SAML Service Provider module.
 */

/**
 * List all registered SAML IDPs for the admin overview page.
 *
 * @ingroup themeable
 */
function theme_saml_sp__idp_list($variables) {
  $idp_list = $variables['idps']['idps'];

  $header = array(
    t('Name'),
    t('Machine Name'),
    t('IDP Login URL'),
    t('Status'),
    t('Operations'),
  );

  $rows = array();
  foreach ($idp_list as $idp) {
    if (($idp->export_type & EXPORT_IN_DATABASE) && ($idp->export_type & EXPORT_IN_CODE)) {
      $status = t('Database overriding code');
    }
    elseif ($idp->export_type & EXPORT_IN_DATABASE) {
      $status = t('Database');
    }
    elseif ($idp->export_type & EXPORT_IN_CODE) {
      $status = t('Code');
    }
    else {
      $status = t('Unknown');
    }

    $actions = array();
    $actions[] = l(t('Edit'), "admin/config/people/saml_sp/IDP/{$idp->machine_name}");

    // Only add 'Revert' | 'Delete' if the IDP entry is stored in the DB.
    if ($idp->export_type & EXPORT_IN_DATABASE) {
      $action = ($idp->export_type & EXPORT_IN_CODE) ? t('Revert') : t('Delete');
      $actions[] = l($action, "admin/config/people/saml_sp/IDP/{$idp->machine_name}/delete");
    }

    $actions[] = l(t('Export'), "admin/config/people/saml_sp/IDP/{$idp->machine_name}/export");


    $rows[] = array(
      $idp->name,
      $idp->machine_name,
      $idp->login_url,
      $status,
      implode(' | ', $actions),
    );
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No Identity Providers defined. <a href="@add_idp_link">Add SAML identity provider</a>.', array('@add_idp_link' => url('admin/config/people/saml_sp/IDP/add'))),
  ));
}
