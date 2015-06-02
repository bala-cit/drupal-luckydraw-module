<?php
/**
 * @file
 *  luckydraw_item.admin.inc
 */

class LuckyDrawItemUIController extends EntityDefaultUIController {

  public function hook_menu() {
    return parent::hook_menu();
  }
}

/**
 * luckydraw item form
 */
function luckydraw_item_form($form, &$form_state, $luckydraw_item = NULL) {

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Give a name/label to the prize.'),
    '#default_value' => isset($luckydraw_item->name) ? $luckydraw_item->name : '',
  );

  return $form;
}
