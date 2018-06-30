<?php

/**
 * @file
 * Various additional functions.
 */

/**
 * Get all YMB-blocks as objects.
 */
function _ymb_get_all_blocks() {
  return db_select('ymb', 'n')
    ->fields('n')
    ->execute();
}

/**
 * Get YMB-block as object.
 */
function _ymb_get_block_by_bid($bid) {
  return db_select('ymb', 'n')
    ->fields('n')
    ->condition('n.bid', $bid)
    ->execute()
    ->fetchObject();
}

/**
 * Get custom block title from block_custom table.
 */
function _ymb_get_block_title($fieldname, $data) {
  return db_select('block_custom', 'n')
    ->fields('n', ['info'])
    ->condition("n.{$fieldname}", $data)
    ->execute()
    ->fetchField();
}

/**
 * Remove block data from all relative tables.
 */
function _ymb_remove_block_by_bid($bid) {
  db_delete('ymb')
    ->condition('bid', $bid)
    ->execute();
  db_delete('block_custom')
    ->condition('bid', $bid)
    ->execute();
  db_delete('block')
    ->condition('module', 'block')
    ->condition('delta', $bid)
    ->execute();
  db_delete('block_role')
    ->condition('module', 'block')
    ->condition('delta', $bid)
    ->execute();
}

/**
 * Output raising form.
 */
function _ymb_generate_template($bid, $receiver, $target, $sum, $url, $description) {
  $label_amount = t('Amount');
  $label_required = t('This field is required.');
  $label_pc = t('Yandex.Money');
  $label_ac = t('Bank card');
  $label_submit = t('Pay');
  return <<<EOT
  <div class='ymb-form'>
    <p>{$description}</p>
    <form method='POST' action='https://money.yandex.ru/quickpay/confirm.xml'>
      <div class='form-item form-type-textfield form-item-sum'>
        <label for='edit-sum--{$bid}'>{$label_amount} <span class='form-required' title='{$label_required}'>*</span></label><input id='edit-sum--{$bid}' name='sum' value='{$sum}' data-type='number' class='form-text required'>
      </div>
      <div class='form-item form-type-radios form-item-payment-type'>
        <div id='edit-payment-type--{$bid}' class='form-radios'>
          <div class='form-item form-type-radio form-item-payment-type'>
            <input id='edit-payment-type-pc--{$bid}' type='radio' name='paymentType' value='PC' class='form-radio'><label class='option' for='edit-payment-type-pc--{$bid}'>{$label_pc}</label>
          </div>
          <div class='form-item form-type-radio form-item-payment-type'>
            <input id='edit-payment-type-ac--{$bid}' type='radio' name='paymentType' value='AC' class='form-radio'><label class='option' for='edit-payment-type-ac--{$bid}'>{$label_ac}</label>
          </div>
        </div>
      </div>
      <div class='form-actions form-wrapper' id='edit-actions--{$bid}'>
        <input id='edit-submit--{$bid}' type='submit' value='{$label_submit}' class='form-submit'>
      </div>
      <input type='hidden' name='receiver' value='{$receiver}'><input type='hidden' name='targets' value='{$target}'><input type='hidden' name='quickpay-form' value='donate'><input type='hidden' name='successURL' value='{$url}'>
    </form>
  </div>
EOT;
}
