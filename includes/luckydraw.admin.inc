<?php
/**
 * @file
 *  luckydraw.admin.inc
 */

class LuckyDrawUIController extends EntityDefaultUIController {

  /**
   * Overwrides hook_menu() defaults. Since parent class hook_menu() is
   * mainly  for entity type administration.
   */
  public function hook_menu() {
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;
    $plural_label = isset($this->entityInfo['plural label']) ? $this->entityInfo['plural label'] : $this->entityInfo['label'] . 's';

    $items[$this->path] = array(
      'title' => 'Lucky Draws',
      'page callback' => 'drupal_get_form',
      'page arguments' => array($this->entityType . '_overview_form', $this->entityType),
      'description' => 'Manage ' . $plural_label . '.',
      'access callback' => 'entity_access',
      'access arguments' => array('view', $this->entityType),
      'file' => 'includes/entity.ui.inc',
    );

    // Change the overview menu type for the list of models.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    $items[$this->path . '/add'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', $this->entityType),
      'description' => 'Add a luckydraw.',
      'page callback' => 'luckydraw_add_page',
      //'page arguments' => array($this->entityType, NULL, 'add'),
      'access callback' => 'luckydraw_access',
      'access arguments' => array('create', $this->entityType),
      'type' => MENU_NORMAL_ITEM,
      'file' => 'includes/luckydraw.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    foreach (luckydraw_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->name,
        'description' => $type->description,
        'page callback' => 'luckydraw_form_wrapper',
        'page arguments' => array($id_count + 1),
        'access callback' => 'luckydraw_access',
        'access arguments' => array('edit', 'edit any luckydraw entities'),
        'file' => 'includes/luckydraw.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module']),
      );
    }

    $items[$this->path . '/manage/' . $wildcard] = array(
      'page callback' => 'luckydraw_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'luckydraw_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'includes/luckydraw.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    $items[$this->path . '/manage/' . $wildcard . '/results'] = array(
      'page callback' => 'luckydraw_results_page',
      'page arguments' => array($id_count + 1),
      'access callback' => 'luckydraw_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'includes/luckydraw.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    $items[$this->path . '/manage/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/manage/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'luckydraw_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'luckydraw_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'includes/luckydraw.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    $items['luckydraw/' . $wildcard] = array(
      //'title callback'
      'page callback' => 'luckydraw_page_view',
      'page arguments' => array(1),
      'access callback' => 'luckydraw_access',
      'access arguments' => array('view', 'luckydraw entities'),
      'type' => MENU_NORMAL_ITEM,
    );

    return $items;
  }

  /**
   * Overrides overviewTableRow. Since we need to change the links.
   *
   * @param $conditions
   * @param $id
   * @param $entity
   * @param array $additional_cols
   * @return array
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);
    $row[] = l(t('items'), $this->path . '/manage/' . $id . '/items');
    $row[] = l(t('results'), $this->path . '/manage/' . $id . '/results');
    return $row;
  }

  /**
   * Overrides table headers to add more links
   *
   * @param $conditions
   * @param $rows
   * @param array $additional_header
   * @return array
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $headers = parent::overviewTableHeaders($conditions, $rows, $additional_header);

    foreach ($headers as &$header) {
      if (is_array($header) && $header['data'] == t('Operations')) {
        $header['colspan'] += 2; // add two addition links
      }
    }

    return $headers;
  }
}

function luckydraw_form($form, &$form_state) {
  return __FUNCTION__;
}

/**
 * Add a luckydraw page
 * If there is just one type will directly to the add form, otherwise, to select one.
 *
 * @return string
 * @throws Exception
 */
function luckydraw_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  // If only single type of luckydraw, directly go to it
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  return theme('luckydraw_add_list', array('content' => $content));
}

/**
 * Luckydraw add / edit form wrapper
 *
 * @param null $arg
 * @return array|mixed
 */
function luckydraw_form_wrapper($arg = NULL) {
  if (empty($arg) || !is_object($arg)) {
    $arg = luckydraw_create(array('type' => $arg));
  }
  return drupal_get_form('luckydraw_edit_form', $arg);
}

/**
 * @param $luckydraw
 * @return array|mixed
 */
function luckydraw_delete_form_wrapper($luckydraw) {
  return drupal_get_form('luckydraw_delete_confirm', $luckydraw);
}

/**
 * @param $form
 * @param $form_state
 * @param $luckydraw
 * @return mixed
 */
function luckydraw_delete_confirm($form, &$form_state, $luckydraw) {

  $form['markup']['#markup'] =
    t('Are you sure you want to delete the luckydraw !name and all its items?
    This action cannot be undo and the items could also linked to users who won the prize,
    after deleted, the history of this will be lost!', array('!name' => $luckydraw->name));

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm to delete'),
    '#prefix' => '<p>',
    '#suffix' => '<a href="/admin/content/luckydraws">' . t('Cancel') . '</a></p>',
  );

  $form_state['luckydraw'] = $luckydraw;

  return $form;
}

/**
 * Delete confirmation
 *
 * @param $form
 * @param $form_state
 */
function luckydraw_delete_confirm_submit($form, &$form_state) {
  luckydraw_delete($form_state['luckydraw']);
  drupal_set_message(t('!name and its items are being deleted.', array('!name' => $form_state['luckydraw']->name)));
}

/**
 * Define the luckydraw basic settings. For adding other more specific configs,
 * the module can implements form_alter hook to add more options.
 */
function luckydraw_edit_form($form, &$form_state, $luckydraw) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Lucky Draw Name'),
    '#description' => t('This is for management, to set a different title for user, use Display Title below.'),
    '#default_value' => isset($luckydraw->name) ? $luckydraw->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('An introduction to the luckydraw for administrators.'),
    '#default_value' => isset($luckydraw->description) ? $luckydraw->description : '',
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Publish / Unpublish'),
    '#description' => t('Active the luckydraw by check it or disable by uncheck it.'),
    '#default_value' => isset($luckydraw->status) ? $luckydraw->status : 1,
  );

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['display_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#group' => 'additional_settings',
  );

  $form['display_settings']['settings']['#tree'] = TRUE;

  $form['display_settings']['settings']['display_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Display Title'),
    '#description' => t('Display this title of the lucky draw to users.'),
    '#default_value' => isset($luckydraw->settings['display_title']) ? $luckydraw->settings['display_title'] : '',
  );

  $form['display_settings']['settings']['display_help_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#description' => t('Display this help text on the lucky draw to users.'),
    '#default_value' => isset($luckydraw->settings['display_help_text']) ? $luckydraw->settings['display_help_text'] : '',
  );
//
//  $form['display_settings']['settings']['win_message'] = array(
//    '#type' => 'textarea',
//    '#title' => t('Message to display when user won a prize'),
//    '#description' => t('Give a friendly message to end user when win a prize.'),
//    '#default_value' => isset($luckydraw->settings['win_message']) ? $luckydraw->settings['win_message'] : '',
//  );
//
//  $form['display_settings']['settings']['lose_message'] = array(
//    '#type' => 'textarea',
//    '#title' => t('Message to display when user has drawn no prize'),
//    '#description' => t('Give a friendly message to end user when they have drawn no prize.'),
//    '#default_value' => isset($luckydraw->settings['lose_message']) ? $luckydraw->settings['lose_message'] : '',
//  );

  //$form['additional_settings']['settings']['#tree'] = TRUE;

  $form['other_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Others'),
    '#group' => 'additional_settings',
    '#weight' => 50,
  );

  $form['other_settings']['settings']['#tree'] = TRUE;

  $form['other_settings']['settings']['allow_replay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow replay'),
    '#description' => t('When checked on, the user will be able to replay as many times as possible.'),
    '#default_value' => isset($luckydraw->settings['allow_replay']) ? $luckydraw->settings['allow_replay'] : 0,
  );

  $form['other_settings']['settings']['create_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create block'),
    '#description' => t('Create as a block so you can put it on a different page.'),
    '#default_value' => isset($luckydraw->settings['create_block']) ? $luckydraw->settings['create_block'] : 0,
  );

  $form_state['luckydraw'] = $luckydraw;

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save lucky draw'),
    '#submit' => array('luckydraw_edit_form_submit'),
  );

  if (!empty($luckydraw->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete lucky draw'),
      '#suffix' => l(t('Cancel'), 'admin/content/luckydraws'),
      '#submit' => array('luckydraw_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'luckydraw_edit_form_validate';
  return $form;
}

function luckydraw_edit_form_validate(&$form, &$form_state) {}

function luckydraw_edit_form_submit(&$form, &$form_state) {
  $luckydraw = entity_ui_controller('luckydraw')->entityFormSubmitBuildEntity($form, $form_state);

  $luckydraw->changed = REQUEST_TIME;

  $luckydraw->save();
  $form_state['redirect'] = 'admin/content/luckydraws';
}

function luckydraw_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/luckydraws';
}

function theme_luckydraw_add_list(&$variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="luckydraw-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dt>';
    }
    $output .= '</dl>';
  }
  else {
    $output = '<p>' . t('No luckydraw types have been created yet.
      Please enable the module implements the luckydraw type firstly.') . '</p>';
  }

  return $output;
}

/**
 * The luckydraw result list
 *
 * @param $luckydraw
 * @return rendable array
 */
function luckydraw_results_page($luckydraw) {
  $transactions = luckydraw_transactions_load($luckydraw);

  // This function only exists for PHP > 5.5.0
  if (!function_exists('array_columns')) {
    function array_column($array, $column_name) {
      return array_map(function($element) use($column_name) {return $element[$column_name];}, $array);
    }
  }

  $uids = array_column($transactions, 'uid');
  $users = user_load_multiple($uids);

  $headers = array(
    array('data' => t('User'), 'field' => 'uid'),
    array('data' => t('Drawn')),
    array('data' => t('Result')),
    array('data' => t('When'), 'field' => 'created')
  );

  $rows = array();
  $luckydraw_item_path = 'luckydraw-item/';
  foreach ($transactions as $tran) {
    $rows[] = array(
      $users[$tran['uid']]->name,
      l($tran['name'], $luckydraw_item_path . $tran['liid']),
      $tran['data'],
      date('Y-m-d H:i:s', $tran['created'])
    );
  }

  $variables = array(
    'header' => $headers,
    'rows' => $rows,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => t('There is no results yet'),
  );

  return theme_table($variables);
}