// Register luckydraw wheel namespace
Drupal.luckydraw_wheel = Drupal.luckydraw_wheel || {};

(function($) {

    Drupal.behaviors.LuckyDrawWheel = {
        attach: function (context, settings) {

            var wheel = {

                duration : 6000,

                angle : 0,

                additionalAngle : 360 * parseInt(settings.luckydraw_wheel.additionalCycle),

                spin : function(luckydraw_id) {
                    //console.log(luckydraw_id);
                    Drupal.luckydraw.go(luckydraw_id, function (response) {
                        var angleMin = parseInt(response.result.data.angle.min);
                        var angleMax = parseInt(response.result.data.angle.max);

                        if (response.status != 'ok') {
                            Drupal.luckydraw.message(response);
                            return;
                        }

                        // Generate a number between the angle range min - max
                        var angle = Math.floor(Math.random() * (angleMax - angleMin)) + angleMin;

                        wheel.rotate(angle, response);
                    });
                },

                rotate : function(angle, result) {
                    $('#wheel-rotate', context).rotate({
                        duration: wheel.duration,
                        angle: wheel.angle,
                        animateTo: wheel.additionalAngle + angle,
                        easing: $.easing.easeOutSine,
                        callback: function () {
                            // Call luckydraw core module's message function
                            // and show the result
                            Drupal.luckydraw.message(result);
                        }
                    });
                },

                init : function() {
                    $('#startbtn', context).bind('click', function() {
                        wheel.spin($(this).attr('data-id'));
                    });
                }
            };

            window.onload = function() {
                wheel.init();
            }
        }
    }
}(jQuery));