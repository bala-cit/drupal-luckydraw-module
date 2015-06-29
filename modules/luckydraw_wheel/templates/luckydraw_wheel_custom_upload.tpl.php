<?php

/**
 * This template created for user uploaded wheel materials, such as needle, wheel, start button
 */

?>
<div class="luckydraw-title"><?php print $luckydraw->settings['display_title']; ?></div>
<div class="ld-line">
    <div class="luckydraw-help-text"><?php print $luckydraw->settings['display_help_text']; ?></div>
    <?php if (isset($header)): ?>
      <div class="luckydraw-header">
      <?php foreach ($header as $key => $value): ?>
        <div class="<?php print $key; ?>"><?php print $value; ?></div>
      <?php endforeach; ?>
      </div>
    <?php endif; ?>
</div>
<div class="luckydraw-board">
  <div class="bending"></div>
  <div class="needle" id="needle-arrow">
  </div>
  <div class="wheel">
       <img src="<?php print $luckydraw->settings['upload']['wheel_path']; ?>"  id="wheel-rotate" />
  </div>
  <div class="start-btn" id="startbtn"
       data-id="<?php print $luckydraw->lid; ?>"
       style="background-image: url(<?php print $luckydraw->settings['upload']['startbtn_path']; ?>);">
  </div>
</div>