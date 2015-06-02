<?php
/**
 * @file
 *  LuckyDraw entity class
 */

/**
 * Defaults the title and Uri.
 */
class LuckyDraw extends Entity {

  public $name;
  public $type;
  public $description;
  public $settings;
  public $created;
  public $changed;
  public $uid;

  public function __contruct($values = array()) {
    parent::__contruct($values, 'luckydraw');
  }

  protected function defaultLabel() {
    return $this->name;
  }

  protected function defaultUri() {
    return array('path' => 'luckydraw/' . $this->identifier());
  }
}
