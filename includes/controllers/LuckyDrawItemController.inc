<?php
/**
 * @file
 *  LuckyDrawItemController
 */

class LuckyDrawItemController extends EntityAPIControllerExportable {

  public function create(array $values = array()) {
    global $user;

    $values += array(
      'weight' => 0,
      'data' => '',
      'created' => REQUEST_TIME,
      'updated' => REQUEST_TIME,
      'uid' => $user->uid,
    );

    return parent::create($values);
  }
}
