<?php

/**
 * Implements hook_form_FORM_ID_alter() for privatemsg_list().
 *
 * @see privatemsg_list()
 */
function druru_form_privatemsg_list_alter(&$form, &$form_state) {
  if (isset($form['filter'])) {
    hide($form['filter']);
  }
  if (isset($form['updated']['actions'])) {
    hide($form['updated']['actions']);
  }
  if (isset($form['updated']['list'])) {
    $items = array();
    foreach ($form['updated']['list']['#options'] as $thread) {
      $item = array();
      $last_change = $thread['last_updated'];

      // New messages.
      if ($thread['is_new']) {
        $item[] = array(
          '#type'       => 'html_tag',
          '#tag'        => 'span',
          '#value'      => format_plural($thread['is_new'], '1 new', '@count new'),
          '#attributes' => array(
            'class' => array('label', 'label-success', 'pull-right'),
          ),
        );
      }

      // Subject.
      $item[] = array(
        '#type'       => 'html_tag',
        '#tag'        => 'div',
        '#value'      => check_plain($thread['subject']),
        '#attributes' => array(
          'class' => array(
            'text-primary',
            'participants',
            'h3',
          ),
        ),
      );

      // Time.
      $item[] = array(
        '#type'       => 'html_tag',
        '#tag'        => 'small',
        '#value'      => format_interval(REQUEST_TIME - $last_change),
        '#attributes' => array(
          'class' => array('text-muted', 'pull-right'),
        ),
      );

      // Users.
      $participants = theme('privatemsg_list_field__participants', array(
        'thread' => $thread,
      ));
      $item[] = array(
        '#type'       => 'html_tag',
        '#tag'        => 'div',
        '#value'      => filter_xss($participants['data'], array()),
        '#attributes' => array(
          'class' => array('subject'),
        ),
      );

      $items[] = array(
        '#theme'   => 'link',
        '#text'    => render($item),
        '#path'    => 'messages/view/' . $thread['thread_id'],
        '#options' => array(
          'attributes' => array(),
          'html'       => TRUE,
        ),
      );
    }
    $form['list'] = array(
      '#theme'      => 'bootstrap_list_group',
      '#type'       => 'links',
      '#items'      => $items,
      '#attributes' => array(
        'id' => 'privatemsg-list-wrapper',
      ),
    );
    hide($form['updated']['list']);
    $form['pager'] = $form['updated']['pager'];
    hide($form['updated']['pager']);
  }
  $form['updated']['actions']['#prefix'] = '<div class="form-inline">';
}

/**
 * Implements hook_form_FORM_ID_alter() for privatemsg_new().
 *
 * @see privatemsg_new()
 */
function druru_form_privatemsg_new_alter(&$form, &$form_state) {
  if (isset($form['actions']['cancel'])) {
    hide($form['actions']['cancel']);
  }
  if (isset($form['token'])) {
    hide($form['token']);
  }
  if (isset($form['body'])) {
    $form['body']['#title_display'] = 'invisible';
  }

  $submit = &$form['actions']['submit'];
  $class = _druru_colorize_button($submit['#value']);
  if ($class) {
    $submit['#attributes']['class'][] = 'btn';
    $submit['#attributes']['class'][] = $class;
  }

  // Delete unless UL in description.
  $description = &$form['recipient']['#description'];
  $description = mb_substr($description, 0, mb_strpos($description, '<'));
  $description .= '.';

  $form['top'] = array(
    '#type'       => 'container',
    '#weight'     => -10,
    '#attributes' => array(
      'class' => array('row'),
    ),
  );
  $form['top']['recipient'] = $form['recipient'];
  $form['top']['subject'] = $form['subject'];
  unset($form['recipient'], $form['subject']);

  $form['top']['recipient']['#title'] = t('Users');
  $form['top']['subject']['#title'] = t('Name of the dialog');
  $form['top']['recipient']['#title_display'] = 'invisible';
  $form['top']['subject']['#title_display'] = 'invisible';

  $form['top']['recipient']['#attributes']['placeholder'] = $form['top']['recipient']['#title'];
  $form['top']['subject']['#attributes']['placeholder'] = $form['top']['subject']['#title'];


  $form['top']['recipient']['#prefix'] = '<div class="col-sm-6">';
  $form['top']['recipient']['#suffix'] = '</div>';

  $form['top']['subject']['#prefix'] = '<div class="col-sm-6">';
  $form['top']['subject']['#suffix'] = '</div>';

  if (current_path() == 'messages') {
    $submit['#value'] = t('Create');
    $form['wrapper'] = array(
      '#type'        => 'fieldset',
      '#title'       => druru_icon('plus') . t('New dialog'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
      '#attributes'  => array(
        'class' => array('panel-primary'),
      ),
    );

    // Wrap the form in fideldset.
    foreach (element_children($form) as $element) {
      if ($element != 'wrapper') {
        $form['wrapper'][$element] = $form[$element];
        unset($form[$element]);
      }
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter() for privatemsg_view().
 *
 * @see privatemsg_view()
 */
function druru_privatemsg_view_alter(&$content) {
  // I think, it's useless functionality.
  hide($content['tags']);
  hide($content['participants']);
}

/**
 * Implements hook_form_FORM_ID_alter() for pm_block_user_list().
 *
 * @see pm_block_user_list()
 */
function druru_form_pm_block_user_list_alter(&$form, &$form_state) {
  $form['form'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('col-xs-12', 'col-sm-8'),
    ),
  );
  $form['actions'] = array(
    '#type'       => 'container',
    '#attributes' => array(
      'class' => array('col-xs-12', 'col-sm-4'),
    ),
  );

  foreach (element_children($form['new']) as $element) {
    $form['form'][$element] = $form['new'][$element];
  }

  $form['form']['name']['#attributes']['placeholder'] = $form['form']['name']['#title'];
  $form['form']['name']['#title_display'] = 'invisible';

  $form['actions']['submit'] = $form['form']['submit'];
  $form['actions']['submit']['#value'] = t('Ban');
  $form['actions']['submit']['#attributes']['class'][] = 'btn-warning btn-block';
  unset($form['new'], $form['form']['submit']);
}
