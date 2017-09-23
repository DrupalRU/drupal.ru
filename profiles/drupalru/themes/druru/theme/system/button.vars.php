<?php
/**
 * @file
 * button.vars.php
 */

/**
 * Implements hook_preprocess_button().
 */
function druru_preprocess_button(&$vars) {
  $vars['element']['#attributes']['class'][] = 'btn';
  if (isset($vars['element']['#value'])) {
    if ($class = _druru_colorize_button($vars['element']['#value'])) {
      $vars['element']['#attributes']['class'][] = $class;
    }
    $iconize = theme_get_setting('druru_iconize');
    $value = $vars['element']['#value'];
    if ($iconize && $icon = druru_get_icon_by_title($value)) {
      /*
       * Move value from #value to #title for correct detect triggering
       * element in system func _form_button_was_clicked(), because it
       * compared button value received trough $_POST variables and fetched
       * from form by key. The theme changed default HTML element from
       * <input> to <button>, therefore we can save original value in button
       * attribute (value) and change html inside button - between
       * <button> and </button>.
       *
       * @see _form_button_was_clicked()
       * @see druru_button()
       */
      $vars['element']['#label'] = $icon . $value;
    }
  }
  $elt     = &$vars['element'];
  $classes = &$elt['#attributes']['class'];

  /*
   * This we perform checking an existing bootstrap classes.
   * Bootstrap (base) theme added they bootstrap classes to buttons always.
   * But we can add our different classes before that BT (base theme)
   * handled they.
   * Therefore we should delete last added classes.
   * For example:
   * we add classes ['btn', 'btn-primary']
   * BT add classes ['btn', 'btn-success']
   * We should delete last two classes this.
   */
  $btn_exists       = FALSE;
  $btn_style_exists = FALSE;
  foreach ($classes as $key => $class) {
    if ('btn' == $class) {
      if ($btn_exists) {
        unset($classes[$key]);
      }
      else {
        $btn_exists = TRUE;
      }
    }

    if (strpos($class, 'btn-') !== FALSE) {
      if ($btn_style_exists) {
        unset($classes[$key]);
      }
      else {
        $btn_style_exists = TRUE;
      }
    }
  }
}
