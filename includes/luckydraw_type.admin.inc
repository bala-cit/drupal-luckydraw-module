<?php
/**
 * @file
 *  LuckyDrawTypeUIController
 */

class LuckyDrawTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage luckydraw entity types.';
    return $items;
  }
}

function luckydraw_type_form($form, &$form_state, $luckydraw_type, $op = 'edit', $entity_type = NULL) {

  if ($op == 'clone') {
    $luckydraw_type->name .= ' (cloned)';
    $luckydraw_type->type .= $entity_type . '_clone';
  }

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => isset($luckydraw_type->name) ? $luckydraw_type->name : '',
  );

  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($luckydraw_type->type) ? $luckydraw_type->type : '',
    '#disabled' => $luckydraw_type->isLocked(),
    '#machine_name' => array(
      'exists' => 'luckydraw_get_types',
      'source' => array('name'),
    ),
    '#description' => t('A unique machine-readable name for this luckydraw type. It must contain lowercase letters, numbers and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($luckydraw_type->description) ? $luckydraw_type->description : '',
    '#description' => t('A brief introduction to this luckydraw type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save luckydraw type'),
    '#weight' => 40,
  );

  return $form;
}

function luckydraw_type_form_submit(&$form, &$form_state) {
  $luckydraw_type = entity_ui_form_submit_build_entity($form, $form_state);

  $luckydraw_type->save();
  $form_state['redirect'] = 'admin/structure/luckydraw-types';
}

function luckydraw_type_form_delete_confirm($luckydraw_type) {
  print_r($luckydraw_type);die;
}
