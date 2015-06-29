<?php
/**
 * @file
 *  luckydraw_item.admin.inc
 */

class LuckyDrawItemUIController extends EntityDefaultUIController {

  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;
    $plural_label = isset($this->entityInfo['plural label']) ? $this->entityInfo['plural label'] : $this->entityInfo['label'] . 's';

    $path = $this->path . '/%luckydraw/items';

    $items[$path] = array(
      'title' => 'Lucky Draws Items',
      'page callback' => 'drupal_get_form',
      'page arguments' => array($this->entityType . '_overview_form', $this->entityType),
      'description' => 'Manage ' . $plural_label . '.',
      'access callback' => 'entity_access',
      'access arguments' => array('view', $this->entityType),
      'file' => 'includes/entity.ui.inc',
    );

    $items[$path . '/add'] = array(
      'title' => 'Add a lucky draw item',
      'page callback' => 'luckydraw_item_add_page',
      'page arguments' => array(4, luckydraw_item_create(array())),
      'access callback' => 'luckydraw_access',
      'access arguments' => array('create', $this->entityType),
      'type' => MENU_LOCAL_ACTION,
      'file' => 'includes/luckydraw_item.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    $items[$path . '/' . $wildcard . '/edit'] = array(
      'title' => 'Edit a lucky draw item',
      'page callback' => 'luckydraw_item_form_wrapper',
      'page arguments' => array(4, 6, 'edit'),
      'access callback' => 'luckydraw_access',
      'access arguments' => array('edit', $this->entityType),
      'file' => 'includes/luckydraw_item.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    $items[$path . '/' . $wildcard . '/delete'] = array(
      'title' => 'Delete a luckydraw item',
      'page callback' => 'luckydraw_item_delete_wrapper',
      'page arguments' => array($id_count + 2),
      'access callback' => 'luckydraw_access',
      'access arguments' => array('edit'),
      'file' => 'includes/luckydraw_item.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    $items['luckydraw-item/' . $wildcard] = array(
      'title' => 'View a luckydraw item',
      'page callback' => 'luckydraw_item_view_page',
      'page arguments' => array(1),
      'access callback' => 'luckydraw_access',
      'access arguments' => array('view'),
      'file' => 'includes/luckydraw_item.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    return $items;
  }

  /**
   * Override overvewTable to append luckydraw item parent id to filter the items only belows to the parent
   *
   * @param array $conditions
   * @return Array
   */
  public function overviewTable($conditions = array()) {

    $url_comp = explode('/', request_uri());

    $conditions += array(
      'lid' => $url_comp[5] // The luckydraw entity identifier from url component
    );

    return parent::overviewTable($conditions);
  }

  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $entity_uri = entity_uri($this->entityType, $entity);

    $row[] = array(
      'data' => array(
        '#theme' => 'entity_ui_overview_item',
        '#label' => entity_label($this->entityType, $entity),
        '#name' => !empty($this->entityInfo['exportable']) ? entity_id($this->entityType, $entity) : FALSE,
        '#url' => $entity_uri ? $entity_uri : FALSE,
        '#entity_type' => $this->entityType,
      ),
    );

    $additional_cols += array(
      $entity->data['rate'] . ' %',
      $entity->weight
    );

    // Add in any passed additional cols.
    foreach ($additional_cols as $col) {
      $row[] = $col;
    }

    // Add a row for the exportable status.
    if (!empty($this->entityInfo['exportable'])) {
      $row[] = array('data' => array(
          '#theme' => 'entity_status',
          '#status' => $entity->{$this->statusKey},
        ));
    }

    // For exportable entities we add an export link.
    $exportable = !empty($this->entityInfo['exportable']);
    // If i18n integration is enabled, add a link to the translate tab.
    $i18n = !empty($this->entityInfo['i18n controller class']);

    $path = $this->path . '/' . $entity->lid . '/items';

    // Add operations depending on the status.
    if (entity_has_status($this->entityType, $entity, ENTITY_FIXED)) {
      $row[] = array(
        'data' => l(t('clone'), $path . '/' . $id . '/clone'),
        'colspan' => $this->operationCount(),
      );
    }
    else {
      $row[] = l(t('edit'), $path . '/' . $id . '/edit');

      if ($exportable) {
        $row[] = l(t('clone'), $path . '/' . $id . '/clone');
      }

      if (empty($this->entityInfo['exportable']) || !entity_has_status($this->entityType, $entity, ENTITY_IN_CODE)) {
        $row[] = l(t('delete'), $path . '/' . $id . '/delete', array('query' => drupal_get_destination()));
      }
      elseif (entity_has_status($this->entityType, $entity, ENTITY_OVERRIDDEN)) {
        $row[] = l(t('revert'), $path . '/' . $id . '/revert', array('query' => drupal_get_destination()));
      }
      else {
        $row[] = '';
      }
    }
    if ($exportable) {
      $row[] = l(t('export'), $path . '/manage/' . $id . '/export');
    }

    return $row;
  }

  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $additional_header = array(t('Rate'), t('Weight'));
    return parent::overviewTableHeaders($conditions, $rows, $additional_header);
  }
}


function luckydraw_item_add_page($luckydraw, $luckydraw_item) {
  return drupal_get_form('luckydraw_item_form', $luckydraw, $luckydraw_item);
}

function luckydraw_item_form_wrapper($luckydraw, $luckydraw_item, $op) {
  return drupal_get_form('luckydraw_item_form', $luckydraw, $luckydraw_item);
}

/**
 * luckydraw item form
 */
function luckydraw_item_form($form, &$form_state, $luckydraw, $luckydraw_item = NULL) {

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Give a name/label to the prize.'),
    '#required' => TRUE,
    '#default_value' => isset($luckydraw_item->name) ? $luckydraw_item->name : '',
  );

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['generic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Generic'),
    '#group' => 'additional_settings',
  );

  $form['generic']['data']['#tree'] = TRUE;

  $form['generic']['data']['rate'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($luckydraw_item->data['rate']) ? $luckydraw_item->data['rate'] : '',
    '#title' => t('Rate'),
    '#description' => t('The percentage of hitting the prize.'),
    '#field_suffix' => '%',
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
  );

  $form['generic']['weight'] = array(
    '#type' => 'select',
    '#title' => t('Weight'),
    '#options' => drupal_map_assoc(range(-50, 50)),
    '#default_value' => isset($luckydraw_item->weight) ? $luckydraw_item->weight : 0,
  );

  $form['generic']['data']['lose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Has drawn nothing'),
    '#description' => t('Checked by stating the item is losing / draw nothing, sorry to the player.'),
    '#default_value' => isset($luckydraw_item->data['lose']) ? $luckydraw_item->data['lose'] : 0,
  );

  $form['generic']['data']['message'] = array(
    '#type' => 'text_format',
    '#title' => t('Message to display'),
    '#default_value' => isset($luckydraw_item->data['message']['value']) ? $luckydraw_item->data['message']['value'] : '',
  );

  // Load the luckydraw type specific fields form
  // The specific module should define the fields with a function in TYPE.module
  // luckydraw_TYPE_luckydraw_item_fields_form

  $additional_fields = call_user_func('luckydraw_' . $luckydraw->type . '_luckydraw_item_fields_form',
    isset($luckydraw_item->data) ? $luckydraw_item->data : array()
  );

  $form['generic']['data'] += $additional_fields;

  $form_state['luckydraw'] = $luckydraw;
  $form_state['luckydraw_item'] = $luckydraw_item;

  $form['actions'] = array();

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configurations'),
    '#submit' => array('luckydraw_item_edit_form_submit'),
  );

  $form['#validate'][] = 'luckydraw_item_edit_form_validate';

  return $form;
}

function luckydraw_item_edit_form_validate(&$form, &$form_state) {

}

/**
 * @param $form
 * @param $form_state
 */
function luckydraw_item_edit_form_submit(&$form, &$form_state) {
  global $user;

  $luckydraw_item = entity_ui_controller('luckydraw_item')->entityFormSubmitBuildEntity($form, $form_state);

  $luckydraw_item->lid = $form_state['luckydraw']->lid;
  $luckydraw_item->changed = REQUEST_TIME;
  $luckydraw_item->uid = $user->uid;

  $luckydraw_item->save();

  $form_state['redirect'] = 'admin/content/luckydraws/manage/' . $form_state['luckydraw']->lid . '/items';
}

/**
 * @param $luckydraw_item
 */
function luckydraw_item_delete_wrapper($luckydraw_item) {
  return drupal_get_form('luckydraw_item_delete_confirm', $luckydraw_item);
}

/**
 * @param $form
 * @param $form_state
 * @param $luckydraw_item
 * @return mixed
 */
function luckydraw_item_delete_confirm($form, &$form_state, $luckydraw_item) {

  $form['markup']['#markup'] =
    t('Are you sure you want to delete the luckydraw item !name?
    This action cannot be undo and the items could also linked to users who won the prize,
    after deleted, the history of this will be lost!', array('!name' => $luckydraw_item->name));

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm to delete'),
    '#prefix' => '<p>',
    '#suffix' => '<a href="/admin/content/luckydraws/manage/' . $luckydraw_item->lid . '/items">' . t('Cancel') . '</a></p>',
  );

  $form_state['luckydraw_item'] = $luckydraw_item;

  return $form;
}

/**
 * Delete confirmation
 *
 * @param $form
 * @param $form_state
 */
function luckydraw_item_delete_confirm_submit($form, &$form_state) {
  luckydraw_item_delete($form_state['luckydraw_item']);
  drupal_set_message(t('!name is being deleted.', array('!name' => $form_state['luckydraw_item']->name)));
}

/**
 * @param $luckydraw_item
 */
function luckydraw_item_view_page($luckydraw_item) {
  drupal_goto('admin/content/luckydraws/manage/' . $luckydraw_item->lid . '/items/' . $luckydraw_item->liid . '/edit');
}