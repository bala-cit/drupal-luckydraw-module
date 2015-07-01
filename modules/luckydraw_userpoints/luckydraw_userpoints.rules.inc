<?php
/**
 * @file
 *  Rules integration hooks
 */

/**
 * Implements hook_rules_action_info().
 *
 * @return array
 */
function luckydraw_userpoints_rules_action_info() {

  $actions['luckydraw_userpoints_action_return_points'] = array(
    'label' => t('Return !points to a user', userpoints_translation()),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'description' => t('The user will get !points.', userpoints_translation()),
      ),
      'luckydraw' => array(
        'type' => 'integer',
        //'parent' => 'entity',
        'label' => t('Luckydraw Id'),
        //'wrapped' => TRUE,
        'description' => t('Luckydraw is being played.'),
      ),
      'points' => array(
        'type' => 'integer',
        'label' => t('!Points', userpoints_translation()),
        'allow null' => TRUE,
        'description' => t('Amount of !points will return in default. Set as <none> to return all points used to play the draw.', userpoints_translation()),
      ),
    ),
    'group' => t('Luckydraw'),
  );

  return $actions;
}


/**
 * Rules action - return points to a user
 *
 * @param object $account
 * @param object $luckydraw
 * @param integer $points
 */
function luckydraw_userpoints_action_return_points($account, $luckydraw, $points) {
  $points = ($points < 0 && $luckydraw->settings['points_to_play']) ? $luckydraw->settings['points_to_play'] : $points;
  $variables = array(
    'points' => $points,
    'user' => $account,
  );
  userpoints_userpointsapi($variables);
}