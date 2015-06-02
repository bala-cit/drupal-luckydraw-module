<?php
/**
 * @file
 *  LuckyDrawType entity class
 */

class LuckyDrawType extends Entity {
  public $type;
  public $name;
  public $weight = 0;

  public function __contruct($values = array()) {
    parent::__contruct($values, 'luckydraw_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE || $this->status & ENTITY_FIXED));
  }
}
