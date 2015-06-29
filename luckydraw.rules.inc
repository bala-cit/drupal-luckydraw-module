<?php
/**
 * @file luckydraw.rules.inc
 *
 * Define basic rules events for won or lose a prize, other modules should be able to extends or implements
 * specific actions, such as luckydraw_userpoints module implements actions of returning / reducing
 * points based on drawn result.
 *
 */

/**
 * Implements hook_rules_event_info().
 *
 * @return array
 */
function luckydraw_rules_event_info() {
  $items = array(
    'luckydraw_user_won_a_prize' => array(
      'label' => t('After user has won a prize'),
      'group' => t('Luckydraw'),
      'variables' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
        'luckydraw' => array(
          'type' => 'luckydraw',
          'label' => t('Luckydraw'),
        ),
        'prize' => array(
          'type' => array(
            'type' => 'integer',
            'label' => t('The prize (luckydraw item) id.'),
          ),
        ),
      ),
    ),
    'luckydraw_user_drawn_no_prize' => array(
      'label' => t('After user has drawn no prize'),
      'group' => t('Luckydraw'),
      'variables' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
      ),
    ),
  );

  return $items;
}

/**
 * Implements hook_rules_action_info().
 * Define the behavior on when user win a prize or drawn no prize
 */
function luckydraw_rules_action_info() {

}

/**
 * Implements hook_rules_data_info().
 */
function luckydraw_rules_data_info() {
  $data_types = array(
    'luckydraw' => array(
      'label' => t('Luckydraw entity'),
      'wrap' => TRUE, // It makes rules wrap the passed object and make it availabe for data selection.
      'group' => t('Luckydraw'),
    ),
  );

  return $data_types;
}

/**
 * Implements hook_rules_data_type_info()
 *
 * @return array
 */
//function luckydraw_rules_data_type_info() {
//  return array(
//    'luckydraw' => array(
//      'label' => t('Luckydraw entity'),
//      'class' => 'rules_data_type_luckydraw',
//      'savable' => FALSE,
//      'identifiable' => TRUE,
//      'module' => 'Lucky draw',
//    ),
//  );
//}
//
//class rules_data_type_luckydraw extends rules_data_type {
//  function load($lid) {
//    return luckydraw_load($lid);
//  }
//
//  function get_identifier() {
//    $luckydraw = $this->get();
//    return $luckydraw->lid;
//  }
//}