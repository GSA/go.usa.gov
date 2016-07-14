
(function ($) {

Drupal.behaviors.shorten = {
  attach: function (context) {
    // Make sure we can run context.find().
    var ctxt = $(context);
    var shortenedURL = ctxt.find('.shorten-shortened-url');
    var shorten = shortenedURL[0];
    if (shorten) {
      shorten.select();
      shorten.focus();
    }
    shortenedURL.click(function() {
      this.select();
      this.focus();
    });
  }
};

})(jQuery);

function prependShortRow (orig_url,short_url)
{
    var pageTitle = document.getElementById('page-title');
    if ( !pageTitle ) { return; }
    if (!( ('textContent' in pageTitle && pageTitle.textContent=='Your Links') ||
           ('innerText'   in pageTitle && pageTitle.innerText=='Your Links')   ||
           ('innerHTML'   in pageTitle && pageTitle.innerHTML=='Your Links') )) {  return; }
    var table = document.querySelector('#shorturl-statistics table:last-of-type');
    if ( !table ) { return; }
    var currDate = new Date();
    var newRow   = document.createElement('tr');
    newRow.classList.add('details');
    newRow.innerHTML = '<td class="details"><div class="link"><a href="'+ orig_url +'">'+ orig_url +'</a></div>'
                     + '<div class="details"><a href="/shorturl/link/'+ short_url.substr(short_url.lastIndexOf('/')+1) +'">metrics</a> '+ short_url +'</div></td>'
                     + '<td class="clicks"><div class="count">0</div></td>'
                     + '<td class="date">'+(currDate.getMonth()+1)+'/'+currDate.getDate()+'/'+currDate.getFullYear()+'</td>';
    var firstRow = table.tBodies[0].querySelector('tr:first-of-type');
    if ( !firstRow ) {
        firstRow = table.tBodies[0].firstChild;
    }
    if ( firstRow ) {
        table.tBodies[0].insertBefore(newRow,firstRow);
    } else {
        table.tBodies[0].appendChild(newRow);
    }
}
