<?php
/**
 * @file
 * Describe hooks provided by luckydraw module
 */

/**
 * Define the type of luckydraw includes name,
 *
 * @return
 *  Array of luckydraw info
 */
function hook_luckydraw_info() {

  return array(
    'name' => 'Example luckydraw'
  );
}

/**
 * Allow other module to add other information before rendering
 * Usually, it's used for adding theming.
 * Example, see luckydraw_wheel module.
 */
function hook_luckydraw_pre_render($luckydraw) {
  return array(
    'theme' => 'luckydraw_wheel_output',
    'variables' => array('luckydraw' => $luckydraw)
  );
}

/**
 * Allow to describe different rates algorithms
 * @param $variables
 *  An array of factors which results in the rates
 */
function hook_luckydraw_draw_rates(&$variables) {}

/**
 * The luckydraw entity will be validated before user playing
 * this is pretty useful, such as check whether the user has
 *
 * @param $luckydraw
 * @param $account
 * @return array
 */
function hook_luckydraw_process_validate($luckydraw, $account) {

  return array(
    'module_name' => array(
      'message' => '',
    ),
  );
}

function hook_luckydraw_save_transaction($luckydraw, $account, $luckydraw_item_id) {}