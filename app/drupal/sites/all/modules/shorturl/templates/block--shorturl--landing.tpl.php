<?php
$args = arg();
?>

<div class="tabs"><ul class="tabs primary">
        <li <?php

        if (isset($args) && count($args) == 2 && $args[0] == "shorturl" && $args[1] == "all") {
            print 'class="active"';
        }
        ?>><a href="/shorturl/all">All users</a></li>

        <li <?php

        if (isset($args) && count($args) == 2 && $args[0] == "shorturl" && $args[1] == "search") {
            print 'class="active"';
        }
        ?>><a href="/shorturl/search" class="active">Search Your URLs</a></li>

        <li <?php

        if (isset($args) && count($args) == 1 && $args[0] == "shorturl") {
            print 'class="active"';
        }
        ?>><a href="/shorturl">Your URLs</a></li>

    </ul>
</div>

<div class="content">
<?php

if (isset($args) && count($args) == 2 && $args[0] == "shorturl" && $args[1] == "all") {
    print '<h2 class="page-title">25 Most Popular Go.USA.gov Links</h2>';
    $block = module_invoke('shorturl', 'block_view', 'most_popular');
    print render($block['content']);
}

if (isset($args) && count($args) == 2 && $args[0] == "shorturl" && $args[1] == "search") {
    $block = module_invoke('shorturl', 'block_view', 'search_url');
    print render($block['content']);
}

if (isset($args) && count($args) == 1 && $args[0] == "shorturl") {
    $block = module_invoke('shorturl', 'block_view', 'my_url');
    print render($block['content']);
}

?>

</div>
