(function($) {
    $(document).ready(function() {
        $(document).on('change', 'select[name$="keyword_template"]', function() {
            var select = $(this);
            var keywordId = select.val();
            if (!keywordId) return;

            $.getJSON('/api/keywordtemplates/' + keywordId + '/', function(data) {
                var row = select.closest('tr');

                row.find('select[name$="trigger"]').val(data.trigger);
                row.find('select[name$="effect"]').val(data.effect);
                row.find('select[name$="value"]').val(data.value);
                row.find('select[name$="targeting"]').val(data.targeting);
                row.find('select[name$="targeting_rule"]').val(data.targeting_rule);
                row.find('select[name$="condition"]').val(data.condition);
                row.find('input[name$="condition_value"]').val(data.condition_value);
                row.find('select[name$="zone"]').val(data.zone);
                row.find('select[name$="scope"]').val(data.scope);
                row.find('select[name$="linked_card"]').val(data.linked_card);
            });
        });
    });
})(django.jQuery);
