function vbarGraphInit( id, o )
{
    var t = jQuery( '#' + id );
    if ( !t )
    {
        return;
    }

    var h = ( o && o.height ) ? o.height : 16;
    var w = ( o && o.width ) ? o.width : 36;

    t.addClass( 'hbar-graph' );
    t.css( 'height', h + 'em' );
    t.css( 'width', w + 'em' );

    var em = ( t.outerHeight() / h );

    jQuery( 'tfoot, tbody, tbody tr', t )
        .css( 'height', ( h - 6 ) + 'em' );

    var bars = jQuery( 'tbody tr', t );
    if ( !bars.length )
    {
        return;
    }

    var barWidth = Math.round( t.width() / bars.length );
    jQuery( 'tbody tr, tbody td, tbody th', t )
        .css( 'width', barWidth + 'px' );

    var twidth = t.width();
    var space = ( ( ( barWidth + 2 ) * bars.length ) - twidth );
    t.css( 'width', ( twidth + space ) + 'px' );

    var sumValue = 0;
    var scaleValue = 0;
    bars.each( function ( i )
    {
        var valueNode = jQuery( '.value', this );
        if ( valueNode )
        {
            var value = getNumVal( valueNode );
            sumValue += value;
            if ( value > scaleValue )
            {
                scaleValue = value;
            }
        }
    } );
    // var totalClicksNode = jQuery('.totalClicks',t);
    // if ( totalClicksNode )
    // {
    //   totalClicksNode.html(sumValue.toLocaleString());
    // }
    bars.each( function ( i )
    {
        var valueNode = jQuery( '.value', this )
            .get( 0 );
        if ( !valueNode )
        {
            return;
        }

        if ( jQuery( valueNode )
            .attr( 'height' ) )
        {
            rowScale = jQuery( valueNode )
                .attr( 'height' );
            jQuery( valueNode )
                .css( 'height', rowScale );
            return;
        }
        var rowValue = getNumVal( valueNode );
        var rowScale = 0;
        var scaleNode = jQuery( '.scale', this )
            .get( 0 );
        if ( scaleNode ) /// this is always a percentage
        {
            rowScale = Math.max( 0, Math.min( 100, getNumVal( scaleNode ) ) );
            if ( rowScale >= 0 )
            {
                jQuery( valueNode )
                    .css( 'height', rowScale + '%' );
                return;
            }
        }
        rowScale = Math.max( 0, Math.min( 100, ( rowValue / scaleValue ) * 100 ) );
        jQuery( valueNode )
            .css( 'height', rowScale + '%' );
    } );

    function getNumVal( node )
    {
        return parseFloat( jQuery( node )
            .text()
            .replace( /^\D+/, '' ) );
    }
}

function vbarGraphNav( id )
{
    var t = jQuery( '#' + id );
    if ( !t )
    {
        return;
    }

    t.attr( 'tabIndex', '0' );
    t.keydown( keyNav );

    var bars = jQuery( 'tbody tr', t )
        .attr( 'tabIndex', '0' )
        .focus( function ()
        {
            jQuery( this )
                .addClass( 'selected' );
        } )
        .blur( function ()
        {
            jQuery( this )
                .removeClass( 'selected' );
        } )

    function keyNav( event )
    {
        switch ( event.which )
        {
        case 37: // left arrow --> go to previous item
            event.preventDefault();
            var curr = jQuery( 'tbody tr.selected', t );
            if ( curr.length )
            {
                var prev = curr.prev( 'tr' );
                if ( prev.length )
                {
                    curr.blur();
                    prev.focus();
                }
            }
            else
            {
                jQuery( 'tbody tr:last-child', t )
                    .focus();
            }
            break;
        case 39: // right arrow --> go to previous item
            event.preventDefault();
            var curr = jQuery( 'tbody tr.selected', t );
            if ( curr.length )
            {
                var next = curr.next( 'tr' );
                if ( next.length )
                {
                    curr.blur();
                    next.focus();
                }
            }
            else
            {
                jQuery( 'tbody tr:first-child', t )
                    .focus();
            }
            break;
        case 36: // home   --> go to first item
        case 33: // pageup --> go to first item
            event.preventDefault();
            var curr = jQuery( 'tbody tr.selected', t );
            if ( curr.length )
            {
                curr.blur();
            }
            jQuery( 'tbody tr:first-child', t )
                .focus();
            break;
        case 35: // end      --> go to last item
        case 34: // pagedown --> go to last item
            event.preventDefault();
            var curr = jQuery( 'tbody tr.selected', t );
            if ( curr.length )
            {
                curr.blur();
            }
            jQuery( 'tbody tr:last-child', t )
                .focus();
            break;
        case 27: // Escape --> cancel selection
            event.preventDefault();
            var curr = jQuery( 'tbody tr.selected', t );
            if ( curr.length )
            {
                curr.blur();
            }
            jQuery( 'tr.selected', t )
                .blur();
            break;
        }
    }
}