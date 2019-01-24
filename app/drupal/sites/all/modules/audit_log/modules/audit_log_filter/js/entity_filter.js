(function ($) {
  Drupal.behaviors.audit_log_filter_entity_admin = {
    attach: function (context, settings) {
      $('#edit-audit-log-exclude-entity-types input:checkbox').once('auditLogFilterEntityAdmin').click(function() {
        keys = $(this).attr('name').replace(/\]/g,'').split('[');
        if (keys[keys.length-1].indexOf('-all') != -1) {
          $divid = $(this).attr('id').replace(/-all/g,'');
          if ($(this).is(':checked')) {
            $('#' + $divid + ' input:checkbox').attr("checked", "checked");
          }
          else {
            $('#' + $divid + ' input:checkbox').removeAttr("checked");
          }
        }
      });
    }
  }
}(jQuery));
