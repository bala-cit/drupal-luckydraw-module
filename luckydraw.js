/**
 * Intends to create some core / common js functions for other module to integrate with
 */

// Define a namespace luckydraw
Drupal.luckydraw = Drupal.luckydraw || {};

(function($) {
    Drupal.luckydraw.go = function(luckydraw_id, callback) {
        // console.log(luckydraw_id);
        $.ajax({
            type: 'GET',
            url: '/luckydraw/process/js/' + luckydraw_id,
            dataType: 'json',
            cache: false,
            error: function() {
                alert(Drupal.t('An error has occurred! Please try again later.'));
                return false;
            },
            success: function(resp) {
                console.log(resp);
                if (resp.status == 'error') {
                    Drupal.luckydraw.message(resp);
                } else {
                    callback(resp);
                }
            }
        });
    }

    // To display the result to end user
    // You may want to override this function in your own module / theme
    Drupal.luckydraw.message = function(response) {
        console.log(response);
        if (response.status == 'error') {
            alert(response.result);
        } else {
            $('#block-system-main').css('height', '540').after($(response.result.data.message.value));
        }
    }
})(jQuery);