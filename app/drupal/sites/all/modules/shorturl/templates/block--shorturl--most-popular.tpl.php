
<table cellspacing="10" summary="Information on each shortened URL, including the number of clicks, date it was shortened, and the URL" class="sticky-enabled sticky-table">
<thead class="tableHeader-processed"><tr><th scope="col">Info</th><th scope="col">Clicks</th><th scope="col">Date</th> </tr></thead>
<tbody>
<?php

if (isset($content)) {

    $i=0;
    global $user;
    foreach ($content as $alink)
	{
        if (!empty($alink['title']) && strlen($alink['title']) > 0)
		{
            $title = html_entity_decode($alink['title'], ENT_QUOTES, 'UTF-8');
            $url = (strlen($alink['orig_url']) > 65) ? substr($alink['orig_url'], 0, 65).'...' : $alink['orig_url'];
            $title = (strlen($title) > 50) ? substr($title, 0, 50).'...' : $title;
            $details = '<div class="link">'.l($title,$alink['orig_url']).'</div><div class="url">'.htmlspecialchars($url).'</div>';
        } else {
            $url = (strlen($alink['orig_url']) > 45) ? substr($alink['orig_url'], 0, 45).'...' : $alink['orig_url'];
            $details = '<div class="link">'.l($url, $alink['orig_url']).'</div>';
        }

        if (!empty($alink['description']) && strlen($alink['description']) >0)
		{
            $description = html_entity_decode($alink['description'], ENT_QUOTES, 'UTF-8');
            $details .= '<div class="description">'. $description .'</div>';
        }
        $key = $alink['key'];

        if (user_access('view link details'))
		{
            $links = (!empty($alink['links'])) ? $alink['links'] : 1;
            if ($links == 2 ) {
                $details .= '<div class="details">'.l(t('view the 2 links to this page'), 'shorturl/list/'.$key).'</div>';
            } else if ($links > 2) {
                $details .= '<div class="details">'.l(t('view all !links links to this page', array('!links' => $links)), 'shorturl/list/'.$key).'</div>';
            } else {
                $details .= '<div class="details">'.l(t('metrics'), 'shorturl/link/'.$key) .' '. t('!url', array('!url' => _shorturl_remove_https(url($key, array('absolute' => TRUE))) )) ."</div>";
            }
        }

        $details .= '<span id="shorturl_link_notes_wrap_'.$key.'">';
        if ( $user->uid == $alink['uid'] )
        {
            $details .= '  <span id="shorturl_link_notes_view_'.$key.'" class="notes-view">';
            $details .= '    <a href="javascript:edit_notes(\''.$key.'\')" class="edit-notes">'.t("notes").'</a> ';
            $details .= '    <span id="shorturl_link_notes_display_'.$key.'">'.filter_xss($alink['notes']).'</span>';
            $details .= '  </span>';
            $details .= '  <span id="shorturl_link_notes_manage_'.$key.'" class="notes-edit">';
            $details .= '    <a href="javascript:save_notes(\''.$key.'\')" class="save-notes">'.t("save").'</a> ';
            $details .= '    <textarea id="shorturl_link_notes_edit_'.$key.'" class="notes-value"></textarea>';
            $details .= '  </span>';
        } else if ( user_access('administer shorturl') && !empty($alink['notes']) ) {
            $details .= '<span id="shorturl_link_notes_display_'.$key.'" class="notes-view"> notes: '.filter_xss($alink['notes']).'</span>';
        }
        $details .= '</span>';

        print "<tr class='" . ($i%2==0? "even":"odd") . "'>";
        print "<th scope='row' class='details'>" . $details . "</th>";
        print "<td class='clicks'>" . number_format($alink['clicks']) . "</td>";
        print "<td class='date'>" .format_date($alink['created'], variable_get('shorturl_date_format', 'short'), variable_get('shorturl_custom_date_format', '')) . "</td>";
        print "</tr>";

    }
}
?>
</tbody>
</table>
