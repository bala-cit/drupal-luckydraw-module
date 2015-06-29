(function($) {
//    Drupal.luckydraw.LuckydrawWheel = Drupal.luckydraw.LuckydrawWheel || {};

    Drupal.behaviors.LuckyDrawWheel = {
        attach: function (context, settings) {

            // Helpers
            shuffle = function(o) {
                for ( var j, x, i = o.length; i; j = parseInt(Math.random() * i), x = o[--i], o[i] = o[j], o[j] = x)
                    ;
                return o;
            };

            String.prototype.hashCode = function(){
                // See http://www.cse.yorku.ca/~oz/hash.html
                var hash = 5381;
                for (i = 0; i < this.length; i++) {
                    char = this.charCodeAt(i);
                    hash = ((hash<<5)+hash) + char;
                    hash = hash & hash; // Convert to 32bit integer
                }
                return hash;
            }

            Number.prototype.mod = function(n) {
                return ((this%n)+n)%n;
            }

            // WHEEL!
            var wheel = {

                timerHandle : 0,
                timerDelay : 30,

                // Set the initial angle
                angleCurrent : 0,
                angleDelta : 0,
                stopAngle : 0,

                size : parseInt(Drupal.settings.luckydraw_wheel.size),

                canvasContext : null,

                colors : [ '#ffff00', '#ffc700', '#ff9100', '#ff6301', '#ff0000', '#c6037e',
                    '#713697', '#444ea1', '#2772b2', '#0297ba', '#008e5b', '#8ac819' ],

                segments : [],

                entities : [],
                seg_colors : [], // Cache of segments to colors

                duration : 5000,

                spinStart : 0,

                frames : 0,

                centerX : parseInt(Drupal.settings.luckydraw_wheel.centerX),
                centerY : parseInt(Drupal.settings.luckydraw_wheel.centerY),

                message : {},

                spin : function() {

                    // Start the wheel only if it's not already spinning
                    if (wheel.timerHandle == 0) {
                        // just set to prevent user keep clicking, while getting result from backend
                        wheel.timerHandle = true;

                        // Call luckydraw core function will get
                        Drupal.luckydraw.go(1, function(resp) {
                            var angleMin = (360 - parseInt(resp.result.data.angle.min)) * Math.PI / 180;
                            var angleMax = (360 - parseInt(resp.result.data.angle.max)) * Math.PI / 180;

                            // TODO: Added
                            //    * Math.PI / 180;
                            while (angleMin >= Math.PI * 2)
                                angleMin -= Math.PI * 2;
                            while (angleMax >= Math.PI * 2)
                                angleMax -= Math.PI * 2;

                            wheel.stopAngle = parseInt(Drupal.settings.luckydraw_wheel.additionalCycle) * 2 * Math.PI
                                + (Math.random() * (angleMax - angleMin) + angleMin);
                            //
                            //console.log(resp);
                            //console.log(wheel.stopAngle / Math.PI * 180);

                            wheel.message.html = resp.html;
                            wheel.message.type = resp.status;
                            wheel.message.result = resp.result;

                            if (resp.status != 'ok') {
                              Drupal.luckydraw.message(wheel.message);
                             // return;
                            }

                            wheel.spinStart = new Date().getTime();
                            wheel.frames = 0;
                            //wheel.sound.play();

                            wheel.timerHandle = setInterval(wheel.onTimerTick, wheel.timerDelay);
                        });
                    }
                },

                onTimerTick : function() {

                    wheel.frames++;
                    wheel.draw();

                    var actualTime = +new Date().getTime();
                    var finished = (actualTime - wheel.spinStart) > wheel.duration;
                    console.log(finished);

                    if (finished) {
                        clearInterval(wheel.timerHandle);
                        wheel.timerHandle = 0;
                        wheel.angleDelta = 0;

                    } else {
                        //console.log(wheel.angleCurrent / Math.PI);
                        // jQuery.easing.easeOutSine = function (x, t, b, c, d) {
                        //    return c * Math.sin(t/d * (Math.PI/2)) + b;
                        // }
                        wheel.angleCurrent = $.easing.easeOutSine(
                            0,                                      // x, extra parameter, not needed for equations
                            actualTime - wheel.spinStart,           // t, current time
                            wheel.angleCurrent,                     // b, begin value
                            wheel.stopAngle - wheel.angleCurrent,   // c, change in value
                            wheel.duration                          // d, duration
                        );
                    }
                },

                init : function(optionList) {
                    try {
                        wheel.initWheel();
                        //wheel.initAudio();
                        wheel.initCanvas();

                        // TODO: ORIGINAL
                        //wheel.draw();

                        $.extend(wheel, optionList);

                    } catch (exceptionData) {
                        alert('Wheel is not loaded ' + exceptionData);
                    }

                },

                initAudio : function() {
                    var sound = document.createElement('audio');
                    sound.setAttribute('src', 'wheel.mp3');
                    wheel.sound = sound;
                },

                initCanvas : function() {
                    var canvas = $('#wheel #canvas').get(0);

                    if ($.browser.msie) {
                        canvas = document.createElement('canvas');
                        var height = Drupal.settings.luckydraw_wheel.height;
                        var width = Drupal.settings.luckydraw_wheel.width;
                        $(canvas).attr('width', width).attr('height', height).attr('id', 'canvas').appendTo('.wheel');

                        canvas = G_vmlCanvasManager.initElement(canvas);
                    }

                    canvas.addEventListener("click", wheel.spin, false);
                    wheel.canvasContext = canvas.getContext("2d");
                },

                initWheel : function() {
                    shuffle(wheel.colors);
                },

                // Called when segments have changed
                update : function() {
                    // Ensure we start mid way on a item
                    //var r = Math.floor(Math.random() * wheel.segments.length);
                    //var r = 0;
                    //wheel.angleCurrent = ((r + 0.5) / wheel.segments.length) * Math.PI * 2;

                    var segments = wheel.segments;
                    var len      = segments.length;
                    var colors   = wheel.colors;
                    var colorLen = colors.length;

                    // Generate a color cache (so we have consistant coloring)
                    var seg_color = new Array();
                    for (var i = 0; i < len; i++)
                        seg_color.push( colors [ segments[i].hashCode().mod(colorLen) ] );

                    wheel.seg_color = seg_color;

                    wheel.draw();
                },

                draw : function() {
                    wheel.clear();
                    wheel.drawWheel();
                    wheel.drawNeedle();
                },

                clear : function() {
                    var ctx = wheel.canvasContext;
                    ctx.clearRect(0, 0, 1000, 800);
                },

                drawNeedle : function() {
                    var ctx = wheel.canvasContext;
                    var centerX = wheel.centerX;
                    var centerY = wheel.centerY;
                    var size = wheel.size;
                    //var radius = Drupal.settings.luckydraw_wheel.centerRadius;

                    ctx.lineWidth = 1;
                    ctx.strokeStyle = '#000000';
                    ctx.fileStyle = '#ffffff';

                    ctx.beginPath();

                    ctx.moveTo(centerX + size - 40, centerY);
                    ctx.lineTo(centerX + size + 20, centerY - 10);
                    ctx.lineTo(centerX + size + 20, centerY + 10);

                    ctx.closePath();

                    ctx.stroke();
                    ctx.fill();

                    // Which segment is being pointed to?
                    //var i = wheel.segments.length - Math.floor((wheel.angleCurrent / (Math.PI * 1.5)) * wheel.segments.length) - 1;

                    // Now draw the winning name
                    //ctx.textAlign = "left";
                    //ctx.textBaseline = "middle";
                    //ctx.fillStyle = '#000000';
                    //ctx.font = Drupal.settings.luckydraw_wheel.needleFont;

                    //ctx.fillText(wheel.segments[i], centerX, centerY - size - 25);
                },

                drawSegment : function(key, lastAngle, angle) {
                    var ctx = wheel.canvasContext;
                    var centerX = wheel.centerX;
                    var centerY = wheel.centerY;
                    var size = wheel.size;

                    var segments = wheel.segments;
                    var len = wheel.segments.length;
                    var colors = wheel.seg_color;

                    var value = segments[key];

                    ctx.save();
                    ctx.beginPath();

                    // Start in the centre
                    ctx.moveTo(centerX, centerY);
                    ctx.arc(centerX, centerY, size, lastAngle, angle, false); // Draw a arc around the edge
                    ctx.lineTo(centerX, centerY); // Now draw a line back to the centre

                    // Clip anything that follows to this area
                    //ctx.clip(); // It would be best to clip, but we can double performance without it
                    ctx.closePath();

                    ctx.fillStyle = colors[key];
                    ctx.fill();
                    ctx.stroke();

                    // Now draw the text
                    ctx.save(); // The save ensures this works on Android devices
                    ctx.translate(centerX, centerY);
                    ctx.rotate((lastAngle + angle) / 2);

                    ctx.fillStyle = '#000000';
                    ctx.fillText(value.substr(0, 20), size / 2 + 20, 0);
                    ctx.restore();

                    ctx.restore();
                },

                drawWheel : function() {
                    var ctx = wheel.canvasContext;

                    var angleCurrent = wheel.angleCurrent;

                    var lastAngle    = angleCurrent;

                    var segments  = wheel.segments;
                    var len       = wheel.segments.length;
                    var colors    = wheel.colors;
                    var colorsLen = wheel.colors.length;

                    var centerX = wheel.centerX;
                    var centerY = wheel.centerY;
                    var size    = wheel.size;
                    var radius  = parseInt(Drupal.settings.luckydraw_wheel.centerRadius);

                    var PI2 = Math.PI * 2;

                    ctx.lineWidth    = 1;
                    ctx.strokeStyle  = '#000000';
                    ctx.textBaseline = "middle";
                    ctx.textAlign    = "center";
                    ctx.font         = Drupal.settings.luckydraw_wheel.wheelFont;

                    for (var i = 1; i <= len; i++) {
                        var angle = PI2 * (i / len) + angleCurrent;
                        wheel.drawSegment(i - 1, lastAngle, angle);
                        lastAngle = angle;
                    }
                    // Draw a center circle
                    ctx.beginPath();
                    ctx.arc(centerX, centerY, radius, 0, PI2, false);
                    ctx.closePath();

                    ctx.fillStyle   = '#ffffff';
                    ctx.strokeStyle = '#000000';
                    ctx.fill();
                    ctx.stroke();

                    // Draw outer circle
                    ctx.beginPath();
                    ctx.arc(centerX, centerY, size, 0, PI2, false);
                    ctx.closePath();

                    ctx.lineWidth   = 10;
                    ctx.strokeStyle = '#000000';
                    ctx.stroke();
                },
            }

            window.onload = function() {
                wheel.init();

                var segments = new Array();
                var entities = new Array();
                $.each($('#venues input:checked'), function(key, cbox) {
                    segments.push( cbox.value );
                    entities.push( $(this).attr('eid') );
                });

                wheel.segments = segments;
                wheel.entities = entities;
                wheel.update();

                // Hide the address bar (for mobile devices)!
                setTimeout(function() {
                    window.scrollTo(0, 1);
                }, 0);
            }

        }
    }
})(jQuery);
