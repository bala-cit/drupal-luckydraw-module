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
 * Allow to describe different rates algorithms
 * @param $variables
 *  An array of factors which results in the rates
 */
function hook_luckydraw_draw_rates(&$variables) {}
