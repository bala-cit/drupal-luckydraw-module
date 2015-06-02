<?php
/**
 * @file
 *  LuckyDrawItem entity
 */

class LuckyDrawItem extends Entity {

  public $name;
  public $data;

  protected function defaultLabel() {
    return $this->name;
  }

  protected function defaultUri() {
    return array('path' => 'luckydraw-item/' . $this->identifier());
  }
}
