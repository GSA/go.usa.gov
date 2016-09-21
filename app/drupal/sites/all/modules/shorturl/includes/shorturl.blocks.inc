<?php

function shorturl_block_info()
{
    $blocks['shorturl_totals'] = array(
        'info' => t('Short URL Totals'),
    );
    $blocks['shorturl_current_user'] = array(
        'info' => t('Short URL Current User'),
    );
    $blocks['most_popular'] = array(
        'info' => t('Short URL Most Popular'),
    );
    // $blocks['search_url'] = array(
    //     'info' => t('Short URL Search Your Links'),
    // );
    // $blocks['my_url'] = array(
    //     'info' => t('Short URL View Your Links'),
    // );
    // $blocks['landing'] = array(
    // 	'info' => t('Short URL Landing'),
    // );
    return $blocks;
}

function shorturl_block_view($delta)
{
    $block = array();
    // don't show the block when user is on the callback page
    switch ($delta) {
        case 'shorturl_totals':
               #$block['subject'] = t('Short URL Totals');
            $block['content'] = _shorturl_get_totals_block_content();
            break;
        case 'shorturl_current_user':
               #$block['subject'] = t('Short URL Totals');
            $block['content'] = _shorturl_current_user_block_content();
            break;
        case 'most_popular':
            $block['subject'] = t('Most Popular Go.USA.gov Links');
            $block['content'] = _shorturl_get_popular_block_content();
            break;
        // case 'search_url':
        //     $block['subject'] = t('Enter a long URL to search your history');
        //     $block['content'] = _shorturl_get_su_block_content();
        //     break;
        // case 'my_url':
        //     $block['subject'] = t('Your Links');
        //     $block['content'] = _shorturl_get_my_block_content();
        //     break;
    }

    return $block;
}

function _shorturl_get_totals_block_content()
{
    $metric_db = _shorturl_metrics_db_name();
    $all_links  = db_query('SELECT COUNT(lid) FROM {shorturl_link}')->fetchField();
    $all_clicks = db_query("SELECT access_count FROM {$metric_db}.shorturl_access_count WHERE count_id = 0")->fetchField();

    $ret = '<p>'.t('!site has shortened !links URLs that have been clicked !clicks times.', array('!site' => variable_get('site_name', 'Drupal'), '!links' => '<strong>'.number_format($all_links).'</strong>', '!clicks' => '<strong>'.number_format($all_clicks).'</strong>')).'</p>';

    return $ret;
}

function _shorturl_current_user_block_content()
{
    global $user;
    if (!empty($user->uid)) {
        return '<div class="user-container">Signed in as '.theme('username', array('account' => $user)).'. '.l('Log out', 'user/logout').'</div>';
    }

    return '<div class="user-container">You are not signed in.</div>';
}

// function _shorturl_get_mp_block_content()
// {
//     drupal_set_title( t("Shortened Link") );
//     return theme('most_popular');
// }
//
function _shorturl_get_popular_block_content($num = 25)
{
    $limit = max(1, min(50, intval($num)));
    drupal_set_title(t('!limit Most Popular !site Links', array('!limit' => $limit, '!site' => variable_get('site_name', 'Drupal'))));

    $metric_db = _shorturl_metrics_db_name();
    $sql       = "
        SELECT l.*, d.links
        FROM (
            SELECT lid, orig_url, title, description, keywords, clicks, created, uid, notes
            FROM {shorturl_link}
            WHERE orig_url <> ''
            ORDER BY clicks DESC
            LIMIT {$limit}
        ) l
        LEFT JOIN {$metric_db}.shorturl_metric_destination d
        ON ( MD5(l.orig_url) = d.destmd5 )
        ORDER BY clicks DESC LIMIT {$limit}
    ";

    $rows   = array();
    $result = db_query($sql);
    foreach ($result as $link) {
        $key    = shorturl_encode_url($link->lid);
        $rows[] = array('orig_url' => $link->orig_url,
            'clicks'               => $link->clicks,
            'date'                 => $link->created,
            'title'                => $link->title,
            'description'          => $link->description,
            'created'              => $link->created,
            'links'                => $link->links,
            'key'                  => $key,
            'notes'                => $link->notes,
            'uid'                  => $link->uid,
        );
    }

    return theme('most_popular', array('content' => $rows));
}
//
// function _shorturl_get_su_block_content()
// {
//     return drupal_get_form('shorturl_search_form');
// }
//
// function _shorturl_get_my_block_content()
// {
//     return theme('my_url');
// }

function shorturl_theme()
{
    return array(
        'most_popular' => array(
            'variables' => array('num' => null), // 'content'=>NULL
            'template'  => 'block--shorturl--most-popular',
            'path'      => drupal_get_path('module', 'shorturl').'/templates',
        ),
        'search_url' => array(
            'variables' => array('vars' => null),
            'template'  => 'templates/search_url',
        ),
        'my_url' => array(
            'variables' => array('vars' => null),
            'template'  => 'templates/my_url',
        ),
        'landing' => array(
            'variables' => array('vars' => null),
            'template'  => 'templates/shorturl_landing',
        ),
    );
}
