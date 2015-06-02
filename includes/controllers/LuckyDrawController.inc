<?php
/**
 * @file
 *  LuckyDrawController
 */

class LuckyDrawController extends EntityAPIControllerExportable {

  public function create(array $values = array()) {
    global $user;

    // Default values
    $values += array(
      'lid' => '',
      'name' => '',
      'description' => '',
      'settings' => '',
      'is_new' => TRUE,
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
    );

    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('luckydraw', $entity);

    /*
    $content['author'] = array('#markup' => t('Created by: !author', array(
      '!author' => $wrapper->uid->name->value(array('sanitize' => TRUE)))));
*/
    $content['description'] = array();

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}
