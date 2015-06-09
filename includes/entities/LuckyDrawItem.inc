<?php
/**
 * @file
 *  LuckyDrawItem entity
 */

class LuckyDrawItem extends Entity {

  public $lid;
  public $liid;
  public $name;
  public $data;
  public $weight;
  public $uid;

  public function __contruct($values = array()) {
    parent::__contruct($values, 'luckydraw_item');
  }

  protected function defaultLabel() {
    return $this->name;
  }

  protected function defaultUri() {
    return array('path' => 'luckydraw-item/' . $this->identifier());
  }
}
