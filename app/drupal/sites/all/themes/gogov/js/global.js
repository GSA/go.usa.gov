(function($){
	$(document).ready(function(){


		edit_notes = function(key){
			$("#shorturl_link_notes_edit_"   + key).val($("#shorturl_link_notes_display_" + key).html());
			$("#shorturl_link_notes_view_"   + key).hide();
			$("#shorturl_link_notes_manage_" + key).show();
			$("#shorturl_link_notes_edit_"   + key).focus();
		}
		save_notes = function(key){

			jQuery.ajax({ 
				type:"get", url:"/shorturl/notes/"+key,
				data:{notes:jQuery("#shorturl_link_notes_edit_"+key).val()},
				dataType:"jsonp", crossDomain:true, jsonp:"callback",
				success:function(data,status,jqxhr){
					if ( data && data.hasOwnProperty("notes") ) {
						$("#shorturl_link_notes_display_"+key).html(data.notes);
					}
				},
				complete:function(jqxhr,status){
					$("#shorturl_link_notes_view_"   + key).show();
					$("#shorturl_link_notes_manage_" + key).hide();
				}
			});
		}

			
	});
})(jQuery);