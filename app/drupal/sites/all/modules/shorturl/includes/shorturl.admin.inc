<?php

// $Id: shorturl.admin.inc,v 1.1.2.1 2009/08/08 06:52:50 inadarei Exp $

/**
 * @file
 * Admin callbacks for the ShortURL module.
 */

/**
 * Implementation of hook_admin_settings().
 */
function shorturl_admin_settings_form($form,    &$form_state)
{
    $time                    = time();
    $form['shorturl_engine'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Engine Options'),
        '#collapsible' => true,
        '#collapsed'   => false,
    );
    $form['shorturl_engine']['shorturl_domain'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Short Domain:'),
        '#default_value' => variable_get('shorturl_domain', $_SERVER['HTTP_HOST']),
        '#required'      => true,
        '#description'   => t('The domain to be used in short URLs'),
    );
    $form['shorturl_engine']['shorturl_start_from'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Start From ID#:'),
        '#default_value' => variable_get('shorturl_start_from', 3844),
        '#required'      => true,
        '#description'   => t('3844 is approximately the first 3-characters long short URI. Indicate a number that is less or more, depending on your needs.'),
    );
    $form['shorturl_engine']['shorturl_allow_duplicates'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Allow duplicate submissions'),
        '#description'   => t('Allows user to create more than one link to the same URL.  This is important for tracking links by users and for targeting different links to different roups for the purpose of comparing metrics.'),
        '#default_value' => variable_get('shorturl_allow_duplicates', 0),
        '#return_value'  => 1,
    );
    $form['shorturl_statistics'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Statistics Options'),
        '#collapsible' => true,
        '#collapsed'   => false,
    );
    $form['shorturl_statistics']['shorturl_track_clicks'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Track Clickthroughs'),
        '#default_value' => variable_get('shorturl_track_clicks', 1),
        '#return_value'  => 1,
    );
    $form['shorturl_statistics']['shorturl_date_format'] = array(
        '#type'    => 'select',
        '#title'   => t('Date format'),
        '#options' => array(
            'short'  => format_date($time, 'short'),
            'medium' => format_date($time, 'medium'),
            'long'   => format_date($time, 'long'),
            'custom' => t('Custom'),
        ),
        '#default_value' => variable_get('shorturl_date_format', 'short'),
    );
    $form['shorturl_statistics']['shorturl_custom_date_format'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Custom date format'),
        '#description'   => t('If "Custom", see <a href="http://us.php.net/manual/en/function.date.php" target="_blank">the PHP docs</a> for date formats.'),
        '#default_value' => variable_get('shorturl_custom_date_format', ''),
    );

    return system_settings_form($form);
}
