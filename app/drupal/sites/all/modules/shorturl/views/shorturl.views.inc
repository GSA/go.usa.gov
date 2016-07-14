<?php

/**
 * @file
 * ShortUrl Views data include file
 */

function shorturl_views_data() {

    // Basic table information.

    $data['shorturl_link']['table']['group']  = t('shorturl_link');

    // Advertise this table as a possible base table
    $data['shorturl']['table']['base'] = array(
        'field' => 'lid',
        'title' => t('ShortURLs'),
        'help' => t('ShortURLs.'),
        'weight' => 10,
    );


    // note that this next line adds to the $data['users'] array
    // and avoids us from having to do relationships in order to get
    // user data into our Views of shorturl_link links
    $data['users']['table']['join']['shorturl_link'] = array(
        'left_field' => 'uid',
        'field' => 'uid',
    );

    // User ID
    $data['shorturl_link']['uid'] = array(
        'title' => t('User ID'),
        'help' => t('ID of user who created the link.'),
        'field' => array(
            'handler' => 'views_handler_field_user',
            'click sortable' => TRUE,
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_user_uid',
            'name field' => 'name', // display this field in the summary
        ),
        'filter' => array(
            'title' => t('Name'),
            'handler' => 'views_handler_filter_user_name',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
            'handler' => 'views_handler_relationship',
            'base' => 'users',
            'base field' => 'uid',
            'label' => t('user'),
        ),
    );

    // uid
    $data['shorturl_link']['uid_current'] = array(
        'real field' => 'uid',
        'title' => t('Current user'),
        'help' => t('Filter the view to the currently logged in user.'),
        'filter' => array(
            'handler' => 'views_handler_filter_user_current',
            'type' => 'yes-no',
        ),
    );


    // shorturl Desination - Long URL
    $data['shorturl_link']['orig_url'] = array(
        'title' => t('Original URL'),
        'help' => t('The destination URL'),
        'field' => array(
            //'handler' => 'shurly_handler_field_destination',
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    // Link created timestamp
    $data['shorturl_link']['created'] = array(
        'title' => t('Created date'),
        'help' => t('The date/time the link was created.'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_date',
        ),
    );

    // remote_ip
    $data['shorturl_link']['remote_ip'] = array(
        'title' => t('Remote Ip'),
        'help' => t('Remote IP'),
        'field' => array(
            //'handler' => 'shurly_handler_field_destination',
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    // Title
    $data['shorturl_link']['title'] = array(
        'title' => t('Title'),
        'help' => t('Title'),
        'field' => array(
            //'handler' => 'shurly_handler_field_destination',
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    // Description
    $data['shorturl_link']['description'] = array(
        'title' => t('Description'),
        'help' => t('Description'),
        'field' => array(
            //'handler' => 'shurly_handler_field_destination',
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    // Keywords
    $data['shorturl_link']['keywords'] = array(
        'title' => t('Keywords'),
        'help' => t('Keywords'),
        'field' => array(
            //'handler' => 'shurly_handler_field_destination',
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    // Clicks;
    $data['shorturl_link']['clicks'] = array(
        'title' => t('Clicks'),
        'help' => t('Number of times this link has been followed.'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
            'allow empty' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),

    );

    // Keywords
    $data['shorturl_link']['notes'] = array(
        'title' => t('Notes'),
        'help' => t('Notes'),
        'field' => array(
            //'handler' => 'shurly_handler_field_destination',
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );



    /*$data['shurly']['link_test'] = array(
        'field' => array(
            'title' => t('Test URL'),
            'help' => t('Redirecting link.'),
            'handler' => 'shurly_handler_field_shurly_link_test',
            'parent' => 'views_handler_field',
        ),
    );

    $data['shurly']['link_delete'] = array(
        'field' => array(
            'title' => t('Delete URL'),
            'help' => t('Link to delete the URL'),
            'handler' => 'shurly_handler_field_shurly_link_delete',
            'parent' => 'views_handler_field',
        ),
    );*/

    return $data;
}

