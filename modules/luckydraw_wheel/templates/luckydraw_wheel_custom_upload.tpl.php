<?php

/**
 * This template created for user uploaded wheel materials, such as needle, wheel, start button
 */

?>
<div class="title"><?php print $luckydraw->settings['display_title']; ?></div>
<div class="help-text"><?php print $luckydraw->settings['display_help_text']; ?></div>
<div class="luckydraw-board">
  <div class="needle">
    <img id="needle-startbtn" data-id="<?php print $luckydraw->lid; ?>" src="<?php print $luckydraw->settings['upload']['needle']; ?>">
  </div>
  <div class="wheel">
    <img id="wheel-rotate" src="<?php print $luckydraw->settings['upload']['wheel']; ?>">
  </div>
</div>