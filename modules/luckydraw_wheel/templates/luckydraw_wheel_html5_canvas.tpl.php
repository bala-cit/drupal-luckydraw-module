<?php

/**
 * @file
 * Default theme implementation to display a luckydraw wheel drawed using html5 canvas.
 */

?>
<div class="luckydraw-title"><?php print $luckydraw->settings['display_title']; ?></div>
<div class="luckydraw-help-text"><?php print $luckydraw->settings['display_help_text']; ?></div>
<?php if (isset($luckydraw->settings['header'])): ?>
  <div class="luckydraw-header"><?php print $luckydraw->settings['header']; ?></div>
<?php endif; ?>
<div id="venues" style="float: left; display:none">
  <ul>
    <?php foreach ($luckydraw->items as $delta => $item) : ?>
    <li>
      <input
        id="venue-<?php print $delta; ?>"
        name="<?php print $item->name . '-' . $delta; ?>"
        eid="<?php print $item->liid; ?>"
        value="<?php print $item->name; ?>"
        type="checkbox"
        checked="checked">
      <label for="venue-<?php print $delta; ?>"><?php print $item->name; ?></label>
    </li>
    <?php endforeach; ?>
  </ul>
</div>
<div id="wheel">
<canvas id="canvas"
        width="<?php print $luckydraw->settings['html5_canvas']['width'] ?>"
        height="<?php print $luckydraw->settings['html5_canvas']['height'] ?>">
</canvas>
</div>
<div id="stats">
</div>
