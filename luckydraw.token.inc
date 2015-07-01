<?php
/**
 * @file luckydraw_token_info().
 *
 */

/**
 * Implements hook_token_info().
 *
 * @return array
 */
function luckydraw_token_info() {
  $type = array(
    'name' => t('Luckydraw entity'),
    'description' => t('A single luckydraw entity'),
    'needs-data' => 'luckydraw',
  );

  $token['lid'] = array(
    'name' => t('Luckydraw entity identifier'),
    'description' => t('Luckydraw entity identifier'),
  );

  $token['name'] = array(
    'name' => t('Luckydraw name'),
    'description' => t('The luckydraw name displayed to the adminstrator.'),
  );

  $token['display_title'] = array(
    'name' => t('Display title'),
    'description' => t('The luckydraw name displayed to the user.'),
  );

  $token['display_help_text'] = array(
    'name' => t('Display help text'),
    'description' => t('The luckydraw help text displayed to the user.'),
  );

  $token['description'] = array(
    'name' => t('Luckydraw description'),
    'description' => t('The luckydraw description displayed to the administrator.'),
  );

  $token['type'] = array(
    'name' => t('Type'),
    'description' => t('The luckydraw type'),
  );

  return array(
    'types' => array('luckydraw' => $type),
    'tokens' => array('luckydraw' => $token),
  );
}

/**
 * Implements hook_tokens().
 *
 * @param $type
 * @param $tokens
 * @param $array
 */
function luckydraw_tokens($type, $tokens, $array $data = array(), array $options = array()) {

  $replacements = array();

  if ($type == 'luckydraw' && !empty($data['luckydraw'])) {
    $luckydraw_entity = $data['luckydraw'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'lid':
          $replacements[$original] = $luckydraw_entity->lid;
          break;

        case 'name':
          $replacements[$original] = $luckydraw_entity->name;
          break;

        case 'description':
          $replacements[$original] = $luckydraw_entity->settings['description'];
          break;

        case 'display_title':
          $replacements[$original] = $luckydraw_entity->settings['display_title'];
          break;

        case 'display_help_text':
          $replacements[$original] = $luckydraw_entity->settings['display_help_text'];
          break;

        case 'type':
          $replacements[$original] = $luckydraw_entity->type;
          break;
      }
    }
  }

  return $replacements;
}