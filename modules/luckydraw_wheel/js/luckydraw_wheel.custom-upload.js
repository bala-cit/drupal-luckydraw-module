// Register luckydraw wheel namespace
Drupal.luckydraw_wheel = Drupal.luckydraw_wheel || {};

(function($) {
    $(document).ready(function() {

        $('#needle-startbtn').click(function(e) {
            // console.log($(this).attr('data-id'));
            Drupal.luckydraw_wheel.luckydraw($(this).attr('data-id'));
        });

    });

    Drupal.luckydraw_wheel.luckydraw = function(luckydraw_id) {
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
                $("#needle-startbtn").unbind('click').css("cursor", "default");

                // Generate a number between the angle range min - max
                var a = Math.round(Math.random() * (resp.result.data.angle.max - resp.result.data.angle.min) + resp.result.data.angle.min);
                var p = resp.result.name;

                $("#wheel-rotate").rotate({
                    duration: 3000,
                    angle: 0,
                    animateTo: 1800+a,
                    easing: $.easing.easeOutSine,
                    callback: function() {
                        var confirmed = confirm(Drupal.t("Configuration, you've drawn" + p + "! Try again?"));
                        if (confirmed) {
                            Drupal.luckydraw_wheel.luckydraw(luckydraw_id);
                        } else {
                            return false;
                        }
                    }
                });
            }
        });
    }
}(jQuery));